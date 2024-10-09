//
//
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>


Lexer * create_lexer(char *text){
    Lexer * lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(lexer->text, text);
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
    return lexer;
};

void free_lexer(Lexer * lexer){
    if(lexer == NULL) return;
    // free text
    if(lexer->text != NULL){
        free(lexer->text);
    }
}

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

//Token * create_token(TokenType type, ValueType valueType, int value){
//    Token * token = (Token *)malloc(sizeof(Token));
//    if (token == NULL) {
//        fprintf(stderr, "Memory allocation failed for token.\n");
//        exit(EXIT_FAILURE);
//    }
//    token->type = type;
//    token->valueType = valueType;
//    token->value.intValue = value;
//    return token;
//}

Token * create_token(TokenType type, ValueType valueType, char * valueString){
    Token * token = (Token *)malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed for token.\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->valueType = valueType;

    if(valueType == INT) {
        token->value.intValue = strtod(valueString, NULL);
        int value = token->value.intValue;
        if(value == INT_MAX || value == INT_MIN ){
            fprintf(stderr, "Value provided for token is out of bounds for INT type");
            exit(EXIT_FAILURE);
        }
    }
    if(valueType == LONG) {
        token->value.longValue = strtol(valueString, NULL, 10);
        long value = token->value.longValue;
        if(value == LONG_MAX || value == LONG_MIN ){
            fprintf(stderr, "Value provided for token is out of bounds for LONG type");
            exit(EXIT_FAILURE);
        }
    }
    if(valueType == FLOAT) {
        errno = 0;
        token->value.floatValue = strtof(valueString, NULL);
        float value = token->value.floatValue;
        if(value == HUGE_VALF || value == -HUGE_VALF ){
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }
    }
    if(valueType == DOUBLE) {
        errno = 0;
        token->value.doubleValue = strtod(valueString, NULL);
        float value = token->value.floatValue;
        if(value == HUGE_VALF || value == -HUGE_VALF ){
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }

    }
    if(valueType == CHAR) token->value.charValue = *valueString;
    if(valueType == STRING){
        token->value.strValue = calloc(strlen(valueString) + 1, sizeof(char));
        strcpy(token->value.strValue, valueString);
    }

    return token;
}


Token * identifier(Lexer * lexer) {
    unsigned short capacity = 10;
    char *result = calloc(capacity, sizeof(char));
    long index = 0;
    while (lexer->current_char != ' ' && lexer->current_char != ';' && isalpha(lexer->current_char)) {
        char currChar = lexer->current_char;
        long size = strlen(result);
        if (size >= index - 1) {
            char *newResult = calloc(capacity * 2, sizeof(char));
            for (int i = 0; i < capacity; ++i) {
                newResult[i] = result[i];
            }
            free(result);
            result = newResult;
        }
        strncpy(result, &currChar, 1);
        advance(lexer);
    }

    Token * token = create_token(TOKEN_IDENTIFIER, STRING, result);

    return token;
}

// Get next token
Token * get_next_token(Lexer * lexer){
    while(lexer->current_char != '\0'){
        if(isspace(lexer->current_char)){
            skip_whitespace(lexer);
            continue;
        }

        if(isalpha(lexer->current_char)){
            return identifier(lexer);
        }

        if(isdigit(lexer->current_char)){
            int value = integer(lexer);
            char * intStrValue = calloc(32, sizeof(char));
            sprintf(intStrValue, "%d", value);
//            return create_token(TOKEN_NUMBER, INT, integer(lexer));
            return create_token(TOKEN_NUMBER, INT, intStrValue);
        }

        if(lexer->current_char == '='){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_ASSIGNMENT, CHAR, "=");
        }

        if(lexer->current_char == '+'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_PLUS, CHAR, "+");
        }

        if(lexer->current_char == '-'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MINUS, CHAR, "-");
        }

        if(lexer->current_char == '*'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MULT, CHAR, "*");
        }

        if(lexer->current_char == '/'){
            advance(lexer);
            return create_token(TOKEN_OPERATOR_DIV, CHAR, "/");
        }

        if(lexer->current_char == '('){
            advance(lexer);
            return create_token(TOKEN_LPAREN, CHAR, "(");
        }

        if(lexer->current_char == ')'){
            advance(lexer);
            return create_token(TOKEN_RPAREN, CHAR, ")");
        }

        printf("\nError : Invalid character.\n");
        exit(EXIT_FAILURE);
    }
    return create_token(TOKEN_EOF, INT, "-1");
}

void free_token(Token * token){
    if(token == NULL) return;
    if(token->valueType == STRING && token->value.strValue != NULL) {
        free(token->value.strValue);
    }
    free(token);
}
