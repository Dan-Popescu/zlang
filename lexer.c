//
// Created by Charara on 10/20/2024.
//

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <math.h>

/**
 *
 * Function for creating a lexer object
 * @param text : the text to be tokenized
 * @return : a lexer object
 *
 * This function initializes the lexer object with the text to be tokenized by allocating memory for the text
 * as well as the lexer object itself. It then copies the text to the lexer object's text field
 * and sets the position of the lexer to the first character of the text
 * and sets the current character to the first character of the text
 *
 */
Lexer *create_lexer(const char *text) {
    Lexer *lexer = (Lexer *) malloc(sizeof(Lexer));
    lexer->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(lexer->text, text);
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
    return lexer;
};


/**
 *
 * Function for freeing memory allocated for the lexer object
 * @param lexer : the lexer object to be freed
 *
 * This function frees the memory allocated for the text field of the lexer object
 * and then frees the memory allocated for the lexer object itself
 *
 */
void free_lexer(Lexer *lexer) {
    if (lexer == NULL) return;
    // free text
    if (lexer->text != NULL) {
        free(lexer->text);
    }
    free(lexer);
}

/**
 * Function for advancing lexer position to next character from which to start extracting next token
 * @param lexer : the lexer object
 *
 * This function increments the position of the lexer object by one
 * and sets the current character to the character at the new position
 */

void advance(Lexer *lexer) {
    lexer->pos++;
    lexer->current_char = lexer->text[lexer->pos];
}


/**
 * Function for skipping whitespace characters
 * @param lexer : the lexer object
 *
 * This function advances the lexer object to the next non-whitespace character
 */
void skip_whitespace(Lexer *lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        advance(lexer);
    }
}

/**
 *
 * Function for extracting the integer value of the next token
 *
 * @param lexer : the lexer object
 * @return : the integer value of the next token
 *
 * this function extracts the integer value of the next token. It does this by iterating over the characters
 * until it reaches a non-digit character. It then converts the extracted characters to an integer value.
 * it advances the lexer object to the next non-digit character using the advance() function
 *
 */
int integer(Lexer *lexer) {
    int result = 0;
    while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
        result = result * 10 + (int) (lexer->current_char - '0');
        advance(lexer);
    }
    return result;
}

/**
 *
 * Function for creating a token object
 *
 * @param type : the type of the token
 * @param valueType : the value type of the token
 * @param valueString : the value of the token as a string
 * @return : the token object
 *
 * This function creates a token object by allocating memory for the token object
 * and then setting the type, value type and value of the token object
 *
 */
