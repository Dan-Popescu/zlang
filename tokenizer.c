//
// Created by Popescu Dan on 29/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"


/**
 * This function performs the role of a lexical analyser / lexer / tokenizer
 * It's responsible for breaking a statement into tokens, one token at a time
 * Once a token is extracted, statement pointer is updated to point to the next position
 * after the end of the token extracted
 *
 * @param statement Pointer to position in statement string, from which next token is to be extracted
 * @return Token structure of the token at position pointed by the statement pointer
 */

Token * get_next_token(char ** statement){

    printf("\n\n statement received in lexer : %s", *statement);
//    printf("\n *statement is : %c", **statement);

    while(**statement== ' ') (*statement)++; // Ignore white space

    Token * token = malloc(sizeof(Token));
    token->value = calloc(64, sizeof(char));
    token->startPtr = *statement;


    if(isdigit(**statement)){
        // If it's a number
        token->type = TOKEN_NUMBER;
        int idx = 0;
        while(isdigit(**statement)){
            token->value[idx] = **statement;
            ++idx;
            (*statement)++;
        }
        token->value[idx]='\0';
    }else if(**statement == '+' ||
             **statement == '-' ||
             **statement == '*' ||
             **statement == '/' ||
             **statement == '^'){

        // if it's an operator
        token->type = TOKEN_OPERATOR;
        token->value[0] = **statement;
        token->value[1] = '\0';
        (*statement)++;
    }else if(**statement == ';'){
        token->type = TOKEN_INSTRUCTION_END;
        token->value[0] = **statement;
        token->value[1] = '\0';
        (*statement)++;
    }

    token->endPtr = *statement;
    return token;
}


/**
 *@brief This function takes in a pointer to statement or expression string and a required token type
 * The next token in the statement/expression is extracted and checked against the required token type
 * If the required token type is satisfied then the token is returned, otherwise NULL is returned to signal
 * that there is no match
 *
 * @param expression
 * @param tokenType
 * @return Pointer to a token structure if the the token is of the required tokenTye, otherwise NULL is returned
 */
Token * consume_token(char **expression, TokenType tokenType){
    Token * token = get_next_token(expression);

    if(token->type == tokenType){
        printf("\n token value in consume_token : %s", token->value);
        return token;
    }else{
        // make expression point to the first character of the last consumed token
        // in order to be able to consume it again if needed
        *expression = token->startPtr;
        return NULL;
    }

}

/**
 *
 * A factor is any numeric value of any type : INT, LONG, FLOAT, DOUBLE
 * A factor is called as such because * and / operations have higher precedence than + and -
 *
 * @param expression is a pointer to an expression/statement string
 * @return
 */

Token * factor(char **expression){
    // checks if token is of a type allowed for an operand
    // if it is, the token is returned and pointer increment to point to next token
    // else it returns NULL
    Token * token = consume_token(expression, TOKEN_NUMBER);
    return token;
}


/**
 *
 * @param expression
 * @return
 */

ExpressionValue * term(char **expression){
    printf("\n term function called");
    Token * factorToken = factor(expression);

    if(factorToken == NULL){
        printf("\n Error parsing input. Expected number.");
        return NULL;
    }

    ExpressionValue * resultVal = malloc(sizeof(ExpressionValue));
    resultVal->value = calloc(64, sizeof(char));
    strcpy(resultVal->value ,factorToken->value);
    resultVal->valueType = LONG;

    Token * instructionEndToken = consume_token(expression, TOKEN_INSTRUCTION_END);
    if(instructionEndToken != NULL){
        return resultVal;
    }

    Token * operatorToken = consume_token(expression, TOKEN_OPERATOR);
    if(operatorToken == NULL){
        fprintf(stderr, "\nError parsing input. Expected operator but received something else.\n");
        free(resultVal->value);
        free(resultVal);
        return NULL;
    }

    if(strcmp(operatorToken->value, "*") != 0 && strcmp(operatorToken->value, "/") != 0){
//        *expression = operatorToken->startPtr;
        *expression = factorToken->endPtr;
        return resultVal;
    }


    while(operatorToken->type == TOKEN_OPERATOR &&
            (strcmp(operatorToken->value, "*") == 0 || strcmp(operatorToken->value, "/") == 0))
    {
        printf("\n looping in while");
        factorToken = consume_token(expression, TOKEN_NUMBER);
        if(factorToken == NULL){
            fprintf(stderr, "\nError parsing input. Number expected.");
            return NULL;
        }

        if(strcmp(operatorToken->value,"*") == 0){
            // perform the multiplication
            multiplyNextOperand(resultVal, factorToken, LONG);
        }else if(strcmp(operatorToken->value, "/") == 0){
            divideByNextOperand(resultVal, factorToken, LONG);
        }

        instructionEndToken = consume_token(expression, TOKEN_INSTRUCTION_END);
        if(instructionEndToken != NULL){
            return resultVal;
        }

        operatorToken = consume_token(expression, TOKEN_OPERATOR);
        if(strcmp(operatorToken->value, "*") != 0 && strcmp(operatorToken->value, "/") != 0){
            *expression = factorToken->endPtr;
            return resultVal;
        }
    }

    return resultVal;
}

