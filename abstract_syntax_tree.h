//
// Created by popes on 01/10/2024.
//

#include "lexer.h"


#ifndef ZLANG_ABSTRACT_SYNTAX_TREE_H
#define ZLANG_ABSTRACT_SYNTAX_TREE_H

typedef enum {
    NUMBER_NODE,
    BINARY_OPERATOR_NODE
} NodeType;

typedef struct NumberNode NumberNode;
typedef struct BinaryOpNode BinaryOpNode;
typedef struct ASTNode ASTNode;

typedef union{
    NumberNode * numNode;
    BinaryOpNode * binaryOpNode;
} NodeUnion;

struct ASTNode {
    NodeType type;
    NodeUnion * node;
};

struct NumberNode {
    Token * token;
    int value;
};

struct BinaryOpNode{
    ASTNode * left;
    Token * operator;
    ASTNode * right;
};


ASTNode * create_number_node(TokenType tokenType, ValueType valueType, int value);
ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right );

#endif //ZLANG_ABSTRACT_SYNTAX_TREE_H
