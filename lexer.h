//
//
//

#ifndef ZLANG_LEXER_H
#define ZLANG_LEXER_H

typedef struct {
    char *text;
    int pos;
    char current_char;
} Lexer;

typedef enum{
    TOKEN_NUMBER,
//    TOKEN_OPERATOR,
    TOKEN_OPERATOR_PLUS,
    TOKEN_OPERATOR_MINUS,
    TOKEN_OPERATOR_MULT,
    TOKEN_OPERATOR_DIV,
    TOKEN_VARIABLE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_PERIOD,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_IDENTIFIER, // variable names, function names
    TOKEN_OPERATOR_ASSIGNMENT,
    TOKEN_KEYWORD, // reserved keywords in the language
    TOKEN_INSTRUCTION_END, // ;character
    TOKEN_EOF
} TokenType;

typedef enum{
    UNSIGNED_CHAR,
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    UNSIGNED_LONG,
    CHAR,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    STRING
} ValueType;


typedef struct{
    int intValue;
    long longValue;
    float floatValue;
    double doubleValue;
    char * strValue;
} TokenValueUnion;

typedef struct{
    TokenType type;
    TokenValueUnion value;
    unsigned int valueType;
} Token;

typedef struct{
    unsigned int valueType;
    void * value;
} ExpressionValue;

Lexer * create_lexer(char *text);
void free_lexer(Lexer * lexer);
void advance(Lexer * lexer);
void skip_whitespace(Lexer * lexer);
int integer(Lexer * lexer);
Token * create_token(TokenType type, ValueType valueType, int value);
void free_token(Token * token);
Token * identifier(Lexer * lexer);
Token * get_next_token(Lexer * lexer);



#endif //ZLANG_LEXER_H
