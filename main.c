#include<stdio.h>
#include<string.h>
#include "huffman.h"

static void print_usage(const char*program){
    printf("Usage:\n");
    printf(" %s compress <input_file> <output_file> \n",program);
    printf(" %s decompress <input_file> <output_file> \n",program);
    printf("\n");
    printf("Example:\n");
    printf(" %s compress input.txt compressed.huf\n",program);
    printf(" %s decompress compressed.huf restored.txt\n",program);
}

int main(int argc, char*argv[])
{
    if(argc!=4){
        print_usage(argv[0]);
        return 1;    
    }
    int success = 0;
    if(strcmp(argv[1],"compress")==0){
        success = huffman_compress(argv[2],argv[3]);
    }
    else if(strcmp(argv[1],"decompress")==0){
        success = huffman_decompress(argv[2],argv[3]);
    }
    else{
        print_usage(argv[0]);
        return 1;
    }
    if(!success){
        fprintf(stderr,"Operation failed\n");
        return 1;
    }

    printf("Operation completed successfully\n");
    return 0;
}