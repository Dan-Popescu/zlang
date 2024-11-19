

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

/**
 *
 * Initializes a Lexer with the input text, starting position, and the first character.
 *
 * @param text - The source code text to tokenize.
 * @return - A pointer to the created Lexer.
 */

Lexer *create_lexer(char *text)
{
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(lexer->text, text);
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
    return lexer;
};

/**
 *
 * Frees the memory allocated for the Lexer, including its text buffer.
 *
 * @param lexer - The Lexer to free.
 */

void free_lexer(Lexer *lexer)
{
    if (lexer == NULL)
        return;
    // free text
    if (lexer->text != NULL)
    {
        free(lexer->text);
    }
    free(lexer);
}

/**
 *
 * Moves the Lexer's position to the next character in the text.
 *
 * @param lexer - The Lexer being advanced.
 */

void advance(Lexer *lexer)
{
    lexer->pos++;
    lexer->current_char = lexer->text[lexer->pos];
}

/**
 *
 * Advances the Lexer's position to skip over whitespace characters.
 *
 * @param lexer - The Lexer performing the operation.
 */

void skip_whitespace(Lexer *lexer)
{
    while (lexer->current_char != '\0' && isspace(lexer->current_char))
    {
        advance(lexer);
    }
}

/**
 *
 * Parses and returns the next integer value from the Lexer's text.
 *
 * @param lexer - The Lexer performing the operation.
 * @return - The integer value parsed.
 */

int integer(Lexer *lexer)
{
    int result = 0;
    while (lexer->current_char != '\0' && isdigit(lexer->current_char))
    {
        result = result * 10 + (int)(lexer->current_char - '0');
        advance(lexer);
    }
    return result;
}

/**
 *
 * Creates a token with the specified type, value type, and value.
 * Handles validation and memory allocation for the token.
 *
 * @param type - The type of the token (e.g., identifier, operator, etc.).
 * @param valueType - The type of the token's value (e.g., INT, STRING).
 * @param valueString - The string representation of the token's value.
 * @return - A pointer to the created Token.
 */

