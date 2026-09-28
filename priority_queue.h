#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "huffman_tree.h"
#include <stddef.h>

#define HEAP_CAPACITY (HUFFMAN_SYMBOLS*2)

typedef struct MinHeap{
    HuffmanNode* data[HEAP_CAPACITY];
    size_t size;
}MinHeap;

MinHeap* minheap_create(void);
int minheap_push(MinHeap*heap,HuffmanNode*node);
HuffmanNode*minheap_pop(MinHeap*heap);
void minheap_destroy(MinHeap*heap);

#endif