#ifndef SKEW_HEAP_H
#define SKEW_HEAP_H

#include <stdbool.h>

typedef struct Node Node;
struct Node;

typedef struct SkewHeap SkewHeap;
struct SkewHeap {
    Node* root;
};

void init_skew_heap(SkewHeap* heap);
SkewHeap* new_skew_heap();
void free_skew_heap(SkewHeap* heap);

int skew_heap_push(SkewHeap* heap, const char* value);
int skew_heap_pop(SkewHeap* heap, char* out);
const char* skew_heap_peek(SkewHeap* heap);
size_t skew_heap_size(SkewHeap* heap);
size_t skew_heap_depth(SkewHeap* heap);
bool skew_heap_empty(SkewHeap* heap);


#endif // SKEW_HEAP_H
