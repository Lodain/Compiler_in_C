//input: Source code
//output: String of Tokens
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    NUMBER,
    OPERATOR,
    COMMA,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    END,
    ERROR
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

char* removeWhitespaceAndComments(char* source);

Token* scan(char* code){
    int flag=1;
    int errorFlag=0;
    int state;
    Token* tokens=NULL;
    int tokenNumber=0;
    char* sourceCode=removeWhitespaceAndComments(code);
    char* start=sourceCode;
    while (*sourceCode != '\0' && !errorFlag) {
        state=0;
        flag=1;
        if ((*sourceCode=='+' || *sourceCode=='-') || (*sourceCode >= '0' && *sourceCode <= '9')) {
            while (flag) {
                if (*sourceCode >= '0' && *sourceCode <= '9') {
                    switch (state) {
                        case 0:
                        case 1:
                        case 2:
                            state=2;
                            sourceCode++;
                            break;
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            state=5;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode == '+' || *sourceCode == '-') {
                    switch (state) {
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        case 4:
                            state=6;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if(*sourceCode == '.') {
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode == 'e') {
                    switch (state) {
                        case 2:
                            state=4;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=2 && state!=5)   // I am not in a final state
                        errorFlag=1;   
                }
                
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=NUMBER;
                tokens[tokenNumber-1].value=(char*)malloc((sourceCode-start+1)*sizeof(char));
                strncpy(tokens[tokenNumber-1].value,start,sourceCode-start);
                tokens[tokenNumber-1].value[sourceCode-start]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='a'){
            while (flag){
                if (*sourceCode=='a'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='d'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=3)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='+';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='s'){
            while (flag){
                if (*sourceCode=='s'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='u'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='b'){
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=3)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='-';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='d'){
            while (flag){
                if (*sourceCode=='d'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='i'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='v'){
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=3)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='/';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='p'){
            while (flag){
                if (*sourceCode=='p'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='o'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='w'){
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=3)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='^';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='t'){
            while (flag){
                if (*sourceCode=='t'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='e'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='r'){
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='n'){
                    switch (state){
                        case 3:
                            state=4;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=4)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='?';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='m'){
            while (flag){
                if (*sourceCode=='m'){
                    switch (state){
                        case 0:
                            state=1;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='u'){
                    switch (state){
                        case 1:
                            state=2;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if(*sourceCode=='l'){
                    switch (state){
                        case 2:
                            state=3;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='o'){
                    switch (state){
                        case 1:
                            state=4;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else if (*sourceCode=='d'){
                    switch (state){
                        case 4:
                            state=5;
                            sourceCode++;
                            break;
                        default:
                            flag=0;
                            errorFlag=1;
                            break;
                    }
                }
                else {
                    flag=0;
                    if (state!=5 && state!=3)   // I am not in a final state
                        errorFlag=1;   
                }
            }
            if (!errorFlag){
                tokenNumber++;
                tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
                tokens[tokenNumber-1].type=OPERATOR;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                if (state==5)
                    tokens[tokenNumber-1].value[0]='%';
                else
                    tokens[tokenNumber-1].value[0]='*';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode==','){
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=COMMA;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]=',';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
        else if (*sourceCode=='('){
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=LEFT_PARENTHESIS;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]='(';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
        else if (*sourceCode==')'){
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=RIGHT_PARENTHESIS;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]=')';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
    }
    if (!errorFlag){
        tokenNumber++;
        tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
        tokens[tokenNumber-1].type=END;
        tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
        tokens[tokenNumber-1].value[0]='$';
        tokens[tokenNumber-1].value[1]='\0';
    }
    else {
        free(tokens);
        tokens=(Token*)malloc(2*sizeof(Token));
        tokens[0].type=ERROR;
        tokens[0].value=(char*)malloc(2*sizeof(char));
        tokens[0].value[0]='E';
        tokens[0].value[1]='\0';
        tokens[1].type=END;
        tokens[1].value=(char*)malloc(2*sizeof(char));
        tokens[1].value[0]='$';
        tokens[1].value[1]='\0';
    }
    return tokens;
}

char* removeWhitespaceAndComments(char* source) {
    int inComment = 0;
    int index = 0;
    int length = strlen(source);
    char* result = (char*)malloc(length + 1); // Allocate memory for the result

    for (int i = 0; i < length; i++) {
        if (!inComment && i + 1 < length && source[i] == '/' && source[i + 1] == '*') {
            inComment = 1; // Entering a comment block
            i++; // Skip the '*' character
        } else if (inComment && i + 1 < length && source[i] == '*' && source[i + 1] == '/') {
            inComment = 0; // Exiting a comment block
            i++; // Skip the '/' character
        } else if (!inComment && !isspace((unsigned char)source[i])) {
            result[index++] = source[i]; // Copy non-whitespace, non-comment characters
        }
    }

    result[index] = '\0'; // Null-terminate the result string
    return result;
}

void main(){
    char string[50]="add(5, mul(3, sub(10, pow(6, 4))))";
    Token* tokens=scan(string);
    for (int i=0;tokens[i].type!=END;i++){
        printf("%s  ", tokens[i].value );
    }
}
