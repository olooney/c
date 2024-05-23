#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// implements the 64-bit Fowler-Noll-Vo hashing algorithm for strings.
uint64_t fnv_hash(const char* string);

typedef struct {
    uint64_t size;                // current number of members
    size_t table_size;            // size of the table
    char** keys;                   // array of keys index by hash.
    uint64_t* values;             // array of values
} Dict;

Dict* new_dict();
void free_dict(Dict* t);

bool dict_set(Dict* t, const char* key, uint64_t value);
bool dict_get(Dict* t, const char* key, uint64_t* value);

#endif // DICT_H
