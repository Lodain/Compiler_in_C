#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runExample(const char* input) {
    printf("Input: %s\n", input);
    Token* tokens = scan((char*)input);
    char* result = parse(tokens);
    printf("Output: %s\n\n", result);

    // Free allocated memory
    free(result);
    for (int i = 0; tokens[i].type != END; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}

int main() {
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open test.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';
        runExample(line);
    }

    fclose(file);
    return 0;
}