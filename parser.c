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
    if (parser == NULL) {
        fprintf(stderr, "Memory allocation failed when trying to create new parser.\n");
        exit(EXIT_FAILURE);
    }
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}

void free_parser(Parser * parser){
    if(parser == NULL) return;
    // free the lexer
    if(parser->lexer != NULL) free_lexer(parser->lexer);
    // free current token
    if(parser->current_token != NULL) free_token(parser->current_token);
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
 * factor (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN | variable
 *
 * @param parser
 * @return
 */
ASTNode * factor(Parser * parser){

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
//        int value = token->value;
        int value = token->value.intValue;
        consume_token(parser, TOKEN_NUMBER);
        ASTNode * node = create_number_node(TOKEN_NUMBER, INT, value);
        return node;
    }else if(token->type == TOKEN_LPAREN){
        consume_token(parser, TOKEN_LPAREN);
        ASTNode * result = expr(parser);
        consume_token(parser, TOKEN_RPAREN);
        return result;
    }else if(token->type == TOKEN_IDENTIFIER){
        ASTNode * varNode = variable(parser);
        return varNode;
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



/**
 *
 * variable : TOKEN_IDENTIFIER
 *
 * @param parser
 * @return
 */

ASTNode * variable(Parser * parser){

    // make a deep copy of current token from the parser to avoid mutation during parsing (consume_token)
    Token * currToken = malloc(sizeof(Token));
    currToken->type = parser->current_token->type;
    currToken->valueType = parser->current_token->valueType;
    currToken->value = parser->current_token->value;

    ASTNode * varNode = create_variable_node(currToken);
    consume_token(parser, TOKEN_IDENTIFIER);

    return varNode;
}

/**
 *
 * assignment_statement : VARIABLE TOKEN_ASSIGNMENT_OPERATOR expr
 *
 * @param parser
 * @return
 */
ASTNode * assignment_statement(Parser * parser){

    // create node with the identifier of the variable
    ASTNode * left = variable(parser);

    // make a deep copy of current token from the parser to avoid mutation during parsing (consume_token)
    Token * token = malloc(sizeof(Token));
    token->type = parser->current_token->type;
    token->valueType = parser->current_token->valueType;
    token->value = parser->current_token->value;

    // consume the token assignment operator
    consume_token(parser, TOKEN_OPERATOR_ASSIGNMENT);

    // crate node with the expression assigned to the variable (identifier)
    ASTNode * right = expr(parser);
    ASTNode * assignmentNode =  create_assignment_node(left, token, right);

    return assignmentNode;
}

/**
 *
 * statement : assignment_statement |
 *
 *
 * @param parser
 * @return
 */
ASTNode * statement(Parser * parser){
//    @TODO complete later when we will have other kinds of statements
    Token * currToken = parser->current_token;
    if(currToken->type == TOKEN_IDENTIFIER){
        ASTNode * assignmentNode = assignment_statement(parser);
        return assignmentNode;
    }else{
        ASTNode * emptyNode = create_empty_node();
        return emptyNode;
    }
}
