#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"


Token* tokenize(char** position) {
    char* start = *position;

    // skip leading whitespace
    while ( *start != '\0' && isspace(*start) ) start++;

    if ( *start == '\0' ) {
        *position = start;
        return NULL;
    }

    Token* token = (Token*)malloc(sizeof(Token));
    if ( token == NULL ) {
        perror("Out of memory: allocating Token\n");
        *position = start;
        return NULL;
    }

    token->token_type = KEYWORD_TOKEN;
    char* end = start + 1;
    while ( *end != '\0' && !isspace(*end) ) end++;

    size_t length = end - start;
    token->integer_value = (length > 0) ? 1 : 0;
    token->value = (char*)malloc(length+1);
    if ( token->value == NULL ) {
        perror("Out of memory: allocating Token value\n");
        *position = end;
        return NULL;
    }
    strncpy(token->value, start, length);
    token->value[length] = '\0';

    *position = end;
    return token;
}
