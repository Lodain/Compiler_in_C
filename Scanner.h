#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"

// Function declarations from Scanner.c
Token* scan(char* code);
char* removeWhitespaceAndComments(char* source);

#endif // SCANNER_H