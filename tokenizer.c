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

    printf("\n statement received in lexer : %s", *statement);
//    printf("\n *statement is : %c", **statement);

    while(**statement== ' ') (*statement)++; // Ignore white space

    Token * token = malloc(sizeof(Token));
    token->value = calloc(64, sizeof(char));

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
    printf("\n Token type required : %u", tokenType);
    Token * token = get_next_token(expression);
    printf("\n Token value : %s", token->value);
    if(token->type == tokenType){
    }else{
        printf("\nError parsing input");
        return NULL;
    }
    return token;
}

/**
 *
 * @param expression is a pointer to an expression/statement string
 * @return
 */

Token * term(char **expression){
    // checks if token is of a type allowed for an operand
    // if it is, the token is returned and pointer increment to point to next token
    // else it returns NULL
    Token * token = consume_token(expression, TOKEN_NUMBER);
    return token;
}


ExpressionValue * eval_expr(char ** expression){

    // Put the first element in result value
    // Get the first term @TODO to modify later, since expression can start with parenthesis as well
    Token * currToken = term(expression);
    if(currToken == NULL){
        printf("\nError parsing input");
    }

    // Allocate memory for expression value to be returned
    ExpressionValue * resultValue = malloc(sizeof(ExpressionValue));
    resultValue->value = calloc(64, sizeof(char));

    // Initialize value in expression to be returned to the value of the first term token
    strcpy(resultValue->value, currToken->value);
    resultValue->valueType = LONG;

    // printf for checking
    printf("\n First result value is : %s", resultValue->value);

    // while there are term tokens (as represented by tokens of type TOKEN_NUMBER), keep performing operations
    // and updating the result value
    while(currToken->type == TOKEN_NUMBER){
        currToken = get_next_token(expression);

        // if token is one of the tokens indicating end of instruction then return value;
        if(currToken->type == TOKEN_INSTRUCTION_END){
            return resultValue;
        }

        // else check that next token following a term token is the addition + operator and perform the operation if
        // it's the case
        if(currToken->type==TOKEN_OPERATOR && strcmp(currToken->value, "+") == 0){
            currToken = term(expression);
            // if there are no term tokens following the operator, then an error is thrown/displayed and program ends
            if(currToken==NULL){
                printf("\nError parsing input");
                return NULL;
            }
            addNextOperand(resultValue, currToken, LONG);
            printf("\n result value : %s", resultValue->value);
        }
        // else check if the next token following a term token is the minus - operator and perform the operation if
        // it's the case
        else if(currToken->type==TOKEN_OPERATOR && strcmp(currToken->value, "-") == 0){
            currToken = term(expression);
            // if there are no term tokens following the operator, then an error is thrown/displayed and program ends
            if(currToken==NULL){
                printf("\nError parsing input");
                return NULL;
            }
            subtractNextOperand(resultValue, currToken, LONG);
            printf("\n result value : %s", resultValue->value);
        }
    }

    // If we got here, then no end of statement was found => an error should be displayed
    free(resultValue->value);
    free(resultValue);
    fprintf(stderr, "\nNo end of statement found. You forgot the the semi-colon at the end of your "
                    "statement.");
    return NULL;
}


void addNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(nextTokenOperand->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 171 : Conversion error from string to long");
        }
        long newVal = prevVal + currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(nextTokenOperand->value, NULL);
        fprintf(stderr, "\ntokenizer.c Line 178 : Conversion error from string to double");
        long newVal = prevVal + currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}

void subtractNextOperand(ExpressionValue * resultValue, Token * nextTokenOperand, ValueType returnType){
    if(returnType == LONG || returnType == INT){
        long prevVal = (strtol)(resultValue->value, NULL, 10);
        long currVal = (strtol)(nextTokenOperand->value, NULL, 10);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 189 : Conversion error from string to long");
        }
        long newVal = prevVal - currVal;
        sprintf(resultValue->value, "%ld", newVal);
    }else if(returnType == DOUBLE || returnType == FLOAT){
        long prevVal = (strtod)(resultValue->value, NULL);
        long currVal = (strtod)(nextTokenOperand->value, NULL);
        if(prevVal == 0 || currVal == 0){
            fprintf(stderr, "\ntokenizer.c Line 197 : Conversion error from string to double");
        }
        long newVal = prevVal - currVal;
        sprintf(resultValue->value, "%lf", newVal);
    }
}




//unsigned char is_operator(const Token * token){
//    char * value = token->value;
//    switch(*value){
//        case '+':
//            return 1;
//        case '-':
//            return 1;
//        case '/':
//            return 1;
//        case '*':
//            return 1;
//    }
//    if( *value='+' )
//}





ExpressionValue * evaluate_expr(char ** expression){
    char ** exprPtr = expression;
    Token * left = get_next_token(exprPtr);
    printf("\nleft operand token value is : %s", left->value);
    Token * op = get_next_token(exprPtr);
    printf("\noperator token value is : %s", op->value);
    Token * right = get_next_token(exprPtr);
    printf("\nright token value is : %s", right->value);

//    Token *exprResult = malloc(sizeof(Token));
    ExpressionValue  *exprResult = malloc(sizeof(ExpressionValue));


    unsigned char lValType = get_token_value_type(left);
    unsigned char opValType = get_token_value_type(op);
    unsigned char rValType = get_token_value_type(right);

    if(left->type==TOKEN_NUMBER && op->type==TOKEN_OPERATOR && right->type==TOKEN_NUMBER){

        if(lValType == LONG && rValType==LONG){
            printf("\nlValType == LONG && rValType==LONG");
            long lVal = (atoi)(left->value);
            char opVal = (char)(*op->value);
            long rVal = (atoi)(right->value);
            printf("\n lVal is : %ld", lVal);
            printf("\n opVal is : %c", opVal);
            printf("\n rVal is : %ld", rVal);
            printf("\n lVal %c rVal is : %ld", opVal, lVal + rVal);
            if(opVal=='+'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = LONG;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%ld" , lVal + rVal);
            }else if(opVal=='-'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = LONG;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%ld" , lVal - rVal);
            }
            else if(opVal=='*'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = LONG;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%ld" , lVal * rVal);
            }
            else if(opVal=='/'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = LONG;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%ld" , lVal / rVal);
            }

//            printf("\n lVal %c rVal is : %ld", opVal, (atof)(exprResult->value));

        }else if(lValType == DOUBLE && rValType && DOUBLE){

            double lVal = (atof)(left->value);
            char opVal = (char)(*op->value);
            double rVal = (atof)(right->value);
            printf("\n lVal is : %lf", lVal);
            printf("\n opVal is : %c", opVal);
            printf("\n rVal is : %lf", rVal);
            if(opVal=='+'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = DOUBLE;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%lf" , lVal + rVal);
            }else if(opVal=='-'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = DOUBLE;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%lf" , lVal - rVal);
            }
            else if(opVal=='*'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = DOUBLE;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%lf" , lVal * rVal);
            }
            else if(opVal=='/'){
//                exprResult->type = TOKEN_NUMBER;
                exprResult->valueType = DOUBLE;
                exprResult->value = calloc(64, sizeof(char));
                sprintf(exprResult->value, "%lf" , lVal / rVal);
            }

//            printf("\n lVal %c rVal is : %lf", opVal, exprResult->value);
        }
    }



    return exprResult;
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
