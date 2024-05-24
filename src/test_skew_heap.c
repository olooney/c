#include <stdio.h>
#include "skew_heap.h"

int main(int argc, char** argv) {
    SkewHeap* heap = new_skew_heap();

    const char* abc = "yubgrlieamdncwftpxsjhovzqk";

    char letter[2];
    letter[1] = '\0';
    for ( int i = 0; abc[i] != '\0'; i++ ) {
        letter[0] = abc[i];
        skew_heap_push(heap, letter);
    }

    while ( !skew_heap_empty(heap) ) {
        char buffer[1024];
        skew_heap_pop(heap, buffer);
        printf("%s", buffer);
    }
    printf("\n");

    free_skew_heap(heap);
    return 0;
}
