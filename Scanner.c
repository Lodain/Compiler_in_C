//input: Source code
//output: String of Tokens
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NUMBER,
    OPERATOR,
    SYMBOL
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token* scan(char* sourceCode){
    Token* tokens;
    int tokenNumber=0;
    char* start=sourceCode;
    int flag=1;
    int digitFlag=0;    //i use this flag to check if the last character is a digit
    int eFlag=0;        //i use this flag to check if the last character is e
    int dotFlag=0;      //i use this flag to check if the last character is a dot
    int signFlag=0;    //i use this flag to check if the last character is a sign
    while (*sourceCode != '\0') {
        if ((*sourceCode=='+' || *sourceCode=='-') || *sourceCode >= '0' && *sourceCode <= '9') {
            while (flag)
            {
                if (*sourceCode >= '0' && *sourceCode <= '9') {
                    sourceCode++;
                    digitFlag=1;
                    eFlag=0;
                    dotFlag=0;
                    signFlag=0;
                }
                else if (*sourceCode == '.') {
                    if (digitFlag) {
                        sourceCode++;
                        dotFlag=1;
                        eFlag=0;
                        digitFlag=0;
                        signFlag=0;
                    }
                    else{
                        flag=0;
                    }
                }
                else if (*sourceCode == 'e') {
                    if (digitFlag) {
                        sourceCode++;
                        eFlag=1;
                        dotFlag=0;
                        digitFlag=0;
                        signFlag=0;
                    }
                    else{
                        flag=0;
                    }
                }
                else if (*sourceCode == '+' || *sourceCode == '-') {
                    if (signFlag || dotFlag || digitFlag) {
                        flag=0;
                    }
                    else{
                        sourceCode++;
                        signFlag=1;
                        dotFlag=0;
                        digitFlag=0;
                        eFlag=0;
                    }
                }
                else{
                    flag=0;
                }
            }
            signFlag=0;
            dotFlag=0;
            digitFlag=0;
            eFlag=0;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=NUMBER;
            tokens[tokenNumber-1].value=(char*)malloc((sourceCode-start+1)*sizeof(char));
            strncpy(tokens[tokenNumber-1].value,start,sourceCode-start);
            start=sourceCode;
        }
    }

}