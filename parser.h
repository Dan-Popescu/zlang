//
// Created by popes on 02/10/2024.
//

#include "lexer.h"
#include "abstract_syntax_tree.h"


#ifndef ZLANG_PARSER_H
#define ZLANG_PARSER_H

typedef struct{
    Lexer * lexer;
    Token * current_token;
} Parser;

Parser * create_parser(Lexer * lexer);
void consume_token(Parser * parser, TokenType tokenType);
ASTNode * factor(Parser * parser);
ASTNode * term(Parser * parser);
ASTNode *  expr(Parser * parser);


#endif //ZLANG_PARSER_H
