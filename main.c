#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char code[] = "add(5, mul(3, sub(10, pow(6, 4))))";
    Token* tokens; 
    char* result;
    tokens = scan(code);
    result = parse(tokens);

    printf("Parse result: %s\n", result);

    // Free allocated memory
    free(result);
    for (int i = 0; tokens[i].type != END; i++) {
        free(tokens[i].value);
    }
    free(tokens);

    return 0;
}