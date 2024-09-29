//
// Created by popes on 29/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

Token * get_next_token(char *statement){
    /*
     * This function performs the role of a lexical analyser / lexer / tokenizer
     * It's responsible for breaking a statement into tokens, one character at a time
    */

    printf("\n statement received in lexer : %s", statement);
//    printf("\n *statement is : %c", *statement);

    char * charPtr = statement;

//    while(*charPtr== ' ') (charPtr)++; // Ignore white space

    Token * token = malloc(sizeof(Token));
    token->value = calloc(64, sizeof(char));

    if(isdigit(*charPtr)){
        // If it's a number
        token->type = TOKEN_NUMBER;
        int idx = 0;
        while(isdigit(*charPtr)){
            token->value[idx] = *charPtr;
            ++idx;
            charPtr++;
        }
        token->value[idx]='\0';
    }else if(*statement == '+' ||
             *statement == '-' ||
             *statement == '*' ||
             *statement == '/' ||
             *statement == '^'){

        // if it's an operator
        token->type = TOKEN_OPERATOR;
        token->value[0] = *statement;
        token->value[1] = '\0';
    }

    return token;
}


Token * evaluate_expr(char * expression){
    char * exprPtr = expression;
    Token * left = get_next_token(exprPtr);
    printf("\nleft operand token value is : %s", left->value);
    exprPtr++;
    Token * op = get_next_token(exprPtr);
    printf("\noperator token value is : %s", op->value);
    exprPtr++;
    Token * right = get_next_token(exprPtr);
    printf("\nright token value is : %s", right->value);

    Token *exprResult = malloc(sizeof(Token));

    if(left->type==TOKEN_NUMBER && op->type==TOKEN_OPERATOR && right->type==TOKEN_NUMBER){
        double lVal = (atof)(left->value);
        char opVal = (char)(*op->value);
        double rVal = (atof)(right->value);
        printf("\n lVal is : %lf", lVal);
        printf("\n opVal is : %c", opVal);
        printf("\n rVal is : %lf", rVal);
        printf("\n lVal %c rVal is : %lf", opVal, lVal + rVal);
        if(opVal=='+'){
            exprResult->type = TOKEN_NUMBER;
            exprResult->value = calloc(64, sizeof(char));
            sprintf(exprResult->value, "%lf" , lVal + rVal);
        }
    }


    return exprResult;
}
