//
// Created by popes on 01/10/2024.
//

#include "lexer.h"


#ifndef ZLANG_ABSTRACT_SYNTAX_TREE_H
#define ZLANG_ABSTRACT_SYNTAX_TREE_H


typedef struct{

} AST;

typedef struct{
    Token token;
    void * value;
} NumberNode;

typedef struct{
    NumberNode left;
    Token operator;
    NumberNode right;
} BinaryOp;

#endif //ZLANG_ABSTRACT_SYNTAX_TREE_H
