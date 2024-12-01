#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OPERATORS 13

// Define operators
enum Operators {
    ADD, SUB, MUL, DIV, MOD, POW, TERN, COMMA, LPAREN, RPAREN, I, DOLLAR
};

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_COMMA,
    TOKEN_END
} TokenType;

// Define a token structure
typedef struct {
    TokenType type;
    char value[32];
} Token;

typedef struct treeNode{
    int data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;
typedef TreeNode *TreeNodePtr;

typedef struct {
    char left;
    char right[10];
}Rule;

// Precedence table
static char precedenceTable[NUM_OPERATORS][NUM_OPERATORS] = {
    // ADD  SUB  MUL  DIV  MOD  POW  TERN COMMA LPAREN RPAREN  I   DOLLAR
    { '>', '>', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>' }, // ADD
    { '>', '>', '<', '<', '<', '<', '<', '>', '<', '>', '<', '>' }, // SUB
    { '>', '>', '>', '>', '>', '<', '<', '>', '<', '>', '<', '>' }, // MUL
    { '>', '>', '>', '>', '>', '<', '<', '>', '<', '>', '<', '>' }, // DIV
    { '>', '>', '>', '>', '>', '<', '<', '>', '<', '>', '<', '>' }, // MOD
    { '>', '>', '>', '>', '>', '>', '<', '>', '<', '>', '<', '>' }, // POW
    { '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>' }, // TERN
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', '<', '>' }, // COMMA
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', ' ' }, // LPAREN
    { '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', ' ', '>' }, // RPAREN
    { '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', ' ', '>' }, // I
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', ' ' }  // DOLLAR
};

static Rule rules[10] = {
    {'S', "i"},         //1: S -> i
    {'S', "add A"},     //2: S -> add A
    {'S', "sub A"},     //3: S -> sub A
    {'S', "mul A"},     //4: S -> mul A
    {'S', "div A"},     //5: S -> div A
    {'S', "mod A"},     //6: S -> mod A
    {'S', "pow A"},     //7: S -> pow A
    {'S', "tern B"},    //8: S -> tern B
    {'A', "(S,S)"},     //9: A -> (S,S)
    {'B', "(S,S,S)"},   //10: B -> (S,S,S)
};

char* parse(Token* tokens, int numTokens) {

}
