#ifndef HUFFMAN_H
#define HUFFMAN_H

#define HUFFMAN_MAGIC "HUF1"

int huffman_compress(const char*input, const char*output);
int huffman_decompress(const char*input, const char*output);

#endif