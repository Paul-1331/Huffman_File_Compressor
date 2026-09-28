#include "huffman_tree.h"
#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

HuffmanNode*create_node(uint64_t freq, unsigned char symbol, int is_leaf, HuffmanNode*left,HuffmanNode*right){
    HuffmanNode*node = malloc(sizeof(HuffmanNode));
    if(!node) return NULL;

    node->freq = freq;
    node->is_leaf = is_leaf;
    node->left = left;
    node->right = right;
    node->symbol =  symbol;
    return node;
}

void free_tree(HuffmanNode*root){
    if(!root) return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

HuffmanNode*build_tree(const uint64_t freq_table[HUFFMAN_SYMBOLS]){
    MinHeap* minheap = minheap_create();
    if(minheap==NULL) return NULL;
    for(int i = 0;i<HUFFMAN_SYMBOLS;i++){
        if(freq_table[i]>0){
            HuffmanNode*node = create_node(freq_table[i],(unsigned char)i,1,NULL,NULL);
            if(node==NULL){
                goto fail;
            }

            if(!minheap_push(minheap,node)){
                free_tree(node);
                goto fail;
            }
        }
    }

    // no symbols
    if(minheap->size==0){
        minheap_destroy(minheap);
        return NULL;
    }

    // only one symbol
    if(minheap->size==1){
        HuffmanNode*root = minheap_pop(minheap);
        minheap_destroy(minheap);
        return root;
    }

    // build the Huffman Tree
    while(minheap->size>1){
        HuffmanNode*left = minheap_pop(minheap);
        HuffmanNode*right = minheap_pop(minheap);

        HuffmanNode*parent = create_node(left->freq+right->freq,0,0,left,right);

        if(parent==NULL){
            free_tree(left);
            free_tree(right);
            goto fail;
        }

        if(!minheap_push(minheap,parent)){
            free_tree(parent);
            goto fail;
        }
    }

    HuffmanNode*root = minheap_pop(minheap);
    minheap_destroy(minheap);
    return root;

fail:
    while(minheap->size>0){
        HuffmanNode*node = minheap_pop(minheap);
        free_tree(node);
    }

    minheap_destroy(minheap);
    return NULL;
}

static int build_codes_recursive(const HuffmanNode*node, uint32_t code, uint8_t len, HuffmanCode codes[HUFFMAN_SYMBOLS]){
    if(node==NULL) return 0;

    if(node->is_leaf){

        // case where input only contains one distinct symbol
        // so we reach here with len = 0 and code = 0
        // manually assign len = 1, code = 0
        // eg if AAAAA, then the code for A is 0 and has len 1
        if(len==0){
            len = 1;
            code = 0;
        }

        codes[node->symbol].len = len;
        codes[node->symbol].code = code;
        return 1;
    }

    // implementation supports Huffman Codes of maximum length 32 bits
    if (len >= 32) {
        fprintf(stderr, "Error: Huffman tree is too deep, cannot be stored in 32 bits\n");
        return 0;
    }

    // 0 when going left
    if(!build_codes_recursive(node->left,code<<1,len+1,codes)) return 0;
    // 1 when going right
    if(!build_codes_recursive(node->right,(code<<1)|1U,len+1,codes)) return 0;

    return 1;
}

// generate Huffman Code for every symbol from the Huffman Tree
int build_codes(const HuffmanNode*root, HuffmanCode codes[HUFFMAN_SYMBOLS]){
    if(root==NULL||codes==NULL){
        return 0;
    }
    return build_codes_recursive(root,0,0,codes);
}