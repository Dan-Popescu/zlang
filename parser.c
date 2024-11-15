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
    free(parser);
    parser = NULL;
}

/**
 * Helper function that allows to check that current token is of the expected type based on language
 * grammar rule
 * This function that checks whether the current token is of the expected type
 * If the token is of the expected type, it updates the parser to hold the next type,
 * otherwise an error message is printed and the program exits
 *
 * @param parser
 * @param tokenType
 */

//void consume_token(Parser * parser, TokenType tokenType){
//    if(parser->current_token->type == tokenType){
////        free(parser->current_token);
////        free_token(parser->current_token);
////        parser->current_token = malloc(sizeof(Token));
//        parser->current_token = get_next_token(parser->lexer);
//    }else{
//        printf("\nError. Invalid syntax.\n");
//        exit(EXIT_FAILURE);
//    }
//}


// V2
//void consume_token(Parser * parser, TokenType tokenType){
//    if(parser->current_token->type == tokenType){
//
////        if(parser->current_token->value.strValue != NULL){
////            free(parser->current_token->value.strValue);
////            parser->current_token->value.strValue = NULL;
////        }
//        free(parser->current_token);
//        parser->current_token = get_next_token(parser->lexer);
//    }else{
//        printf("\nError. Invalid syntax.\n");
//        exit(EXIT_FAILURE);
//    }
//}

void consume_token(Parser * parser, TokenType tokenType){
    if(parser->current_token->type == tokenType){
          if(parser->current_token->value.strValue != NULL){
            free(parser->current_token->value.strValue);
            parser->current_token->value.strValue = NULL;
        }
        free(parser->current_token);
        Token * next_token = get_next_token(parser->lexer);
        parser->current_token = next_token;
    }else{
        printf("\nError. Invalid syntax.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
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
    }else{
        fprintf(stderr, "No factor could be parsed based on token type of value : %d", token->type);
        exit(EXIT_FAILURE);
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
 * factor : (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN | variable
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
//    currToken->value.strValue = parser->current_token->value.strValue;
    currToken->value.strValue = calloc(strlen(parser->current_token->value.strValue)+1,
                                       sizeof(char));
    strcpy(currToken->value.strValue, parser->current_token->value.strValue);

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

//    consume_token(parser, TOKEN_SEMI_COLON);

    return assignmentNode;
}

ASTNode * print_statement(Parser * parser){
    consume_token(parser, TOKEN_KEYWORD_PRINT);
    ASTNode * exprNode = expr(parser);
    ASTNode * printNode = create_print_node(exprNode);
    consume_token(parser, TOKEN_SEMI_COLON);

    return printNode;
//    return exprNode;

}

/**
 *
 * statement : assignment_statement | print_statement
 *
 *
 * @param parser
 * @return
 */
ASTNode * statement(Parser * parser){
    Token * currToken = parser->current_token;
    if(currToken->type == TOKEN_IDENTIFIER){
        ASTNode * assignmentNode = assignment_statement(parser);
        return assignmentNode;
    }else if(currToken->type == TOKEN_KEYWORD_PRINT){
        ASTNode * node = print_statement(parser);
        return node;
    }else{
        ASTNode * emptyNode = create_empty_node();
        return emptyNode;
    }
}

/**
 * statements_list : statement |
 *                   statement TOKEN_SEMI_COLON statements_list
 *
 * @param parser
 * @return
 */

ASTNode * statements_list(Parser * parser){
    // find first statement
    ASTNode * stmtNode = statement(parser);

    // allocate memory for 10 statements initially
    unsigned short capacity = 10;
    unsigned short size=1;
    ASTNode ** nodes = malloc(capacity * sizeof(ASTNode *));
    nodes[0] = stmtNode;

    // identify/parser all other statements in input and store them in a list of nodes
    while(parser->current_token->type == TOKEN_SEMI_COLON){
        // check if size of the nodes list needs to be augmented
        if(size >= capacity){
            // double the capacity
            ASTNode ** newNodes = malloc(2 * capacity * sizeof(ASTNode *));
            for(unsigned short idx = 0; idx < capacity; ++idx){
                if(idx <= size){
                    newNodes[idx]=nodes[idx];
                }else if(idx > size){
                    newNodes[idx] = NULL;
                }
            }
            free(nodes);
            nodes = newNodes;
            capacity *= 2;
        }
        consume_token(parser, TOKEN_SEMI_COLON);

        // parse next statement
        ASTNode * new_stmt_node = statement(parser);
        if(new_stmt_node->type != EMPTY_NODE){
            nodes[size] = new_stmt_node;
            ++size;
        }

    }

    // create a single ASTNode representing the entire statements node list
    ASTNode * stmtListNode = create_statements_node_list(nodes, capacity, size);

    return stmtListNode;
}