#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OPERATORS 13

typedef enum {
    ADD=0,    //0
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

// Define a token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct treeNode{
    char* data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;
typedef TreeNode *TreeNodePtr;

typedef struct stackTree{
    TreeNodePtr value;
    struct stackTree* next;
} StackTree;
typedef StackTree* StackTreePtr;

typedef struct {
    Token left;
    int right[10];
}Rule;

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
        exit(EXIT_FAILURE); // Consider returning a default Token or handling this case differently
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

void printTree(TreeNodePtr tree){
    if (tree == NULL) return;
    printTree(tree->left);
    printf("%s ", tree->data);
    printTree(tree->right);
}


char* parse(Token* tokens) {
    StackPtr stack = NULL;
    Token endToken;
    TreeNodePtr tree = NULL;
    Token stackTop;
    Token symbol1;
    Token memory[50];
    char action;
    char* output=NULL;
    int n, i, j, flag;
    StackTreePtr stackTree = NULL;
    symbol1.type = P;   //<
    endToken.type = END;
    endToken.value = (char*)malloc(2*sizeof(char));
    endToken.value[0] = '$';
    endToken.value[1] = '\0';
    push(&stack, endToken);
    while(1){
        stackTop = pop(&stack);
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
        else action = precedenceTable[stackTop.type][tokens->type];
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
                while(stackTop.type != P && stackTop.type!=END){
                    memory[n++]=stackTop;
                    stackTop=pop(&stack);
                }
                if (stackTop.type != P || n>7){     //if n>7 then there is no rule to apply
                    output = (char*)malloc(14*sizeof(char));
                    strcpy(output, "Syntax Error!1");
                    return output;
                }
                for(i=0; i<10; i++){
                    flag=0;
                    for(j=0; j<n; j++){
                        if(rules[i].right[j]!=memory[n-j-1].type){
                            flag=1;
                            break;
                        }
                    }
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
                if(flag){
                    output = (char*)malloc(14*sizeof(char));
                    strcpy(output, "Syntax Error!2");
                    return output;
                }
                break;
            }
        
        printStack(stack);
        printf("\n");
    }
    if (stack->value.type == END){
        output = (char*)malloc(14*sizeof(char));
        strcpy(output, "true!");
        printTree(stackTree->value);
        return output;
    }
    else{
        output = (char*)malloc(14*sizeof(char));
        strcpy(output, "Syntax Error!3");
        return output;
    }
}

int main() {
    /*
    Token tokens[] = {
        {ADD, NULL},
        {LPAREN, NULL}, 
        {NUMBER, "5"},
        {COMMA, NULL},
        {MUL, NULL},
        {LPAREN, NULL},
        {NUMBER, "3"},
        {COMMA, NULL},
        {SUB, NULL},
        {LPAREN, NULL},
        {NUMBER, "10"},
        {COMMA, NULL},
        {POW, NULL},
        {LPAREN, NULL},
        {NUMBER, "6"},
        {COMMA, NULL},
        {NUMBER, "4"},
        {RPAREN, NULL},
        {RPAREN, NULL},
        {RPAREN, NULL},
        {RPAREN, NULL},
        {END, NULL}
    };
    */
    
    Token tokens[] = {
        {ADD, "+"},
        {LPAREN, "("},
        {ADD, "+"},
        {LPAREN, "("},
        {NUMBER, "1"},
        {COMMA, ","},
        {NUMBER, "2"},
        {RPAREN, ")"},
        {COMMA, ","},
        {NUMBER, "2"},
        {RPAREN, ")"},
        {END, NULL}
    };
    
    int i=0;
    while(tokens[i].type != END){
        printf("%d ", tokens[i].type);
        i++;
    }
    printf("\n");
    
    char* result = parse(tokens);
    printf("Parse result: %s\n", result);
    free(result);
    
    return 0;
}

