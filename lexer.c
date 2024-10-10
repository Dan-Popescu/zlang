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
    free(lexer);
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

RESERVED_KEYWORDS * initialize_reserved_keywords(){
    RESERVED_KEYWORDS * reservedKeywords = malloc(sizeof (RESERVED_KEYWORDS));
    reservedKeywords->size = 1;
    reservedKeywords->keywords = malloc(reservedKeywords->size * sizeof(RESERVED_KEYWORD * ));

    const char * lexemes[] = {"print"};
    TokenType tokenTypes[] = { TOKEN_KEYWORD_PRINT};

    for(unsigned long i = 0; i < reservedKeywords->size; ++i){
        reservedKeywords->keywords[i] = malloc(sizeof(RESERVED_KEYWORD));
        reservedKeywords->keywords[i]->token = malloc(sizeof(Token));
        reservedKeywords->keywords[i]->token->type = tokenTypes[i];
        reservedKeywords->keywords[i]->token->valueType = STRING;
        reservedKeywords->keywords[i]->token->value.strValue = calloc(strlen(lexemes[i]) + 1, sizeof(char));
        strcpy(reservedKeywords->keywords[i]->token->value.strValue, lexemes[i]);
    }

    return reservedKeywords;
}

void free_reserved_keywords(RESERVED_KEYWORDS * reservedKeywords){
    for (unsigned short i = 0; i < reservedKeywords->size; i++) {
        free(reservedKeywords->keywords[i]->token);
        free(reservedKeywords->keywords[i]);
    }
    free(reservedKeywords->keywords);
}

Token * identifier(Lexer * lexer) {
    unsigned short capacity = 10;
    char *result = calloc(capacity, sizeof(char));
    while (lexer->current_char != ' ' && lexer->current_char != ';' && isalpha(lexer->current_char)) {
        char currChar = lexer->current_char;
        long size = strlen(result);
        if (size >= capacity - 1) {
            char *newResult = calloc(capacity * 2, sizeof(char));

            for (int i = 0; i < capacity; ++i) {
                if(i<=size) newResult[i] = result[i];
                else newResult[i] = '\0';
            }
            free(result);
            result = newResult;
        }
        strcat(result, &currChar);
        advance(lexer);
    }

    // check if the identifier corresponds to any of the reserved keywords
    RESERVED_KEYWORDS  * reservedKeywords = initialize_reserved_keywords();
    for(unsigned short i = 0; i < reservedKeywords->size; ++i){
        if(strcmp(reservedKeywords->keywords[i]->token->value.strValue, result)==0){
            char * keywordStr = reservedKeywords->keywords[i]->token->value.strValue;
            unsigned int tokenType = reservedKeywords->keywords[i]->token->type;
            unsigned int tokenValueType = reservedKeywords->keywords[i]->token->valueType;
            char * tokenStrValue = calloc(strlen(keywordStr) + 1 ,
                                                  sizeof(char));
            strcpy(tokenStrValue, keywordStr);

            Token * keywordToken = create_token(tokenType, tokenValueType, tokenStrValue);

            free_reserved_keywords(reservedKeywords);
            return keywordToken;
        }
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

        if(lexer->current_char == ';'){
            advance(lexer);
            return create_token(TOKEN_SEMI_COLON, CHAR, ";");
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
