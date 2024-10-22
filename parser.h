//
// Created by Charara on 10/20/2024.
//


#ifndef ZLANG_PARSER_H
#define ZLANG_PARSER_H
#include "abstract_syntax_tree.h"

typedef struct{
    Lexer * lexer;
    Token * current_token;
} Parser;

Parser * create_parser(Lexer * lexer);
void free_parser(Parser * parser);
void consume_token(Parser * parser, TokenType tokenType);
ASTNode * factor(Parser * parser);
ASTNode * term(Parser * parser);
ASTNode *  expr(Parser * parser);
ASTNode * variable(Parser * parser);
ASTNode * assignment_statement(Parser * parser);
ASTNode * statement(Parser * parser);
ASTNode * statements_list(Parser * parser);


#endif //ZLANG_PARSER_H
