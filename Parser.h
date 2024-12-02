#ifndef PARSER_H
#define PARSER_H

#include "Token.h"

// TreeNode structure declaration
typedef struct treeNode {
    char* data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;
typedef TreeNode *TreeNodePtr;

// Function declaration from Parser.c
char* parse(Token* tokens);

#endif // PARSER_H