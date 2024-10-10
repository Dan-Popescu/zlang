//
//
//

#include "lexer.h"


#ifndef ZLANG_ABSTRACT_SYNTAX_TREE_H
#define ZLANG_ABSTRACT_SYNTAX_TREE_H

#include <stdio.h>

typedef enum {
    NUMBER_NODE,
    BINARY_OPERATOR_NODE,
    UNARY_OPERATOR_NODE,
    VARIABLE_NODE,
    ASSIGNMENT_NODE,
    EMPTY_NODE,
    PRINT_NODE,
} NodeType;

typedef struct NumberNode NumberNode;
typedef struct BinaryOpNode BinaryOpNode;
typedef struct UnaryOpNode UnaryOpNode;
typedef struct VariableNode VariableNode;
typedef struct AssignOpNode AssignOpNode;
typedef struct PrintNode PrintNode;
typedef struct EmptyNode EmptyNode;
typedef struct ASTNode ASTNode;

typedef union{
    NumberNode * numNode;
    BinaryOpNode * binaryOpNode;
    UnaryOpNode * unaryOpNode;
    VariableNode * variableNode;
    AssignOpNode * assignOpNode;
    EmptyNode * emptyNode;
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

struct UnaryOpNode{
    Token * operator;
    ASTNode * expression;
};

struct VariableNode{
    Token * varToken;
    ValueType valueType;
};

struct AssignOpNode{
    ASTNode * identifier;
    Token * assignmentToken;
    ASTNode * expression;
};

//struct PrintNode{
//    ASTNode * expression;
//    ExpressionValue value;
//};

struct EmptyNode{};


ASTNode * create_number_node(TokenType tokenType, ValueType valueType, int value);
ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right );
ASTNode * create_unary_operator_node(Token * token, ASTNode * expression);
ASTNode * create_variable_node(Token * varToken);
ASTNode * create_assignment_node(ASTNode * left, Token * assignmentToken, ASTNode * right);
ASTNode * create_empty_node();

#endif //ZLANG_ABSTRACT_SYNTAX_TREE_H
