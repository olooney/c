#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dict.h"

// implements the 64-bit Fowler-Noll-Vo hashing algorithm for strings.
uint64_t fnv_hash(const char* string) {
    const uint64_t fnv_prime = 1099511628211UL;
    uint64_t hash = 14695981039346656037UL; // FNV offset basis
    const char* c = string;
    while (*c) hash = (hash * fnv_prime)^(*c++);
    return hash;
}

bool dict_init(Dict* dict) {
    dict->size = 0;
    dict->table_size = 32;
    dict->keys = calloc(dict->table_size, sizeof(char*));
    if (!dict->keys ) return false;
    dict->values = calloc(dict->table_size, sizeof(uint64_t));
    if (!dict->values ) {
        free(dict->keys);
        return false;
    }
    // dict->growth_table = NULL;
    // dict->growth_progress = 0;
    return true;
}

Dict* new_dict() {
    Dict* dict = (Dict*) malloc(sizeof(Dict));
    dict_init(dict);
    return dict;
}

void free_dict(Dict* dict) {
    for ( size_t i=0; i < dict->table_size; i++ ) {
        free(dict->keys[i]);
    }
    free(dict->keys);
    free(dict->values);
    free(dict);
}

bool dict_set(Dict* dict, const char* key, uint64_t value) {
    const uint64_t mask = dict->table_size-1;
    uint64_t hashed_key = fnv_hash(key);
    uint64_t index = hashed_key & mask;
    while ( true ) { 
        const char* existing_key = dict->keys[index];
        if ( !existing_key ) {
            // add a brand new key
            dict->keys[index] = malloc(strlen(key) + 1);
            strcpy(dict->keys[index], key);
            break;
        } else if ( strcmp(key, existing_key) == 0 ) {
            // key exists; overwrite the corresponding value
            break;
        } else { 
            // a hash collision has occured. We'll use open addressing 
            // to find the next available slot.
            index = (index * 5 + 1) % dict->table_size;
        }
    // guarenteed to terminate if dict->size < dict->table_size
    }
    dict->size++;
    dict->values[index] = value;    

    if ( dict->size > dict->table_size/2 ) {
       // TODO begin growing table. 
    }

   return true;
}

bool dict_get(Dict* dict, const char* key, uint64_t* value) {
    const uint64_t mask = dict->table_size-1;
    uint64_t hashed_key = fnv_hash(key);
    uint64_t index = hashed_key & mask;
    while ( true ) {
        const char* existing_key = dict->keys[index];
        if ( !existing_key ) {
            return false;
        } else if ( strcmp(key, existing_key) == 0 ) {
            *value = dict->values[index];
            return true;
        } else {
            index = (index * 5 + 1) % dict->table_size;
        }
    }
    return false;
}

