//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "abstract_syntax_tree.h"

Parser * create_parser(Lexer * lexer){
    Parser * parser = (Parser *)malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}


void consume_token(Parser * parser, TokenType tokenType){
    if(parser->current_token->type == tokenType){
        free(parser->current_token);
        parser->current_token = get_next_token(parser->lexer);
    }else{
        printf("\nError. Invalid syntax.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
// * factor : NUMBER | LPAREN expr RPAREN
 * factor (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN
 *
 * @param parser
 * @return
 */
ASTNode * factor(Parser * parser){

//    Token * token = parser->current_token;
    Token * token = malloc(sizeof(Token));
    token->type = parser->current_token->type;
    token->value = parser->current_token->value;
    token->valueType = parser->current_token->valueType;

    if(token->type == TOKEN_OPERATOR_PLUS){
        consume_token(parser, TOKEN_OPERATOR_PLUS);
        ASTNode * expression = factor(parser);
        ASTNode * node = create_unary_operator_node(token, expression);
        return node;
    }if(token->type == TOKEN_OPERATOR_MINUS){
        consume_token(parser, TOKEN_OPERATOR_MINUS);
        ASTNode * expression = factor(parser);
        ASTNode * node = create_unary_operator_node(token, expression);
        return node;
    }else if(token->type == TOKEN_NUMBER){
        int value = token->value;
        consume_token(parser, TOKEN_NUMBER);
        ASTNode * node = create_number_node(TOKEN_NUMBER, INT, value);
        return node;
    }else if(token->type == TOKEN_LPAREN){
        consume_token(parser, TOKEN_LPAREN);
        ASTNode * result = expr(parser);
        consume_token(parser, TOKEN_RPAREN);
        return result;
    }
}

/**
 *
 * term : factor ( (MULT | DIV) * factor)*
 *
 * @param parser
 * @return
 */

ASTNode * term(Parser * parser){
    ASTNode * left = factor(parser);

    while(parser->current_token->type == TOKEN_OPERATOR_MULT ||
          parser->current_token->type == TOKEN_OPERATOR_DIV){

        // create new token to avoid value mutation issues (make sure the copy of the value is not changed
        // by changes made to the current token stored in the parser)
        Token * token = malloc(sizeof(Token));
        token->type = parser->current_token->type;
        token->value = parser->current_token->value;
        token->valueType = parser->current_token->valueType;

        if(token->type == TOKEN_OPERATOR_MULT){
            consume_token(parser, TOKEN_OPERATOR_MULT);
        }else if(token->type == TOKEN_OPERATOR_DIV){
            consume_token(parser, TOKEN_OPERATOR_DIV);
        }
        ASTNode * right = factor(parser);
        ASTNode * node = create_binary_operator_node(token, left, right);
        // the result of this operation becomes the new left node
        left = node;
    }
    return left;
}

/**
 *
 *
 * expr : term ( (PLUS | MINUS) term)*
 * term : factor ( (MUL | DIV) * factor)*
 * factor : NUMBER | LPAREN expr RPAREN
 *
 * @param parser
 * @return
 */

ASTNode *  expr(Parser * parser){
    ASTNode * left = term(parser);

    while(parser->current_token->type == TOKEN_OPERATOR_PLUS ||
          parser->current_token->type == TOKEN_OPERATOR_MINUS){

        // create deep copy of the current token from the parser
        Token * token = malloc(sizeof(Token));
        token->type = parser->current_token->type;
        token->value = parser->current_token->value;
        token->valueType = parser->current_token->valueType;

        if(token->type == TOKEN_OPERATOR_PLUS){
            consume_token(parser, TOKEN_OPERATOR_PLUS);
        }else if(token->type == TOKEN_OPERATOR_MINUS){
            consume_token(parser, TOKEN_OPERATOR_MINUS);
        }

        ASTNode * right = term(parser);
        ASTNode * node = create_binary_operator_node(token, left, right);
        //the newly created node become the new left node
        left = node;
    }

    return left;
}


Token * identifier(Parser * parser){
    unsigned short capacity = 10;
    char * result = calloc(capacity, sizeof(char));
    long index = 0;
    while(parser->lexer->current_char != ' ' && isalpha(parser->lexer->current_char)){
        char currChar = parser->lexer->current_char;
        long size = strlen(result);
        if(size >= index - 1){
            char * newResult = calloc(capacity * 2, sizeof(char));
            for(int i = 0; i < capacity; ++i){
                newResult[i] = result[i];
            }
            free(result);
            result = newResult;
        }
        strncpy(result, &currChar , 1);
        advance(parser->lexer);
    }

    Token * token = malloc(sizeof(Token));
    token->type = TOKEN_IDENTIFIER;
    token->valueType = STRING;


}