
// TokenType 
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
    P,      //13    symbol '<'
    A,      //14
    B,      //15
    S,      //16
} TokenType;

// Token structure declaration
typedef struct {
    TokenType type;
    char *value;
} Token;
