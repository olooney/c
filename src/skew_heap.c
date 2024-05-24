#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "skew_heap.h"

typedef struct Node Node;
struct Node {
    char* value;
    Node* left;
    Node* right;
};

Node* new_node(const char* value) {
    size_t length = strlen(value);
    char* new_value = (char*)malloc(length + 1);
    strcpy(new_value, value);
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = new_value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_node(Node* node) {
    if ( node == NULL ) return;
    free(node->value);
    free_node(node->left);
    free_node(node->right);
}

size_t node_count(Node* node) {
    if ( node == NULL ) {
        return 0;
    }
    return 1 + 
        node_count(node->left) + 
        node_count(node->right);
}

size_t node_depth(Node* node) {
    if ( node == NULL ) {
        return 0;
    }
    size_t left_depth = node_depth(node->left);
    size_t right_depth = node_depth(node->right);

    if ( left_depth > right_depth ) {
        return left_depth + 1;
    } else {
        return right_depth + 1;
    }
}

Node* merge(Node* left, Node* right) {
    if ( left == NULL ) return right;
    if ( right == NULL ) return left;

    Node* less;
    Node* more;
    if ( strcmp(left->value, right->value) < 0 ) {
        less = left;
        more = right;
    } else {
        less = right;
        more = left;
    }

    // TODO: avoid unnecessary malloc/free
    Node* node = new_node(less->value);
    node->left = merge(less->right, more);
    node->right = less->left;

    free(less->value);
    free(less);

    return node;
}

void init_skew_heap(SkewHeap* heap) {
    heap->root = NULL;
}

SkewHeap* new_skew_heap() {
    SkewHeap* heap = malloc(sizeof(heap));
    if ( heap ) init_skew_heap(heap);
    return heap;
}

void free_skew_heap(SkewHeap* heap) {
    if ( heap == NULL ) return;
    free_node(heap->root);
    free(heap);
}

int skew_heap_push(SkewHeap* heap, const char* value) {
    if ( heap == NULL ) return 1;
    if ( value == NULL ) return 2;

    Node* node = new_node(value);
    heap->root = merge(heap->root, node);

    return 0;
}

int skew_heap_pop(SkewHeap* heap, char* out) {
    if ( heap == NULL ) return 1;
    if ( heap->root == NULL ) return 3;

    strcpy(out, heap->root->value);
    Node* old_root = heap->root;
    heap->root = merge(heap->root->left, heap->root->right);

    free(old_root->value);
    free(old_root);
    
    return 0;
}

const char* skew_heap_peek(SkewHeap* heap) {
    if ( heap == NULL ) return NULL;
    if ( heap->root == NULL ) return NULL;

    return heap->root->value;
}

// TODO: Union?

size_t skew_heap_size(SkewHeap* heap) { 
    if ( heap == NULL ) return 0;
    return node_count(heap->root);
}

size_t skew_heap_depth(SkewHeap* heap) { 
    if ( heap == NULL ) return 0;
    return node_depth(heap->root);
}

bool skew_heap_empty(SkewHeap* heap) {
    if ( heap == NULL ) return true;
    return (heap->root == NULL);
}
