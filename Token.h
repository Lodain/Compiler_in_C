#ifndef TOKEN_H
#define TOKEN_H

// TokenType enum declaration
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

// Token structure declaration
typedef struct {
    TokenType type;
    char *value;
} Token;

#endif // TOKEN_H