//
//
//

#include "abstract_syntax_tree.h"
#include "lexer.h"
#include "parser.h"

#ifndef ZLANG_INTERPRETER_H
#define ZLANG_INTERPRETER_H

typedef struct GLOBAL_SCOPE GLOBAL_SCOPE;

typedef struct{
    Parser * parser;
    GLOBAL_SCOPE * global_scope;
} Interpreter;


typedef union{
    int intValue;
    long longValue;
    float floatValue;
    double doubleValue;
    char charValue;
    char * stringValue;
} VariableValue;

typedef struct{
    VariableNode * variableNode;
    VariableValue value;
} VariableScope;

struct GLOBAL_SCOPE {
    unsigned short capacity;
    unsigned short size;
    VariableScope ** variables;
};


Parser * create_parser(Lexer * lexer);
//Interpreter * create_interpreter(Parser * parser);
Interpreter * create_interpreter(Parser * parser, GLOBAL_SCOPE * global_scope);
void consume_token(Parser * parser, TokenType tokenType);
ASTNode * factor(Parser * parser);
ASTNode * term(Parser * parser);
ASTNode * expr(Parser * parser);
//int interpret(ASTNode * node);
int interpret(Interpreter * interpreter, ASTNode * node);
void free_interpreter(Interpreter * interpreter);
int visit_number_node( Interpreter * interpreter, ASTNode * node);
void visit_assign_node( Interpreter * interpreter, ASTNode *node);
int visit_var_node( Interpreter * interpreter, ASTNode *node);
int visit_bin_op_node( Interpreter * interpreter, ASTNode * node);
int visit_unary_op_node( Interpreter * interpreter, ASTNode *node);
int visit_node( Interpreter * interpreter,ASTNode * node);

//int display_AST_RPN( Interpreter * interpreter, ASTNode * node);

GLOBAL_SCOPE * init_global_scope(unsigned short initialCapacity);
VariableScope * find_variable_in_global_scope(GLOBAL_SCOPE * globalScope, const char * varName);
void add_variable_to_global_scope(GLOBAL_SCOPE * globalScope, VariableScope * varScope);
void set_variable_in_global_scope(GLOBAL_SCOPE * global_scope, VariableScope * var_scope);
void display_global_scope_variables(GLOBAL_SCOPE * global_scope);
void free_global_scope(GLOBAL_SCOPE * globalScope);




#endif //ZLANG_INTERPRETER_H
