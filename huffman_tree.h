#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <stdint.h>

#define HUFFMAN_SYMBOLS 256

typedef struct HuffmanNode{
    uint64_t freq;
    unsigned char symbol;
    int is_leaf;
    struct HuffmanNode*left;
    struct HuffmanNode*right;
}HuffmanNode;

typedef struct HuffmanCode{
    uint64_t freq;
    uint32_t code; // assuming that max code length is only 32 bits
    uint8_t len; // actual length, ie number of bits(out of 32) used to represent a symbol
}HuffmanCode;

HuffmanNode*create_node(const uint64_t freq, unsigned char symbol, int is_leaf, HuffmanNode*left, HuffmanNode*right);

HuffmanNode*build_tree(const uint64_t freq_table[HUFFMAN_SYMBOLS]);
void free_tree(HuffmanNode*root);
int build_codes(const HuffmanNode*root, HuffmanCode codes[HUFFMAN_SYMBOLS]); // takes Huffman Tree and creates the HuffmanCode for each symbol ie byte (0-255) and fills the table codes

#endif