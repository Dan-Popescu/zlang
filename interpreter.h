//
//
//

#include "abstract_syntax_tree.h"

#ifndef ZLANG_INTERPRETER_H
#define ZLANG_INTERPRETER_H

#include "lexer.h"

//typedef struct{
//    Lexer * lexer;
//    Token * current_token;
//} Interpreter;

typedef struct{
    Lexer * lexer;
    Token * current_token;
} Parser;


typedef struct{
    Parser * parser;
} Interpreter;

Parser * create_parser(Lexer * lexer);
Interpreter * create_interpreter(Parser * parser);
void consume_token(Parser * parser, TokenType tokenType);
ASTNode * factor(Parser * parser);
ASTNode * term(Parser * parser);
ASTNode * expr(Parser * parser);
int interpret(ASTNode * node);



#endif //ZLANG_INTERPRETER_H
