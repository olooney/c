#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <stdbool.h>

void rabin_karp(
    const char* text, 
    const char* pattern
);

void rabin_karp_many(
    const char* text, 
    char** patterns, 
    size_t n_patterns, 
    bool use_hash
);

#endif // RABIN_KARP_H
