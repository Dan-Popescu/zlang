//
// Created by Charara on 10/20/2024.
//

#include "parser.h"
#include <stdlib.h>


/**
 * Create a new parser object
 * @param lexer : Lexer object
 * @return : Parser object
 *
 * This function allocates memory for a new parser object and initializes it with the lexer object
 * the current_token is initialized with a call to get_next_token(lexer)
 *
 */
Parser *create_parser(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
    if (parser == NULL) {
        fprintf(stderr, "Memory allocation failed when trying to create new parser.\n");
        exit(EXIT_FAILURE);
    }
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}


/**
 * Free memory allocated for the parser object
 * @param parser : Parser object
 *
 * This function frees memory allocated for the parser object
 * It also frees memory allocated for the lexer object and the current token
 */
void free_parser(Parser *parser) {
    if (parser == NULL) return;
    // free the lexer
    if (parser->lexer != NULL) free_lexer(parser->lexer);
    // free current token
    if (parser->current_token != NULL) free_token(parser->current_token);
    free(parser);
    parser = NULL;
}


/**
 * Consume the current token if it matches the expected token type
 * @param parser : Parser object
 * @param tokenType : TokenType
 *
 * This function consumes the current token if it matches the expected token type.
 * The consumption involves replacing current_token field with a new call to get_next_token(lexer).
 * If the current token does not match the expected token type, the function prints an error message
 * and exits the program
 */
