#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OPERATORS 16

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
    A,      //12
    B,      //13
    S,      //14
    ERROR,  //15
    P,      //16    <
    
} TokenType;

// Define a token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct treeNode{
    int data;
    struct treeNode *left;
    struct treeNode *right;
} TreeNode;
typedef TreeNode *TreeNodePtr;

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
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', ' ' },  // DOLLAR
    { '=', '=', '=', '=', '=', '=', '=', '=', '=', '>', '=', '>' },   // A
    { '=', '=', '=', '=', '=', '=', '=', '=', '=', '>', '=', '>' },   // B
    { '=', '=', '=', '=', '=', '=', '=', '<', '=', '<', '=', '=' },   // S
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

void printStack(StackPtr stack) {
    if (stack == NULL) {
        printf("Stack is empty\n");
        return;
    }
    
    StackPtr current = stack;
    printf("Stack (top to bottom): ");
    while (current != NULL) {
        printf("%d ", current->value.type);
        current = current->next;
    }
}


char* tokenToString(Token* tokens, int n){
    int i=0;
    char* output=(char*)malloc(sizeof(char));
    output[0]='\0';
    for(i=0; i<n; i++){
        output = (char*)realloc(output, (strlen(output)+strlen(tokens[i].value))*sizeof(char));
        strcat(output, tokens[i].value);
    }
    return output;
}

char* parse(Token* tokens) {
    StackPtr stack = NULL;
    Token endToken;
    TreeNodePtr tree = NULL;
    Token stackTop;
    Token symbol1, s;
    Token memory[50];
    char action;
    char* output=NULL;
    int n, i, j, flag;
    s.type = S;
    symbol1.type = P;   //<
    endToken.type = END;
    endToken.value = (char*)malloc(2*sizeof(char));
    endToken.value[0] = '$';
    endToken.value[1] = '\0';
    stack = (StackPtr)malloc(sizeof(StackNode));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    stack->value = endToken;
    stack->next = NULL;
    
    while(1){
        stackTop = pop(&stack);
        if (stackTop.type == S && stack->value.type == END){
            break;
        }
        //printf("%d - ", tokens->type);
        //printf("%c", precedenceTable[stackTop.type][tokens->type]);
         
            switch(precedenceTable[stackTop.type][tokens->type]){
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
                        break;
                    }
                }
                if(flag){
                    output = (char*)malloc(14*sizeof(char));
                    strcpy(output, "Syntax Error!2");
                    return output;
                }
                break;
            default:
                output = (char*)malloc(14*sizeof(char));
                strcpy(output, "Syntax Error!3");
                return output;
            }
        
        printStack(stack);
        printf("\n");
    }
    if (stack->value.type == END){
        output = (char*)malloc(14*sizeof(char));
        strcpy(output, "true!");
        return output;
    }
    else{
        output = (char*)malloc(14*sizeof(char));
        strcpy(output, "Syntax Error!3");
        return output;
    }
}

int main() {
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

