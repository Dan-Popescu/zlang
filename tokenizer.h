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

    TOKEN_END, // ; character
} TokenType;


typedef struct{
    TokenType type;
//    double value; // value when type is number
//    char operator; // operator when type is operator
//    char delimiter;
//    char value[64];
    char *value;
    long line;
    long column;
} Token; // should probably use a union instead

Token *get_next_token(char * statement);
Token *evaluate_expr(char * expression);

#endif //ZLANG_TOKENIZER_H
