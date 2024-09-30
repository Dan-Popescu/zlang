//
// Created by Popescu Dan on 29/09/2024.
//

#ifndef ZLANG_TOKENIZER_H
#define ZLANG_TOKENIZER_H

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
    char *value;
    unsigned int valueType;
    char * startPtr;
    char * endPtr;
    long line;
    long column;
} Token;

typedef struct{
    unsigned int valueType;
    void * value;
} ExpressionValue;


Token *get_next_token(char ** statement);
ExpressionValue *evaluate_expr(char ** expression);
ExpressionValue *  eval_expr(char ** expression);
ValueType get_token_value_type(const Token * token);
void addNextOperand(ExpressionValue * resultValue, ExpressionValue * termValue, ValueType returnType);
void subtractNextOperand(ExpressionValue * resultValue, ExpressionValue * termValue, ValueType returnType);
void multiplyNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType);
void divideByNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType);

#endif //ZLANG_TOKENIZER_H
