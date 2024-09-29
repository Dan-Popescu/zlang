//
// Created by Dan Popescu on 28/09/2024.
//

#ifndef ZLANG_OP_S_H
#define ZLANG_OP_S_H

typedef struct {
    char operator;
    int precedence;   //
    int association; // indicates whether expression is to be applied from 1) right to left or 2) left to right
    int unary;      // 0 or 1 : indicates whether the operator is a unary operator or not

} op_s;



#endif //ZLANG_OP_S_H
