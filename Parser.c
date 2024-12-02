#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OPERATORS 13

typedef enum {
    ADD=0,  //0
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
    ERROR,  //12
    P,      //13    <
    A,      //14
    B,      //15
    S,      //16
} TokenType;

//this is the token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

//declaration of the tree node structure
typedef struct treeNode{
    char* data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;
typedef TreeNode *TreeNodePtr;

//declaration of the stack for the abstract syntax tree
typedef struct stackTree{
    TreeNodePtr value;
    struct stackTree* next;
} StackTree;
typedef StackTree* StackTreePtr;

//declaration of the rule structure
typedef struct {
    Token left;
    int right[10];
}Rule;

//declaration of the stack for the parsing
typedef struct stackNode{
    Token value;
    struct stackNode* next;
} StackNode;
typedef StackNode* StackPtr;

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

//rules of the CFG
static Rule rules[10] = {
    {{S, NULL}, {NUMBER, -1}},                                     //0: S -> i
    {{S, NULL}, {ADD, A, -1}},                                     //1: S -> add A
    {{S, NULL}, {SUB, A, -1}},                                     //2: S -> sub A
    {{S, NULL}, {MUL, A, -1}},                                     //3: S -> mul A
    {{S, NULL}, {DIV, A, -1}},                                     //4: S -> div A
    {{S, NULL}, {MOD, A, -1}},                                     //5: S -> mod A
    {{S, NULL}, {POW, A, -1}},                                     //6: S -> pow A
    {{S, NULL}, {TERN, B, -1}},                                    //7: S -> tern B
    {{A, NULL}, {LPAREN, S, COMMA, S, RPAREN, -1}},                //8: A -> (S,S)
    {{B, NULL}, {LPAREN, S, COMMA, S, COMMA, S, RPAREN, -1}},      //9: B -> (S,S,S)
};

// functions for the structures used in the parsing:

void push(StackPtr* top, Token value){
    StackPtr newNode = (StackPtr)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->next = *top;
    *top = newNode;
}

Token pop(StackPtr* top){
    if (*top == NULL) {
        fprintf(stderr, "Stack is empty!\n");
        exit(EXIT_FAILURE); 
    }
    Token value = (*top)->value;
    StackPtr temp = *top;
    *top = (*top)->next;
    free(temp);
    return value;
}

void pushTree(StackTreePtr* top, TreeNodePtr value) {
    StackTreePtr newNode = (StackTreePtr)malloc(sizeof(StackTree));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->value = value;
    newNode->next = *top;
    *top = newNode;
}

TreeNodePtr popTree(StackTreePtr* top) {
    if (*top == NULL) {
        fprintf(stderr, "Stack is empty!\n");
        exit(EXIT_FAILURE);
    }
    TreeNodePtr value = (*top)->value;
    StackTreePtr temp = *top;
    *top = (*top)->next;
    free(temp);
    return value;
}

//this function prints the stack for debugging purposes
//now is not used
void printStack(StackPtr stack) {
    if (stack == NULL) {
        printf("Stack is empty\n");
        return;
    }
    
    StackPtr current = stack;
    printf("Stack (top to bottom): ");
    while (current != NULL) {
        switch(current->value.type){
            case ADD: printf("ADD "); break;
            case SUB: printf("SUB "); break;
            case MUL: printf("MUL "); break;
            case DIV: printf("DIV "); break;
            case MOD: printf("MOD "); break;
            case POW: printf("POW "); break;
            case TERN: printf("TERN "); break;
            case COMMA: printf("COMMA "); break;
            case LPAREN: printf("( "); break;
            case RPAREN: printf(") "); break;
            case NUMBER: printf("i "); break;
            case END: printf("END "); break;
            case ERROR: printf("ERROR "); break;
            case P: printf("P "); break;
            case A: printf("A "); break;
            case B: printf("B "); break;
            case S: printf("S "); break;
        }
        current = current->next;
    }
}

// semantic action corresponding at rule S -> i (i is a number)
void semanticAction1(StackTreePtr* stackTree, Token token){      
    TreeNodePtr treeNode = (TreeNodePtr)malloc(sizeof(TreeNode));
    treeNode->data = token.value;
    treeNode->left = NULL;
    treeNode->right = NULL;
    pushTree(stackTree, treeNode);
}

// semantic action corresponding at rule A -> (S,S)
void semanticAction2(StackTreePtr* stackTree){
    TreeNodePtr treeNode = (TreeNodePtr)malloc(sizeof(TreeNode));
    treeNode->data = NULL;
    treeNode->right = popTree(stackTree);
    treeNode->left = popTree(stackTree);
    pushTree(stackTree, treeNode);
}

// semantic action corresponding at rule B -> (S,S,S)
void semanticAction3(StackTreePtr* stackTree){
    TreeNodePtr treeNode = (TreeNodePtr)malloc(sizeof(TreeNode));
    TreeNodePtr treeNode2 = (TreeNodePtr)malloc(sizeof(TreeNode));
    treeNode->data = NULL;
    treeNode2->data = (char*)malloc(2*sizeof(char));
    treeNode2->data[0] = ':';
    treeNode2->data[1] = '\0';
    treeNode2->right = popTree(stackTree);
    treeNode2->left = popTree(stackTree);
    treeNode->right = treeNode2;
    treeNode->left = popTree(stackTree);
    pushTree(stackTree, treeNode);
}

// semantic action corresponding at rule S -> add A, sub A, mul A, div A, mod A, pow A, tern B
void semanticAction4(StackTreePtr* stackTree, Token token){
    (*stackTree)->value->data = token.value;
}

//this function prints the abstract syntax tree for debugging purposes
//now is not used
void printTree(TreeNodePtr tree){
    if (tree == NULL) return;
    printTree(tree->left);
    printf("%s ", tree->data);
    printTree(tree->right);
}

//this function returns the precedence of the operator
//I used it to insert the parentheses in the output string when needed
int getPrecedence(const char* op) {
    if (op == NULL) return 0;
    if (op[0] == ':') return 1;
    if (op[0] == '+' || op[0] == '-') return 2;
    if (op[0] == '*' || op[0] == '/' || op[0] == '%') return 3;
    if (op[0] == '^') return 4;
    return 0;
}

//this function creates the output string using the abstract syntax tree, it also inserts the parentheses when needed
char* finalOutput(TreeNodePtr tree, char* output) {
    if (tree == NULL) return output;

    if (output == NULL) {
        output = (char*)malloc(256 * sizeof(char)); 
        output[0] = '\0';
    }

    // Here I check if the parentheses are needed based on the precedence of the operators
    int needParens = 0;
    if (tree->left && getPrecedence(tree->data) && getPrecedence(tree->left->data)) {
        if (getPrecedence(tree->data) > getPrecedence(tree->left->data)) {
            needParens = 1;
        }
        else if (getPrecedence(tree->data) == 4 && getPrecedence(tree->left->data) == 4){
            needParens = 1;
        }
    }

    if (needParens) strcat(output, "( ");

    //left subtree
    output = finalOutput(tree->left, output);

    if (needParens){
        strcat(output, ") ");
        needParens = 0;
    }

    // Append current node's data
    strcat(output, tree->data);
    strcat(output, " "); // I add a space to separate the operators

    // same as before but for the right subtree
    if (tree->right && getPrecedence(tree->data) && getPrecedence(tree->right->data)) {
        if (getPrecedence(tree->data) > getPrecedence(tree->right->data)) {
            needParens = 1;
        }
        else if (getPrecedence(tree->data) == 4 && getPrecedence(tree->right->data) == 4){
            needParens = 1;
        }
    }

    if (needParens) strcat(output, "( ");

    //right subtree
    output = finalOutput(tree->right, output);

    if (needParens){
        strcat(output, ") ");
        needParens = 0;
    }


    return output;
}

char* parse(Token* tokens) {
    StackPtr stack = NULL;      //stack for parsing
    Token endToken;             //end symbol, first symbol of the stack
    Token stackTop;             //top of the stack 
    Token symbol1;              //token with the symbol < to help in parsing
    Token memory[50];           //I used this array of token to store the elements that were on the top of the stack when a rule was applied
    char action;                //action to be taken
    char* output=NULL;          //output string
    int n, i, j, flag;          //some variables for loops and flags
    StackTreePtr stackTree = NULL; //this stack help me create the abstract syntax tree
    symbol1.type = P;              //P is the type that I gave to the symbol <
    endToken.type = END;           //END is the type that I gave to the end symbol
    endToken.value = (char*)malloc(2*sizeof(char));     //allocate memory for the end symbol
    endToken.value[0] = '$';                           
    endToken.value[1] = '\0';
    push(&stack, endToken);        //push the end symbol to the stack
    while(1){
        stackTop = pop(&stack);      //here I pop the top of the stack to use for determining the action to be taken
        if (stackTop.type == S && stack->value.type == END && tokens->type == END){
            break;
        }
        if ((stackTop.type == A || stackTop.type == B || stackTop.type == S) && tokens->type != END){
            if ((stackTop.type == A || stackTop.type == B) && (tokens->type == RPAREN || tokens->type == COMMA)){
                action = '>';
            }
            else action ='=';
        }
        else if (tokens->type == END){
            action = '>';
        }
        else action = precedenceTable[stackTop.type][tokens->type]; //here I use the precedence table to determine the action to be taken
        switch(action){
            case '=':
                push(&stack, stackTop);
                push(&stack, *tokens);
                tokens++;
                break;
            case '<':
                push(&stack, stackTop);
                push(&stack, symbol1);
                push(&stack, *tokens);
                tokens++;
                break;
            case '>':
                n=0;
                //here I store the elements that are on the top of the stack in the memory array until I find the symbol <
                while(stackTop.type != P && stackTop.type!=END){
                    memory[n++]=stackTop;
                    stackTop=pop(&stack);
                }
                //if the stack top is not < there were no < in the stack so there is a syntax error
                if (stackTop.type != P || n>7){     //if n>7 then there is no rule to apply
                    output = (char*)malloc(14*sizeof(char));
                    strcpy(output, "Syntax Error!");
                    return output;
                }
                for(i=0; i<10; i++){
                    flag=0;
                    for(j=0; j<n; j++){
                        // here I check the memory from the end to the beginning to see if the rule is applicable
                        if(rules[i].right[j]!=memory[n-j-1].type){  //if the rule is not applicable then flag=1
                            flag=1;
                            break;
                        }
                    }
                    //if the rule is applicable then I push the left part of the rule to the stack and apply the semantic action corresponding to the rule
                    if(!flag && rules[i].right[n]==-1){
                        push(&stack, rules[i].left);
                        switch (i){
                            case 0: semanticAction1(&stackTree, memory[n-1]); break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                            case 7: semanticAction4(&stackTree, memory[n-1]); break;
                            case 8: semanticAction2(&stackTree); break;
                            case 9: semanticAction3(&stackTree); break;
                        }
                        break;
                    }
                }
                if(flag){    //if flag=1 there is no rule applicable then there is a syntax error
                    output = (char*)malloc(14*sizeof(char));
                    strcpy(output, "Syntax Error!");
                    return output;
                }
                break;
            }
        // I used this two function to check the stack
        //printStack(stack);
        //printf("\n");
    }
    if (stack->value.type == END){   //if the stack top is the end symbol then I create the output string
        output = finalOutput(stackTree->value, output);
        return output;
    }
    else{
        output = (char*)malloc(14*sizeof(char));
        strcpy(output, "Syntax Error!");
        return output;
    }
}