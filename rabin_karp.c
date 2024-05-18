#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rabin_karp.h"

void rabin_karp(const char* text, const char* pattern) {
    const char* pc; // pointer-to-character
    uint64_t base = 256;
    // uint64_t prime_modulus = (1ULL << 29) -1;
    uint64_t prime_modulus = (1ULL << 11) -1;
    size_t text_length = strlen(text);

    // calculate the length and hash of the pattern
    uint64_t pattern_hash = 0;
    for ( pc = pattern; *pc != '\0'; pc++ ) {
        pattern_hash = (pattern_hash * base + *pc ) % prime_modulus;
    }
    size_t pattern_length = pc - pattern;

    // compute the inverse power
    uint64_t inverse_power = 1;
    uint64_t base2 = base;
    for (uint64_t exponent = pattern_length-1; exponent > 0; exponent >>= 1) {
        if ( exponent % 2 ) {
            inverse_power = (inverse_power * base2) % prime_modulus;
        }
        base2 = (base2 * base2) % prime_modulus;
    }

    // initialize the text hash to the same length as the pattern
    uint64_t text_hash = 0;
    for ( pc = text; pc < text+pattern_length; pc++ ) {
        text_hash = (text_hash * base + *pc ) % prime_modulus;
    }

    // scan the text
    size_t line_number = 1;
    const char* line_start = text;
    size_t end_position = text_length - pattern_length + 1;
    for ( size_t i=0; i < end_position; i++ ) { 
        // report matches
        if ( text_hash == pattern_hash ) {
            if ( strncmp(text + i, pattern, pattern_length ) == 0 ) {
                // printf("match \"%s\" found on line %zu at position %zu (%lu == %lu)\n", pattern, line_number, i, text_hash, pattern_hash);
                if ( text + pattern_length + i - line_start < 80 ) {
                    printf("%.80s\n", line_start);
                } else {
                    const char* chunk_start = text + i - 40 - pattern_length;
                    if ( chunk_start < text ) chunk_start = text;
                    printf("%.80s\n", chunk_start);
                }
            }
        }

        // update the rolling hash
        uint64_t negative = (text[i] * inverse_power) % prime_modulus;
        text_hash = (text_hash + prime_modulus - negative) % prime_modulus;
        text_hash = (text_hash * base + text[i+pattern_length]) % prime_modulus;

        // Note: this will compute the rolling hash of the '\0' at the end of
        // the string.  This is perfectly safe as the string is guarenteed to
        // have a null terminator and this hash value is never used. Doing so
        // allows us avoid an extra compare/branch inside the loop.
        //
        if ( text[i + pattern_length] == '\n' ) {
            line_number++;
            line_start = text + i + pattern_length + 1;
        }
    }

}


