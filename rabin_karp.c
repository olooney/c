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


uint64_t polynomial_hash(
    const char* string, 
    uint64_t base, 
    uint64_t prime_modulus
) {
    uint64_t hash = 0;
    const char* pc;
    for ( pc = string; *pc != '\0'; pc++ ) {
        uint64_t byte = (uint8_t)(*pc);
        hash = (hash * base) % prime_modulus;
        hash = (hash + byte) % prime_modulus;
    }
    return hash;
}

uint64_t polynomial_hash_roll(
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


void rabin_karp(const char* text, const char* pattern) {
    const char* pc; // pointer-to-character
    uint64_t base = 256;
    uint64_t prime_modulus = (1ULL << 31) -1;
    size_t text_length = strlen(text);

    // calculate the length and hash of the pattern
    uint64_t pattern_hash = polynomial_hash(pattern, base, prime_modulus);
    size_t pattern_length = strlen(pattern);

    // compute the inverse base
    uint64_t inverse_base = 1;
    uint64_t base2 = base;
    for (uint64_t exponent = pattern_length-1; exponent > 0; exponent >>= 1) {
        if ( exponent % 2 ) {
            inverse_base = (inverse_base * base2) % prime_modulus;
        }
        base2 = (base2 * base2) % prime_modulus;
    }

    // initialize the text hash to the same length as the pattern
    uint64_t text_hash = 0;
    for ( pc = text; pc < text+pattern_length; pc++ ) {
        uint64_t byte = (uint8_t)(*pc);
        text_hash = (text_hash * base + byte ) % prime_modulus;
    }

    // scan the text
    size_t line_number = 1;
    const char* line_start = text;
    size_t end_position = text_length - pattern_length + 1;
    for ( size_t i=0; i < end_position; i++ ) { 
        uint64_t left_byte = (uint8_t)text[i];
        uint64_t right_byte = (uint8_t)text[i + pattern_length];

        // report matches
        if ( text_hash == pattern_hash ) {
            if ( strncmp(text + i, pattern, pattern_length ) == 0 ) {
                printf("%zu:", line_number);
                print_line(line_start, 80);
                // TODO: handle case when pattern isn't in first 80.
            }
        }

        // update the rolling hash
        //
        // Note: this will compute the rolling hash of the '\0' at the end of
        // the string.  This is perfectly safe as the string is guarenteed to
        // have a null terminator and this hash value is never used. Doing so
        // allows us avoid an extra compare/branch inside the loop.
        uint64_t negative = (left_byte * inverse_base) % prime_modulus;
        text_hash = (text_hash + prime_modulus - negative) % prime_modulus;
        text_hash = (text_hash * base) % prime_modulus;
        text_hash = (text_hash + right_byte) % prime_modulus;

        // count lines
        if ( text[i + pattern_length] == '\n' ) {
            line_number++;
            line_start = text + i + pattern_length + 1;
        }
    }

}


