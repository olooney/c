#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    KEYWORD_TOKEN,
    STRING_TOKEN,
    INTEGER_TOKEN,
} TokenType;


typedef struct {
    TokenType token_type;
    char* value;
    long integer_value;
} Token;

// reads and returns a single new Token.
// returns NULL if no token can be read.
// updates the value of the position pointer
// to after the end of the last token returned.
Token* tokenize(char** position);

#endif // TOKEN_H
