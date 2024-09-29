//
// Created by Popescu Dan on 29/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

Token * get_next_token(char ** statement){
    /*
     * This function performs the role of a lexical analyser / lexer / tokenizer
     * It's responsible for breaking a statement into tokens, one character at a time
    */

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
    }

    return token;
}


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