void consume_token(Parser *parser, const TokenType tokenType) {
    if (parser->current_token->type == tokenType) {
        free_token(parser->current_token); // was free(parser->current_token);
        parser->current_token = get_next_token(parser->lexer);
    } {
        printf("\nError. Invalid syntax.\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * Parse a factor node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a factor node based on the following grammar rule:
 * factor (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN | variable
 *
 *
 */
ASTNode *factor(Parser *parser) {
    const TokenType tokenType = parser->current_token->type;

    if (tokenType == TOKEN_OPERATOR_PLUS) {
        consume_token(parser, TOKEN_OPERATOR_PLUS);
        ASTNode *expression = factor(parser);
        ASTNode *node = create_unary_operator_node(parser->current_token, expression);
        return node;
    }
    if (tokenType == TOKEN_OPERATOR_MINUS) {
        consume_token(parser, TOKEN_OPERATOR_MINUS);
        ASTNode *expression = factor(parser);
        ASTNode *node = create_unary_operator_node(parser->current_token, expression);
        return node;
    }
    if (tokenType == TOKEN_NUMBER) {
        const int value = parser->current_token->value.intValue;
        consume_token(parser, TOKEN_NUMBER);
        ASTNode *node = create_number_node(TOKEN_NUMBER, INT, value);
        return node;
    }

    if (tokenType == TOKEN_LPAREN) {
        consume_token(parser, TOKEN_LPAREN);
        ASTNode *result = expr(parser);
        consume_token(parser, TOKEN_RPAREN);
        return result;
    }
    if (tokenType == TOKEN_IDENTIFIER) {
        ASTNode *varNode = variable(parser);
        return varNode;
    }
    fprintf(stderr, "No factor could be parsed based on token type of value : %d", tokenType);

    exit(EXIT_FAILURE);
}


/**
 * Parse a term node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a term node based on the following grammar rule:
 * term : factor ((MUL | DIV) factor)*
 *
 */
ASTNode *term(Parser *parser) {
    ASTNode *left = factor(parser);

    while (parser->current_token->type == TOKEN_OPERATOR_MULT ||
           parser->current_token->type == TOKEN_OPERATOR_DIV) {
        // create new token to avoid value mutation issues (make sure the copy of the value is not changed
        // by changes made to the current token stored in the parser)
        Token *token = malloc(sizeof(Token));
        token->type = parser->current_token->type;
        token->value = parser->current_token->value;
        token->valueType = parser->current_token->valueType;

        if (token->type == TOKEN_OPERATOR_MULT) {
            consume_token(parser, TOKEN_OPERATOR_MULT);
        } else if (token->type == TOKEN_OPERATOR_DIV) {
            consume_token(parser, TOKEN_OPERATOR_DIV);
        }
        ASTNode *right = factor(parser);
        ASTNode *node = create_binary_operator_node(token, left, right);
        // the result of this operation becomes the new left node
        left = node;
    }
    return left;
}


/**
 * Parse an expression node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses an expression node based on the following grammar rule:
 * expr : term ((PLUS | MINUS) term)*
 *
 */
ASTNode *expr(Parser *parser) {
    ASTNode *left = term(parser);

    while (parser->current_token->type == TOKEN_OPERATOR_PLUS ||
           parser->current_token->type == TOKEN_OPERATOR_MINUS) {
        // create deep copy of the current token from the parser
        Token *token = malloc(sizeof(Token));
        token->type = parser->current_token->type;
        token->value = parser->current_token->value;
        token->valueType = parser->current_token->valueType;

        if (token->type == TOKEN_OPERATOR_PLUS) {
            consume_token(parser, TOKEN_OPERATOR_PLUS);
        } else if (token->type == TOKEN_OPERATOR_MINUS) {
            consume_token(parser, TOKEN_OPERATOR_MINUS);
        }

        ASTNode *right = term(parser);
        ASTNode *node = create_binary_operator_node(token, left, right);
        //the newly created node become the new left node
        left = node;
    }

    return left;
}


/**
 * Parse a variable node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a variable node based on the following grammar rule:
 * variable : IDENTIFIER
 *
 */
ASTNode *variable(Parser *parser) {
    // make a deep copy of current token from the parser to avoid mutation during parsing (consume_token)
    Token *currToken = malloc(sizeof(Token));
    currToken->type = parser->current_token->type;
    currToken->valueType = parser->current_token->valueType;
    currToken->value = parser->current_token->value;

    ASTNode *varNode = create_variable_node(currToken);
    consume_token(parser, TOKEN_IDENTIFIER);

    return varNode;
}


/**
 * Parse an assignment statement node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses an assignment statement node based on the following grammar rule:
 * assignment_statement : VARIABLE ASSIGNMENT_OPERATOR expr
 *
 */
ASTNode *assignment_statement(Parser *parser) {
    // create node with the identifier of the variable
    ASTNode *left = variable(parser);

    // make a deep copy of current token from the parser to avoid mutation during parsing (consume_token)
    Token *token = malloc(sizeof(Token));
    token->type = parser->current_token->type;
    token->valueType = parser->current_token->valueType;
    token->value = parser->current_token->value;

    // consume the token assignment operator
    consume_token(parser, TOKEN_OPERATOR_ASSIGNMENT);

    // crate node with the expression assigned to the variable (identifier)
    ASTNode *right = expr(parser);
    ASTNode *assignmentNode = create_assignment_node(left, token, right);

    //    consume_token(parser, TOKEN_SEMI_COLON);

    return assignmentNode;
}


/**
 * Parse a print statement node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a print statement node based on the following grammar rule:
 * print_statement : PRINT expr
 *
 */
ASTNode *print_statement(Parser *parser) {
    consume_token(parser, TOKEN_KEYWORD_PRINT);
    return create_print_node(expr(parser));
}


/**
 * Parse a statement node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a statement node based on the following grammar rule:
 * statement : assignment_statement | print_statement
 *
 */
ASTNode *statement(Parser *parser) {
    const TokenType tokenType = parser->current_token->type;
    //printf("Token type: %d\n", tokenType);

    if (tokenType == TOKEN_IDENTIFIER) {
        return assignment_statement(parser);
    }
    if (tokenType == TOKEN_KEYWORD_PRINT) {
        return print_statement(parser);
    }

    return create_empty_node();
}

// Helper function to allocate memory for nodes
ASTNode **allocate_nodes(unsigned short capacity) {
    ASTNode **nodes = malloc(capacity * sizeof(ASTNode *));
    for (unsigned short idx = 0; idx < capacity; ++idx) {
        nodes[idx] = NULL;
    }
    return nodes;
}


/**
 * Parse a list of statements node
 * @param parser : Parser object
 * @return : ASTNode
 *
 * This function parses a list of statements node based on the following grammar rule:
 * statements_list : statement | statement TOKEN_SEMI_COLON statements_list
 *
 */
ASTNode *statements_list(Parser *parser) {
    // find first statement
    ASTNode *stmtNode = statement(parser);


    // allocate memory for 10 statements initially
    unsigned short capacity = 10;
    unsigned short size = 1;
    ASTNode **nodes = allocate_nodes(capacity);
    nodes[0] = stmtNode;

    // identify/parse all other statements in input and store them in a list of nodes
    while (parser->current_token->type == TOKEN_SEMI_COLON) {
        // check if size of the nodes list needs to be augmented
        if (size >= capacity) {
            // double the capacity
            ASTNode **newNodes = allocate_nodes(2 * capacity);
            for (unsigned short idx = 0; idx < capacity; ++idx) {
                newNodes[idx] = nodes[idx];
            }
            free(nodes);
            nodes = newNodes;
            capacity *= 2;
        }


        // parse next statement
        ASTNode *new_stmt_node = statement(parser);
        if (new_stmt_node->type != EMPTY_NODE) {
            nodes[size] = new_stmt_node;
            ++size;
        }
        consume_token(parser, TOKEN_SEMI_COLON);
    }

    // create a single ASTNode representing the entire statements node list
    ASTNode *stmtListNode = create_statements_node_list(nodes, capacity, size);

    return stmtListNode;
}
