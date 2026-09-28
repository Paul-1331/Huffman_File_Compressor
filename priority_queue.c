#include "priority_queue.h"
#include <stdlib.h>


MinHeap*minheap_create(void){
    MinHeap*heap = malloc(sizeof(MinHeap));
    if(heap==NULL) return NULL;
    heap->size = 0;
    return heap;
}

static void heapify_down(MinHeap*heap,size_t index){
    while(1){
        size_t left = 2*index+1;
        size_t right = 2*index+2;

        size_t smallest = index;

        if(left<heap->size&&heap->data[left]->freq<heap->data[smallest]->freq){
            smallest = left;
        }
        if(right<heap->size&&heap->data[right]->freq<heap->data[smallest]->freq){
            smallest = right;
        }

        if(smallest==index) break;

        HuffmanNode*temp = heap->data[smallest];
        heap->data[smallest] = heap->data[index];
        heap->data[index] = temp;

        index = smallest;
    }
}

static void heapify_up(MinHeap*heap,size_t index){
    while(index>0){
        size_t parent = (index-1)/2;

        if(heap->data[parent]->freq<=heap->data[index]->freq){
            break;
        }

        HuffmanNode*temp = heap->data[parent];
        heap->data[parent] = heap->data[index];
        heap->data[index] = temp;

        index = parent;
    }
}

int minheap_push(MinHeap*heap,HuffmanNode*node){
    if(heap==NULL||node==NULL) return 0;
    if(heap->size>=HEAP_CAPACITY){
        return 0; // heap full. This will not happen since only 256 symbols are possible and 512 array size is more than enough to hold that
    }
    size_t index = heap->size;
    heap->data[index] = node;
    (heap->size)++;
    heapify_up(heap,index);
    return 1;
}

HuffmanNode*minheap_pop(MinHeap*heap){
    if(heap==NULL||heap->size==0){
        return NULL;
    }

    HuffmanNode*min = heap->data[0];

    (heap->size)--;

    if(heap->size>0){
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap,0);
    }
    return min;
}

void minheap_destroy(MinHeap*heap){
    free(heap);
}