//
//
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"
#include <string.h>


Lexer * create_lexer(char *text){
    Lexer * lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(lexer->text, text);
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
    return lexer;
};

/**
 * Function for advancing lexer position to next character from which to start extracting next token
 */

void advance(Lexer * lexer){
    lexer->pos++;
    lexer->current_char = lexer->text[lexer->pos];
}

void skip_whitespace(Lexer * lexer){
    while(lexer->current_char != '\0' && isspace(lexer->current_char)){
        advance(lexer);
    }
}

// Get integer value of next token as integer
int integer(Lexer * lexer){
    int result = 0;
    while(lexer->current_char != '\0'  && isdigit(lexer->current_char)){
        result = result * 10 + (int)(lexer->current_char - '0');
        advance(lexer);
    }
    return result;
}

Token * create_token(TokenType type, ValueType valueType, int value){
    Token * token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->valueType = valueType;
//    token->value = value;
    token->value.intValue = value;
    return token;
}

// Get next token
Token * get_next_token(Lexer * lexer){
    while(lexer->current_char != '\0'){
        if(isspace(lexer->current_char)){
            skip_whitespace(lexer);
            continue;
        }

        if(isdigit(lexer->current_char)){
            return create_token(TOKEN_NUMBER, INT, integer(lexer));
        }

        if(lexer->current_char == '+'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_PLUS, CHAR, '+');
        }

        if(lexer->current_char == '-'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MINUS, CHAR, '-');
        }

        if(lexer->current_char == '*'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MULT, CHAR, '*');
        }

        if(lexer->current_char == '/'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_DIV, CHAR, '/');
        }

        if(lexer->current_char == '('){
            advance(lexer);
            return create_token(TOKEN_LPAREN, CHAR, '(');
        }

        if(lexer->current_char == ')'){
            advance(lexer);
            return create_token(TOKEN_RPAREN, CHAR, ')');
        }

        printf("\nError : Invalid character.\n");
        exit(EXIT_FAILURE);
    }
    return create_token(TOKEN_EOF, INT, -1);

}
