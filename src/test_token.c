#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

void test_tokenize() {
    char input[] = "one two three";
    char* position = input;
    while ( true ) { 
        Token* token = tokenize(&position);
        if ( token == NULL || token->value == NULL ) {
            break;
        }
        printf("token: '%s'\n", token->value);
        free(token);
    }
}

int main(int argc, char** argv) {
    test_tokenize();
    return 0;
}
