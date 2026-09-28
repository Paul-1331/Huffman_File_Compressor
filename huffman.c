#include "huffman.h"
#include "bitio.h"
#include "huffman_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// write metadata/header of compressed file
static int write_header(FILE*out, uint64_t freq_table[HUFFMAN_SYMBOLS]){
    if(fwrite(HUFFMAN_MAGIC,1,4,out)!=4){
        return 0;
    }

    for(int i = 0;i<HUFFMAN_SYMBOLS;i++){
        if(fwrite(&freq_table[i],sizeof(freq_table[i]),1,out)!=1){
            return 0;
        }
    }
    return 1;
}

static int read_header(FILE*in, uint64_t freq_table[HUFFMAN_SYMBOLS]){
    char magic[4];

    if(fread(magic,1,4,in)!=4){
        return 0;
    }

    if(memcmp(magic,HUFFMAN_MAGIC,4)!=0){
        return 0;
    }

    for(int i = 0;i<HUFFMAN_SYMBOLS;i++){
        if(fread(&freq_table[i],sizeof(freq_table[i]),1,in)!=1){
            return 0;
        }
    }
    return 1;
}


int huffman_compress(const char*input_path, const char*output_path){
    uint64_t freq_table[HUFFMAN_SYMBOLS] = {0};
    HuffmanCode codes[HUFFMAN_SYMBOLS] = {0};
    
    FILE*input = fopen(input_path,"rb");
    if(!input){
        perror(input_path);
        return 0;
    }

    int c;
    while((c=fgetc(input))!=EOF){
        freq_table[(unsigned char)c]++;
    }

    if(ferror(input)){
        perror("Reading input");
        fclose(input);
        return 0;
    }

    HuffmanNode*root = build_tree(freq_table);

    FILE *output = fopen(output_path, "wb");
    if (!output){
        perror(output_path);
        fclose(input);
        free_tree(root);
        return 0;
    }

    if (!write_header(output, freq_table)){
        fprintf(stderr, "Error writing Huffman header.\n");
        fclose(input);
        fclose(output);
        free_tree(root);
        return 0;
    }

    if (!root){
        fclose(input);
        fclose(output);
        return 1;
    }

    // generate the huffman code for each symbol
    if (!build_codes(root, codes)){
        fclose(input);
        fclose(output);
        free_tree(root);
        return 0;
    }

    // Return to the beginning for the second pass used for encoding.
    rewind(input);

    BitWriter writer;
    bit_writer_init(&writer, output);

    while((c=fgetc(input))!=EOF) {
        HuffmanCode code = codes[(unsigned char)c];

        if (!bit_writer_write_code(&writer, code.code, code.len)) {
            fprintf(stderr, "Error writing compressed data.\n");
            fclose(input);
            fclose(output);
            free_tree(root);
            return 0;
        }
    }

    if (ferror(input)||!bit_writer_flush(&writer)){
        fprintf(stderr, "Error finalizing compressed data.\n");
        fclose(input);
        fclose(output);
        free_tree(root);
        return 0;
    }

    fclose(input);
    fclose(output);
    free_tree(root);
    return 1;
}

int huffman_decompress(const char*input_path,const char*output_path){
    uint64_t freq_table[HUFFMAN_SYMBOLS] = {0};

    FILE *input = fopen(input_path, "rb");
    if (!input){
        perror(input_path);
        return 0;
    }

    if (!read_header(input, freq_table)){
        fprintf(stderr, "Error: invalid or corrupted Huffman file.\n");
        fclose(input);
        return 0;
    }

    // Reconstruct the Huffman tree from the stored frequencies.
    HuffmanNode *root = build_tree(freq_table);

    FILE *output = fopen(output_path, "wb");
    if (!output){
        perror(output_path);
        fclose(input);
        free_tree(root);
        return 0;
    }

    if (!root){
        fclose(input);
        fclose(output);
        return 1;
    }

    // Special case: the file contains only one distinct symbol.
    // Reproduce that symbol according to its stored frequency.
    if (root->is_leaf){
        uint64_t remaining = root->freq;

        while (remaining > 0){
            if (fputc(root->symbol, output) == EOF){
                fprintf(stderr, "Error writing decompressed file.\n");
                fclose(input);
                fclose(output);
                free_tree(root);
                return 0;
            }
            remaining--;
        }

        fclose(input);
        fclose(output);
        free_tree(root);
        return 1;
    }

    // Determine how many symbols must be reconstructed.
    // This lets us ignore padding bits in the final byte.
    uint64_t total_symbols = 0;
    for (int i = 0; i < HUFFMAN_SYMBOLS; ++i){
        total_symbols += freq_table[i];
    }


    // Start decoding from the root of the Huffman tree.
    uint64_t decoded = 0;
    HuffmanNode *current = root;

    // Initialize bit-level input for the compressed bitstream.
    BitReader reader;
    bit_reader_init(&reader, input);

    // continue until we've reconstructed exactly the number of symbols that existed in the original file.
    while (decoded < total_symbols) {
        int bit = bit_reader_read_bit(&reader);

        if (bit < 0) {
            fprintf(stderr, "Error: compressed data ended unexpectedly.\n");
            fclose(input);
            fclose(output);
            free_tree(root);
            return 0;
        }

        current = bit==1 ? current->right : current->left;

        // A missing child means the bitstream does not match the Huffman tree.
        if(!current){
            fprintf(stderr, "Error: invalid Huffman bit stream.\n");
            fclose(input);
            fclose(output);
            free_tree(root);
            return 0;
        }

        // We reached a leaf, so output the decoded symbol.
        if(current->is_leaf){
            if (fputc(current->symbol, output) == EOF) {
                fprintf(stderr, "Error writing decompressed file.\n");
                fclose(input);
                fclose(output);
                free_tree(root);
                return 0;
            }

            decoded++;
            current = root;
        }
    }

    fclose(input);
    fclose(output);
    free_tree(root);
    return 1;
}