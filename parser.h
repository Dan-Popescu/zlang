//
// Created by popes on 02/10/2024.
//

#include "lexer.h"
#include "abstract_syntax_tree.h"
#include <ctype.h>
#include <string.h>

#ifndef ZLANG_PARSER_H
#define ZLANG_PARSER_H

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


#endif //ZLANG_PARSER_H
