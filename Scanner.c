#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//this is the token type enum
typedef enum {
    ADD,    //0
    SUB,    //1
    MUL,    //2
    DIV,    //3
    MOD,    //4
    POW,    //5
    TERN,   //6
    COMMA,  //7
    LPAREN, //8
    RPAREN, //9
    NUMBER, //10
    END,    //11
    ERROR   //12
} TokenType;

//this is the token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

//this function is used to remove the whitespace and comments from the source code
char* removeWhitespaceAndComments(char* source) {
    int inComment = 0;
    int index = 0;
    int length = strlen(source);
    char* result = (char*)malloc(length + 1); 

    for (int i = 0; i < length; i++) {
        if (!inComment && i + 1 < length && source[i] == '/' && source[i + 1] == '*') {
            inComment = 1; 
            i++; 
        } 
        else if (inComment && i + 1 < length && source[i] == '*' && source[i + 1] == '/') {
            inComment = 0; 
            i++; 
        } 
        else if (!inComment && !isspace((unsigned char)source[i])) {
            result[index++] = source[i];
        }
    }

    result[index] = '\0'; 
    return result;
}

//to create this function I used some FA
//I created a total of 7 FA's, (10 if we count the parentheses and comma)
//I implemented the FA using the PDF 5 (implementation of DFA) as reference
Token* scan(char* code){
    int flag; //flag to check if the input is valid
    int errorFlag=0; //flag to check if there is an error
    int state; //state of the FA
    Token* tokens=NULL; //array of tokens
    int tokenNumber=0; //number of tokens
    char* sourceCode=removeWhitespaceAndComments(code); //source code without whitespace and comments
    char* first=sourceCode; //first character of the source code
    char* start=sourceCode; //start of the source code
    while (*sourceCode != '\0' && !errorFlag) {
        state=0; //initial state of the FA
        flag=1;

        if ((*sourceCode=='+' || *sourceCode=='-') || (*sourceCode >= '0' && *sourceCode <= '9')) { //the input is a number
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
        else if (*sourceCode=='a'){ //the input can be an operator
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
                tokens[tokenNumber-1].type=ADD;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='+';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='s'){ //the input can be an operator
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
                tokens[tokenNumber-1].type=SUB;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='-';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='d'){ //the input can be an operator
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
                tokens[tokenNumber-1].type=DIV;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='/';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='p'){ //the input can be an operator
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
                tokens[tokenNumber-1].type=POW;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='^';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='t'){ //the input can be an operator
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
                tokens[tokenNumber-1].type=TERN;
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                tokens[tokenNumber-1].value[0]='?';
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode=='m'){ //the input can be an operator
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
                
                tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
                if (state==5){
                    tokens[tokenNumber-1].type=MOD;
                    tokens[tokenNumber-1].value[0]='%';
                }
                else{
                    tokens[tokenNumber-1].type=MUL;
                    tokens[tokenNumber-1].value[0]='*';
                }
                tokens[tokenNumber-1].value[1]='\0';
                start=sourceCode;
            }
        }
        else if (*sourceCode==','){ //the input is a comma
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=COMMA;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]=',';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
        else if (*sourceCode=='('){ //the input is a left parenthesis
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=LPAREN;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]='(';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
        else if (*sourceCode==')'){ //the input is a right parenthesis
            sourceCode++;
            tokenNumber++;
            tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
            tokens[tokenNumber-1].type=RPAREN;
            tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
            tokens[tokenNumber-1].value[0]=')';
            tokens[tokenNumber-1].value[1]='\0';
            start=sourceCode;
        }
        else{   //the input is invalid
            errorFlag=1;
        }
    }
    if (!errorFlag){    //if the program reached this point and errorFlag is false, then the input is valid
        tokenNumber++;
        tokens=(Token*)realloc(tokens,tokenNumber*sizeof(Token));
        tokens[tokenNumber-1].type=END;
        tokens[tokenNumber-1].value=(char*)malloc(2*sizeof(char));
        tokens[tokenNumber-1].value[0]='$';
        tokens[tokenNumber-1].value[1]='\0';
    }
    else {    //if the program reached this point and errorFlag is true, then the input is invalid
        free(tokens);
        tokens=(Token*)malloc(sizeof(Token));
        tokens[0].type=ERROR;
        tokens[0].value=(char*)malloc(50*sizeof(char));
        sprintf(tokens[0].value, "Lexical error! Position %d", (sourceCode-first)+1);
    }
    free(first);
    return tokens;
}