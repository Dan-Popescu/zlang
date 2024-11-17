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
    STATEMENTS_LIST_NODE
} NodeType;

typedef struct NumberNode NumberNode;
typedef struct BinaryOpNode BinaryOpNode;
typedef struct UnaryOpNode UnaryOpNode;
typedef struct VariableNode VariableNode;
typedef struct AssignOpNode AssignOpNode;
typedef struct PrintNode PrintNode;
typedef struct StatementsListNode StatementsListNode;
typedef struct EmptyNode EmptyNode;
typedef struct ASTNode ASTNode;

typedef union{
    NumberNode * numNode;
    BinaryOpNode * binaryOpNode;
    UnaryOpNode * unaryOpNode;
    VariableNode * variableNode;
    AssignOpNode * assignOpNode;
    PrintNode * printNode;
    StatementsListNode * stmtListNode;
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

struct StatementsListNode{
    unsigned short capacity;
    unsigned short size;
    ASTNode ** nodes;
};

struct PrintNode{
    ASTNode * expression;
};

struct EmptyNode{};


ASTNode * create_number_node(TokenType tokenType, ValueType valueType, int value);
ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right );
ASTNode * create_unary_operator_node(Token * token, ASTNode * expression);
ASTNode * create_variable_node(Token * varToken);
ASTNode * create_assignment_node(ASTNode * left, Token * assignmentToken, ASTNode * right);
ASTNode * create_print_node(ASTNode * exprNode);
ASTNode * create_statements_node_list(ASTNode ** nodes, unsigned short capacity, unsigned short size);
ASTNode * create_empty_node();
void free_node(ASTNode * node);
void free_statements_list_node(ASTNode * node);
void free_assignment_node(ASTNode * node);
void free_number_node(ASTNode * node);
void free_variable_node(ASTNode * node);
void free_print_node(ASTNode * node);
void free_unary_operator_node(ASTNode * node);
void free_binary_operator_node(ASTNode * node);
void free_empty_node(ASTNode * node);

#endif //ZLANG_ABSTRACT_SYNTAX_TREE_H
