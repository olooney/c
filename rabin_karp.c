#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rabin_karp.h"

void print_line(const char *start, size_t n) {
    for (size_t i = 0; i < n && start[i] != '\0' && start[i] != '\n'; i++) {
        putchar(start[i]);
    }
    putchar('\n');
}


static inline uint64_t poly_hash(
    const char* string, 
    uint64_t base, 
    uint64_t prime_modulus,
    size_t length // must be correct!
) {
    uint64_t hash = 0;
    for ( size_t i=0; i<length; i++ ) {
        uint64_t byte = (uint8_t)(string[i]);
        hash = (hash * base) % prime_modulus;
        hash = (hash + byte) % prime_modulus;
    }
    return hash;
}


static inline uint64_t poly_hash_roll(
    uint64_t hash, 
    uint64_t base, 
    uint64_t inverse_base, 
    uint64_t prime_modulus,
    char left_char,
    char right_char
) { 
        // convert chars to byte 0-255, then widen to 64 bits.
        uint64_t left_byte = (uint8_t)left_char;
        uint64_t right_byte = (uint8_t)right_char;

        // subtract left
        uint64_t negative = (left_byte * inverse_base) % prime_modulus;
        hash = (hash + prime_modulus - negative) % prime_modulus;

        // add right
        hash = (hash * base) % prime_modulus;
        hash = (hash + right_byte) % prime_modulus;

        return hash;
}


// computes base^(length-1), the inverse base needed to
// subtract off the left base from the rolling polynomial hash.
static inline uint64_t poly_invert_base(
    uint64_t base, 
    uint64_t prime_modulus, 
    size_t length
) {
    uint64_t inverse_base = 1;
    uint64_t base2 = base;
    uint64_t exponent;

    // exponentiation by squaring (modulo a prime)
    for ( exponent = length-1; exponent > 0; exponent >>= 1) {
        if ( exponent % 2 ) {
            inverse_base = (inverse_base * base2) % prime_modulus;
        }
        base2 = (base2 * base2) % prime_modulus;
    }

    return inverse_base;
}


void rabin_karp(const char* text, const char* pattern) {
    uint64_t base = 256;
    uint64_t prime_modulus = (1ULL << 31) -1;
    size_t text_length = strlen(text);

    // calculate the length and hash of the pattern
    size_t pattern_length = strlen(pattern);
    uint64_t pattern_hash = poly_hash(pattern, base, prime_modulus, pattern_length);

    // compute the inverse base
    uint64_t inverse_base = poly_invert_base(base, prime_modulus, pattern_length);

    // initialize the text hash to the same length as the pattern
    uint64_t text_hash = poly_hash(text, base, prime_modulus, pattern_length);

    // scan the text
    size_t line_number = 1;
    const char* line_start = text;
    size_t end_position = text_length - pattern_length + 1;
    for ( size_t i=0; i < end_position; i++ ) { 

        // report matches
        if ( text_hash == pattern_hash ) {
            if ( strncmp(text + i, pattern, pattern_length ) == 0 ) {
                printf("%zu:", line_number);
                print_line(line_start, 80);
            }
        }

        // slide the hash window over one
        text_hash = poly_hash_roll(
            text_hash,
            base,
            inverse_base,
            prime_modulus,
            text[i],
            text[i + pattern_length]
        );

        // count lines
        if ( text[i + pattern_length] == '\n' ) {
            line_number++;
            line_start = text + i + pattern_length + 1;
        }
    }
}