Token *create_token(TokenType type, ValueType valueType, const char *valueString) {
    Token *token = (Token *) malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed for token.\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->valueType = valueType;

    if (valueType == INT) {
        token->value.intValue = strtol(valueString, NULL, 10);
        const int value = token->value.intValue;
        if (value == INT_MAX || value == INT_MIN) {
            fprintf(stderr, "Value provided for token is out of bounds for INT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == LONG) {
        token->value.longValue = strtol(valueString, NULL, 10);
        const long value = token->value.longValue;
        if (value == LONG_MAX || value == LONG_MIN) {
            fprintf(stderr, "Value provided for token is out of bounds for LONG type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == FLOAT) {
        errno = 0;
        token->value.floatValue = strtof(valueString, NULL);
        const float value = token->value.floatValue;
        if (value == HUGE_VALF || value == -HUGE_VALF) {
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == DOUBLE) {
        errno = 0;
        token->value.doubleValue = strtod(valueString, NULL);
        const float value = token->value.floatValue;
        if (value == HUGE_VALF || value == -HUGE_VALF) {
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == CHAR) {
        if (valueString == NULL) {
            fprintf(stderr, "Value provided for token is NULL for CHAR type in create_token().\n");
            exit(EXIT_FAILURE);
        }
        token->value.charValue = *valueString;
    }
    if (valueType == STRING) {
        token->value.strValue = calloc(strlen(valueString) + 1, sizeof(char));
        strcpy(token->value.strValue, valueString);
    }

    return token;
}


/**
 *
 * Function for initializing the reserved keywords
 * @return : the reserved keywords
 *
 * This function initializes the reserved keywords by allocating memory for the reserved keywords object
 * and then setting the size of the reserved keywords object to 1
 * and then allocating memory for the keywords array of the reserved keywords object
 * and then setting the value of the keywords array to the reserved keywords
 *
 */
RESERVED_KEYWORDS *initialize_reserved_keywords() {
    RESERVED_KEYWORDS *reservedKeywords = malloc(sizeof(RESERVED_KEYWORDS));
    reservedKeywords->size = 1;
    reservedKeywords->keywords = malloc(reservedKeywords->size * sizeof(RESERVED_KEYWORD *));
    if (reservedKeywords->keywords == NULL) {
        fprintf(stderr, "Memory allocation failed for reserved keywords in initialize_reserved_keywords().\n");
        exit(EXIT_FAILURE);
    }

    const char *lexemes[] = {"print"};
    const TokenType tokenTypes[] = {TOKEN_KEYWORD_PRINT};

    for (unsigned long i = 0; i < reservedKeywords->size; ++i) {
        reservedKeywords->keywords[i] = malloc(sizeof(RESERVED_KEYWORD));
        reservedKeywords->keywords[i]->token = malloc(sizeof(Token));
        reservedKeywords->keywords[i]->token->type = tokenTypes[i];
        reservedKeywords->keywords[i]->token->valueType = STRING;
        reservedKeywords->keywords[i]->token->value.strValue = calloc(strlen(lexemes[i]) + 1, sizeof(char));
        strcpy(reservedKeywords->keywords[i]->token->value.strValue, lexemes[i]);
    }

    return reservedKeywords;
}


/**
 *
 * Function for freeing memory allocated for the reserved keywords
 * @param reservedKeywords : the reserved keywords object
 *
 * This function frees the memory allocated for the reserved keywords object
 * and then frees the memory allocated for the token object of each reserved keyword
 * and then frees the memory allocated for each reserved keyword
 *
 */
void free_reserved_keywords(const RESERVED_KEYWORDS *reservedKeywords) {
    if (reservedKeywords == NULL) return;
    if (reservedKeywords->keywords == NULL) return;
    for (unsigned short i = 0; i < reservedKeywords->size; i++) {
        if (reservedKeywords->keywords[i] == NULL) {
            continue;
        }
        RESERVED_KEYWORD *keyword = reservedKeywords->keywords[i];
        const Token *keywordToken = reservedKeywords->keywords[i]->token;
        if (keywordToken != NULL) {
            free_token(reservedKeywords->keywords[i]->token);
        }
        if (keyword != NULL) {
            free(reservedKeywords->keywords[i]);
        }
    }
    free(reservedKeywords->keywords);
}

// Helper function for dynamic memory allocation and copying
char *allocate_and_copy(char *result, unsigned short *capacity) {
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed for result in allocate and copy.\n");
        exit(EXIT_FAILURE);
    }
    char *newResult = calloc(*capacity * 2, sizeof(char));
    const unsigned long size = strlen(result);

    for (int i = 0; i < *capacity; ++i) {
        if (i <= size) newResult[i] = result[i];
        else newResult[i] = '\0';
    }

    free(result);
    *capacity *= 2;
    return newResult;
}


/**
 * Function for extracting the identifier token such as reserved keywords or user-defined identifiers
 *
 * @param lexer : the lexer object
 * @return : the token object (identifier or reserved keyword) always with STRING value type
 *
 *  this function extracts identifier tokens by iterating over the characters until it reaches a non-alphabet character.
 *  it then checks if the identifier corresponds to any of the reserved keywords. If it does, it returns the corresponding
 *  identifier as a token object. If it does not, it returns the identifier as a token object
 */
Token *identifier(Lexer *lexer) {
    unsigned short capacity = 10;
    char *result = calloc(capacity, sizeof(char));
    while (lexer->current_char != ' ' && lexer->current_char != ';' && isalpha(lexer->current_char)) {
        const char currChar = lexer->current_char;
        unsigned long size = strlen(result);

        if (size >= capacity - 1) {
            result = allocate_and_copy(result, &capacity);
        }

        // update size in case of the reallocation with capacity doubling
        size = strlen(result);
        // append character
        result[size] = currChar;
        result[size + 1] = '\0';
        advance(lexer);
    }

    // check if the identifier corresponds to any of the reserved keywords
    const RESERVED_KEYWORDS *reservedKeywords = initialize_reserved_keywords();
    if (reservedKeywords->keywords == NULL) {
        fprintf(stderr, "Memory allocation failed for reserved keywords in identifier().\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned short i = 0; i < reservedKeywords->size; ++i) {
        if (strcmp(reservedKeywords->keywords[i]->token->value.strValue, result) == 0) {
            const char *keywordStr = reservedKeywords->keywords[i]->token->value.strValue;
            const unsigned int tokenType = reservedKeywords->keywords[i]->token->type;
            const unsigned int tokenValueType = reservedKeywords->keywords[i]->token->valueType;
            char *tokenStrValue = calloc(strlen(keywordStr) + 1, sizeof(char));
            strcpy(tokenStrValue, keywordStr);

            Token *keywordToken = create_token(tokenType, tokenValueType, tokenStrValue);

            free_reserved_keywords(reservedKeywords);
            return keywordToken;
        }
    }

    Token *token = create_token(TOKEN_IDENTIFIER, STRING, result);
    return token;
}

/**
 *
 * Function for extracting the next token from the text
 * @param lexer : the lexer object
 * @return : the next token
 *
 * This function extracts the next token from the text by checking the current character
 * and then returning the corresponding token
 *
 * @note: EOF token is of type INT and has a value of -1
 *
 */
Token *get_next_token(Lexer *lexer) {
    while (lexer->current_char != '\0') {
        if (isspace(lexer->current_char)) {
            skip_whitespace(lexer);
            continue;
        }

        if (isalpha(lexer->current_char)) {
            return identifier(lexer);
        }

        if (isdigit(lexer->current_char)) {
            const int value = integer(lexer);
            char *intStrValue = calloc(32, sizeof(char));
            sprintf(intStrValue, "%d", value);
            //            return create_token(TOKEN_NUMBER, INT, integer(lexer));
            return create_token(TOKEN_NUMBER, INT, intStrValue);
        }

        if (lexer->current_char == '=') {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_ASSIGNMENT, CHAR, "=");
        }

        if (lexer->current_char == '+') {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_PLUS, CHAR, "+");
        }

        if (lexer->current_char == '-') {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MINUS, CHAR, "-");
        }

        if (lexer->current_char == '*') {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MULT, CHAR, "*");
        }

        if (lexer->current_char == '/') {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_DIV, CHAR, "/");
        }

        if (lexer->current_char == '(') {
            advance(lexer);
            return create_token(TOKEN_LPAREN, CHAR, "(");
        }

        if (lexer->current_char == ')') {
            advance(lexer);
            return create_token(TOKEN_RPAREN, CHAR, ")");
        }

        if (lexer->current_char == ';') {
            advance(lexer);
            return create_token(TOKEN_SEMI_COLON, CHAR, ";");
        }

        printf("\nError : Invalid character.\n");
        exit(EXIT_FAILURE);
    }
    return create_token(TOKEN_EOF, INT, "-1");
}


/**
 *
 * Function for freeing memory allocated for a token object
 * @param token : the token object
 *
 * This function frees the memory allocated for the string value of the token object
 * and then frees the memory allocated for the token object itself
 *
 */
void free_token(Token *token) {
    if (token == NULL) return;
    if (token->valueType == STRING && token->value.strValue != NULL) {
        free(token->value.strValue);
    }
    free(token);
}
