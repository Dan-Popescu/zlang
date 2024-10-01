//
// Created by Popescu Dan on 29/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"




//ValueType get_token_value_type(const Token * token){
//    char * value = token->value;
//
//    ValueType * valueType = malloc(sizeof(ValueType));
//
//    // check if it starts with a digit
//    if(isdigit(value[0])){
//        // loop over value characters until end and check if a . separator is present
//        unsigned char idx = 1;
//        char c = value[idx];
//        unsigned char floatingFlag = 0;
//        while(c!= '\0'){
//            if(floatingFlag == 1 && isdigit(c)){
//                floatingFlag++;
//            }
//            if(c=='.' && idx > 0){
//                // value is potentially a float or double value
//                // next character must also be a digit in order to confirm
//                // that the value is a floating value
//                floatingFlag = 1;
//            }
//            ++idx;
//            c = value[idx];
//
//            if(floatingFlag == 2) return DOUBLE;
//        }
//        if(floatingFlag == 0) return LONG;
//    }else if(isalpha(value[0])){
//        // check if it's a reserved keyword
//
//        // if not, then it's an identifier
//        return TOKEN_IDENTIFIER;
//    }
//
//    return INT;
//
//}
