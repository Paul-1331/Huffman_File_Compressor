#ifndef HUFFMAN_H
#define HUFFMAN_H

#define HUFFMAN_MAGIC "HUF1"

int huffman_compress(const char*input_path, const char*output_path);
int huffman_decompress(const char*input_path, const char*output_path);

#endif