#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runExample(const char* input) {
    printf("Input: %s\n", input);
    Token* tokens = scan((char*)input);
    if (tokens->type==ERROR){
        printf("Error: %s\n\n", tokens->value);
        return;
    }
    char* result = parse(tokens);
    printf("Output: %s\n\n", result);

    // Free allocated memory
    free(result);
    for (int i = 0; tokens[i].type != END; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}

int main(int argc, char *argv[]) {
    if (argc >1){
        for (int i = 1; i < argc; i++){
            runExample(argv[i]);
        }
    }
    else{
        FILE *file = fopen("test2.txt", "r");
        if (file == NULL) {
            fprintf(stderr, "Could not open test.txt\n");
            return 1;
        }   

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            runExample(line);
        }
        fclose(file);
    }
    return 0;
}