Token *create_token(TokenType type, ValueType valueType, char *valueString)
{
    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL)
    {
        fprintf(stderr, "Memory allocation failed for token.\n");
        exit(EXIT_FAILURE);
    }
    token->type = type;
    token->valueType = valueType;

    token->value.strValue = NULL;
    token->value.intValue = 0;
    token->value.floatValue = 0;
    token->value.longValue = 0;
    token->value.charValue = 0;
    token->value.doubleValue = 0;

    if (valueType == INT)
    {
        token->value.intValue = strtod(valueString, NULL);
        int value = token->value.intValue;
        if (value == INT_MAX || value == INT_MIN)
        {
            fprintf(stderr, "Value provided for token is out of bounds for INT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == LONG)
    {
        token->value.longValue = strtol(valueString, NULL, 10);
        long value = token->value.longValue;
        if (value == LONG_MAX || value == LONG_MIN)
        {
            fprintf(stderr, "Value provided for token is out of bounds for LONG type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == FLOAT)
    {
        errno = 0;
        token->value.floatValue = strtof(valueString, NULL);
        float value = token->value.floatValue;
        if (value == HUGE_VALF || value == -HUGE_VALF)
        {
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == DOUBLE)
    {
        errno = 0;
        token->value.doubleValue = strtod(valueString, NULL);
        float value = token->value.floatValue;
        if (value == HUGE_VALF || value == -HUGE_VALF)
        {
            fprintf(stderr, "Value provided for token is out of bounds for FLOAT type");
            exit(EXIT_FAILURE);
        }
    }
    if (valueType == CHAR)
        token->value.charValue = *valueString;
    if (valueType == STRING)
    {
        token->value.strValue = calloc(strlen(valueString) + 1, sizeof(char));
        strcpy(token->value.strValue, valueString);
    }

    return token;
}

/**
 *
 * Creates a list of reserved keywords (e.g., `print`, `while`, `for`) with their corresponding token types.
 *
 * @return - A pointer to the initialized RESERVED_KEYWORDS structure.
 */

RESERVED_KEYWORDS *initialize_reserved_keywords()
{
    RESERVED_KEYWORDS *reservedKeywords = malloc(sizeof(RESERVED_KEYWORDS));
    reservedKeywords->size = 3;
    reservedKeywords->keywords = malloc(reservedKeywords->size * sizeof(RESERVED_KEYWORD *));

    const char *lexemes[] = {"print", "while", "for"};
    TokenType tokenTypes[] = {TOKEN_KEYWORD_PRINT, TOKEN_KEYWORD_WHILE, TOKEN_KEYWORD_FOR};

    for (unsigned long i = 0; i < reservedKeywords->size; ++i)
    {
        reservedKeywords->keywords[i] = malloc(sizeof(RESERVED_KEYWORD));
        reservedKeywords->keywords[i]->token = malloc(sizeof(Token));
        reservedKeywords->keywords[i]->token->type = tokenTypes[i];
        reservedKeywords->keywords[i]->token->valueType = STRING;
        reservedKeywords->keywords[i]->token->value.strValue = calloc(strlen(lexemes[i]) + 1, sizeof(char));
        reservedKeywords->keywords[i]->token->value.charValue = 0;
        reservedKeywords->keywords[i]->token->value.intValue = 0;
        reservedKeywords->keywords[i]->token->value.longValue = 0;
        reservedKeywords->keywords[i]->token->value.floatValue = 0;
        reservedKeywords->keywords[i]->token->value.doubleValue = 0;
        strcpy(reservedKeywords->keywords[i]->token->value.strValue, lexemes[i]);
    }

    return reservedKeywords;
}

/**
 *
 * Frees the memory allocated for the RESERVED_KEYWORDS structure, including its tokens.
 *
 * @param reservedKeywords - The RESERVED_KEYWORDS structure to free.
 */

void free_reserved_keywords(RESERVED_KEYWORDS *reservedKeywords)
{
    for (unsigned short i = 0; i < reservedKeywords->size; i++)
    {
        RESERVED_KEYWORD *keyword = reservedKeywords->keywords[i];
        Token *keywordToken = reservedKeywords->keywords[i]->token;
        if (keywordToken != NULL)
        {
            free_token(reservedKeywords->keywords[i]->token);
        }
        if (keyword != NULL)
        {
            free(keyword);
        }
    }
    free(reservedKeywords->keywords);
    reservedKeywords->keywords = NULL;
    free(reservedKeywords);
    reservedKeywords = NULL;
}

/**
 *
 * Parses and returns an identifier or reserved keyword token from the Lexer's text.
 *
 * @param lexer - The Lexer performing the operation.
 * @return - A Token representing the identifier or reserved keyword.
 */

Token *identifier(Lexer *lexer)
{
    unsigned short capacity = 10;
    char *result = calloc(capacity, sizeof(char));
    while (lexer->current_char != ' ' && lexer->current_char != ';' && isalpha(lexer->current_char))
    {
        char currChar = lexer->current_char;
        long size = strlen(result);
        if (size >= capacity - 1)
        {
            char *newResult = calloc(capacity * 2, sizeof(char));

            // copy characters in the new extended string
            for (int i = 0; i < capacity; ++i)
            {
                if (i <= size)
                    newResult[i] = result[i];
                else
                    newResult[i] = '\0';
            }
            free(result);
            result = newResult;
        }

        // update size in case of the reallocation with capacity doubling
        size = strlen(result);
        // append character
        char *char_to_append = calloc(2, sizeof(char));
        char_to_append[0] = currChar;
        strcat(result, char_to_append);
        free(char_to_append);
        char_to_append = NULL;
        advance(lexer);
    }

    // check if the identifier corresponds to any of the reserved keywords
    RESERVED_KEYWORDS *reservedKeywords = initialize_reserved_keywords();
    for (unsigned short i = 0; i < reservedKeywords->size; ++i)
    {
        if (strcmp(reservedKeywords->keywords[i]->token->value.strValue, result) == 0)
        {
            char *keywordStr = reservedKeywords->keywords[i]->token->value.strValue;
            unsigned int tokenType = reservedKeywords->keywords[i]->token->type;
            unsigned int tokenValueType = reservedKeywords->keywords[i]->token->valueType;
            char *tokenStrValue = calloc(strlen(keywordStr) + 1,
                                         sizeof(char));
            strcpy(tokenStrValue, keywordStr);

            Token *keywordToken = create_token(tokenType, tokenValueType, tokenStrValue);
            free(tokenStrValue);

            free_reserved_keywords(reservedKeywords);
            return keywordToken;
        }
    }
    if (reservedKeywords != NULL)
    {
        free_reserved_keywords(reservedKeywords);
    }

    Token *token = create_token(TOKEN_IDENTIFIER, STRING, result);
    free(result);

    return token;
}

/**
 *
 * Retrieves the next token from the Lexer's text, identifying its type and value.
 * Handles different token types, including numbers, operators, and keywords.
 *
 * @param lexer - The Lexer performing the operation.
 * @return - A Token representing the next lexical unit.
 */

Token *get_next_token(Lexer *lexer)
{
    while (lexer->current_char != '\0')
    {
        if (isspace(lexer->current_char))
        {
            skip_whitespace(lexer);
            continue;
        }

        if (isalpha(lexer->current_char))
        {
            return identifier(lexer);
        }

        if (isdigit(lexer->current_char))
        {
            int value = integer(lexer);
            char *intStrValue = calloc(10, sizeof(char));
            sprintf(intStrValue, "%d", value);
            Token *token = create_token(TOKEN_NUMBER, INT, intStrValue);
            free(intStrValue);
            return token;
        }

        if (lexer->current_char == '=')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_ASSIGNMENT, CHAR, "=");
        }

        if (lexer->current_char == '+')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_PLUS, CHAR, "+");
        }

        if (lexer->current_char == '-')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MINUS, CHAR, "-");
        }

        if (lexer->current_char == '*')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_MULT, CHAR, "*");
        }

        if (lexer->current_char == '/')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_DIV, CHAR, "/");
        }

        if (lexer->current_char == '(')
        {
            advance(lexer);
            return create_token(TOKEN_LPAREN, CHAR, "(");
        }

        if (lexer->current_char == ')')
        {
            advance(lexer);
            return create_token(TOKEN_RPAREN, CHAR, ")");
        }

        if (lexer->current_char == ';')
        {
            advance(lexer);
            return create_token(TOKEN_SEMI_COLON, CHAR, ";");
        }

        if (lexer->current_char == '<')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_LESS_THAN, CHAR, "<");
        }

        if (lexer->current_char == '>')
        {
            advance(lexer);
            return create_token(TOKEN_OPERATOR_GREATER_THAN, CHAR, ">");
        }

        if (lexer->current_char == '{')
        {
            advance(lexer);
            return create_token(TOKEN_LBRACE, CHAR, "{");
        }

        if (lexer->current_char == '}')
        {
            advance(lexer);
            return create_token(TOKEN_RBRACE, CHAR, "}");
        }

        printf("\nError : Invalid character : %c\n", lexer->current_char);
        exit(EXIT_FAILURE);
    }
    return create_token(TOKEN_EOF, INT, "-1");
}

/**
 *
 * Frees the memory allocated for a Token, including its value if it is a string.
 *
 * @param token - The Token to free.
 */

void free_token(Token *token)
{
    if (token == NULL)
        return;
    if (token->valueType == STRING && token->value.strValue != NULL)
    {
        free(token->value.strValue);
        token->value.strValue = NULL;
    }
    free(token);
    token = NULL;
}