ExpressionValue * eval_expr(char ** expression){

    // Put the first element in result value
    // Get the first term @TODO to modify later, since expression can start with parenthesis as well
    ExpressionValue * resultValue = term(expression);
    printf("\nLine 156 : resultValue is : %s", resultValue->value);
//    printf("\n current character in expression : %c", **expression);

    Token * instructionEndToken = consume_token(expression, TOKEN_INSTRUCTION_END);
    if(instructionEndToken != NULL && instructionEndToken->valueType==TOKEN_INSTRUCTION_END){
        return resultValue;
    }

    Token * currToken = consume_token(expression, TOKEN_OPERATOR);
    if(currToken == NULL){
        free(resultValue->value);
        free(resultValue);
    }

    // Allocate memory for storing next termValue in case there is one
    ExpressionValue * termValue = malloc(sizeof(ExpressionValue));
    termValue->value = calloc(64, sizeof(char));
    termValue->valueType = LONG;


    // while there are term tokens (as represented by tokens of type TOKEN_NUMBER), keep performing operations
    // and updating the result value
    while(currToken->type == TOKEN_OPERATOR &&
            (strcmp(currToken->value, "+") == 0 || strcmp(currToken->value, "-") == 0) )
    {
        printf("\niteration +");
        // else check that next token following a term token is the addition + operator and perform the operation if
        // it's the case
        if(currToken->type==TOKEN_OPERATOR && strcmp(currToken->value, "+") == 0){
            printf("\n current character in expression : %c", **expression);
            termValue = term(expression);
            // if there are no term tokens following the operator, then an error is thrown/displayed and program ends
            if(termValue==NULL){
                printf("\nLne 198 : Error parsing input");
                return NULL;
            }
            printf("\n term value : %s", termValue);
            addNextOperand(resultValue, termValue, LONG);
            printf("\n resultValue is : %s\n", resultValue->value);
        }
        // else check if the next token following a term token is the minus - operator and perform the operation if
        // it's the case
        else if(currToken->type==TOKEN_OPERATOR && strcmp(currToken->value, "-") == 0){
            termValue = term(expression);
            // if there are no term tokens following the operator, then an error is thrown/displayed and program ends
            if(termValue==NULL){
                printf("\nError parsing input");
                return NULL;
            }
            subtractNextOperand(resultValue, termValue, LONG);
            printf("\n result value : %s", resultValue->value);
        }
            currToken = consume_token(expression, TOKEN_OPERATOR);
            if(currToken == NULL){
                return resultValue;
            }
            if(currToken!=NULL){
                printf("\nLine 216 :  current token value : %s", currToken->value);
            }
            instructionEndToken = consume_token(expression, TOKEN_INSTRUCTION_END);
            if(instructionEndToken != NULL){
                return resultValue;
            }

    }

    // If we got here, then no end of statement was found => an error should be displayed
    free(resultValue->value);
    free(resultValue);
    fprintf(stderr, "\nNo end of statement found. You forgot the the semi-colon at the end of your "
                    "statement.");
    return NULL;
}


void addNextOperand(ExpressionValue * resultValue, ExpressionValue * termValue, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(termValue->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 239 : Conversion error from string to long");
        }
        long newVal = prevVal + currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(termValue->value, NULL);
        fprintf(stderr, "\ntokenizer.c Line 246 : Conversion error from string to double");
        long newVal = prevVal + currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}



void subtractNextOperand(ExpressionValue * resultValue, ExpressionValue * termValue, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(termValue->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 189 : Conversion error from string to long");
        }
        long newVal = prevVal - currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(termValue->value, NULL);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 197 : Conversion error from string to double");
        }
        double newVal = prevVal - currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}

void multiplyNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(nextTokenOperand->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 247 : Conversion error from string to long");
        }
        long newVal = prevVal * currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(nextTokenOperand->value, NULL);
        fprintf(stderr, "\ntokenizer.c Line 254 : Conversion error from string to double");
        double newVal = prevVal * currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}

void divideByNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(nextTokenOperand->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 247 : Conversion error from string to long");
        }
        long newVal = prevVal / currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(nextTokenOperand->value, NULL);
        fprintf(stderr, "\ntokenizer.c Line 254 : Conversion error from string to double");
        double newVal = prevVal / currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}




ValueType get_token_value_type(const Token * token){
    char * value = token->value;

    ValueType * valueType = malloc(sizeof(ValueType));

    // check if it starts with a digit
    if(isdigit(value[0])){
        // loop over value characters until end and check if a . separator is present
        unsigned char idx = 1;
        char c = value[idx];
        unsigned char floatingFlag = 0;
        while(c!= '\0'){
            if(floatingFlag == 1 && isdigit(c)){
                floatingFlag++;
            }
            if(c=='.' && idx > 0){
                // value is potentially a float or double value
                // next character must also be a digit in order to confirm
                // that the value is a floating value
                floatingFlag = 1;
            }
            ++idx;
            c = value[idx];

            if(floatingFlag == 2) return DOUBLE;
        }
        if(floatingFlag == 0) return LONG;
    }else if(isalpha(value[0])){
        // check if it's a reserved keyword

        // if not, then it's an identifier
        return TOKEN_IDENTIFIER;
    }

    return INT;

}
