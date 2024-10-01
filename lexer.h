//
// Created by popes on 01/10/2024.
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
    TOKEN_OPERATOR,
    TOKEN_VARIABLE,
    TOKEN_PUNCTUATION_LEFT_PARENTHESIS,
    TOKEN_PUNCTUATION_RIGHT_PARENTHESIS,
    TOKEN_PUNCTUATION_PERIOD,
    TOKEN_PUNCTUATION_LEFT_BRACKET,
    TOKEN_PUNCTUATION_RIGHT_BRACKET,
    TOKEN_IDENTIFIER, // variable names, function names
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
    TokenType type;
//    char *value;
    int value;
    unsigned int valueType;
//    char * startPtr;
//    char * endPtr;
//    long line;
//    long column;
} Token;

typedef struct{
    unsigned int valueType;
    void * value;
} ExpressionValue;

Lexer * create_lexer(char *text);
void advance(Lexer * lexer);
void skip_whitespace(Lexer * lexer);
int integer(Lexer * lexer);
Token * create_token(TokenType type, ValueType valueType, int value);
Token * get_next_token(Lexer * lexer);


#endif //ZLANG_LEXER_H
