//
// Created by popes on 01/10/2024.
//

#include "abstract_syntax_tree.h"
#include <stdlib.h>

ASTNode * create_number_node(TokenType tokenType, ValueType valueType, int value){
    NumberNode * numNode = malloc(sizeof(NumberNode));
    numNode->token = malloc(sizeof(Token));
    numNode->token->type = tokenType;
    numNode->token->valueType = valueType;
//    numNode->token->value = value;
    numNode->token->value.intValue = value;
    numNode->value = value;

    ASTNode * node = malloc(sizeof(ASTNode));
    node->type = NUMBER_NODE;
    node->node = malloc(sizeof(NumberNode));
    node->node->numNode = numNode;

    return node;
}

ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right ){
    BinaryOpNode * binOpNode = malloc(sizeof(BinaryOpNode));
    binOpNode->left = left;
    binOpNode->operator = opToken;
    binOpNode->right = right;

    ASTNode * node = malloc(sizeof(ASTNode));
    node->type = BINARY_OPERATOR_NODE;
    node->node = malloc(sizeof binOpNode);
    node->node->binaryOpNode = binOpNode;
    return node;
}

ASTNode * create_unary_operator_node(Token * token, ASTNode * expression){
    UnaryOpNode * unaryOpNode = malloc(sizeof(UnaryOpNode));
    unaryOpNode->operator = token;
    unaryOpNode->expression = expression;

    ASTNode * node = malloc(sizeof(ASTNode));
    node->type = UNARY_OPERATOR_NODE;
    node->node = malloc(sizeof(UnaryOpNode));
    node->node->unaryOpNode = unaryOpNode;
    return node;
}
