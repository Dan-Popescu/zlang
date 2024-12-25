//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "abstract_syntax_tree.h"




/**
 *
 * Creates a Parser object to analyze tokenized source code.
 *
 * @param lexer - The lexer used to provide tokens.
 * @return - A pointer to the created Parser.
 */


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



/**
 *
 * Frees the memory allocated for a Parser, including its lexer and current token.
 *
 * @param parser - The Parser to free.
 */

void free_parser(Parser * parser){
    if(parser == NULL) return;
    // free the lexer
    if(parser->lexer != NULL){
        free_lexer(parser->lexer);
        parser->lexer = NULL;
    }
    // free current token
    if(parser->current_token != NULL){
        free_token(parser->current_token);
        parser->current_token = NULL;
    }
    free(parser);
    parser = NULL;
}


/**
 *
 * Ensures the current token is of the expected type and advances to the next token.
 * If the token does not match the expected type, raises a syntax error.
 *
 * @param parser - The Parser managing tokens.
 * @param tokenType - The expected type of the current token.
 */


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
 * Parses a factor, which can be a unary operator, a number, a parenthesized expression, or a variable.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the factor.
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
        free(token);
        return node;
    }if(token->type == TOKEN_OPERATOR_MINUS){
        consume_token(parser, TOKEN_OPERATOR_MINUS);
        ASTNode * expression = factor(parser);
        ASTNode * node = create_unary_operator_node(token, expression);
        free(token);
        return node;
    }else if(token->type == TOKEN_NUMBER){
        int value = token->value.intValue;
        consume_token(parser, TOKEN_NUMBER);
        ASTNode * node = create_number_node(TOKEN_NUMBER, INT, value);
        free(token);
        return node;
    }else if(token->type == TOKEN_LPAREN){
        consume_token(parser, TOKEN_LPAREN);
        ASTNode * result = expr(parser);
        consume_token(parser, TOKEN_RPAREN);
        free(token);
        return result;
    }else if(token->type == TOKEN_IDENTIFIER){
        ASTNode * varNode = variable(parser);
        free(token);
        return varNode;
    }else{
        fprintf(stderr, "No factor could be parsed based on token type of value : %d", token->type);
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * Parses a term composed of factors connected by multiplication or division operators.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the term.
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
 * Parses an expression composed of terms connected by addition or subtraction operators,
 * or containing comparison operators (<, >, <=, >=).
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the expression.
 */


ASTNode *  expr(Parser * parser){
    ASTNode * left = term(parser);

    while(parser->current_token->type == TOKEN_OPERATOR_LESS_THAN ||
          parser->current_token->type == TOKEN_OPERATOR_GREATER_THAN) {

        // Creating a copy of the current token
        Token * token = malloc(sizeof(Token));
        token->type = parser->current_token->type;
        token->value = parser->current_token->value;
        token->valueType = parser->current_token->valueType;

        // Consume the comparison token
        if (token->type == TOKEN_OPERATOR_LESS_THAN) {
            consume_token(parser, TOKEN_OPERATOR_LESS_THAN);
        } else if (token->type == TOKEN_OPERATOR_GREATER_THAN) {
            consume_token(parser, TOKEN_OPERATOR_GREATER_THAN);
        }

        // Analyze the expression to the right of the comparator
        ASTNode * right = term(parser);

        // Create a binary node for the comparator
        ASTNode * node = create_binary_operator_node(token, left, right);

        // The new node becomes the left side for subsequent calculations
        left = node;
    }

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
 * Parses a variable (an identifier) in the source code.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the variable.
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
 * Parses an assignment statement of the form `variable = expression`.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the assignment statement.
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




/**
 *
 * Parses a `print(expression)` statement to display a value.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the `print` statement.
 */

ASTNode * print_statement(Parser * parser){

    consume_token(parser, TOKEN_KEYWORD_PRINT);
    ASTNode * exprNode = expr(parser);
    ASTNode * printNode = create_print_node(exprNode);

    return printNode;
}


/**
 *
 * Parses a generic statement: assignment, print, loop, or a block of code.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the statement.
 */

ASTNode * statement(Parser * parser) {
    
    Token * currToken = parser->current_token;

//    // Gestion des points-virgules isolés
//    if (currToken->type == TOKEN_SEMI_COLON) {
//        printf("Skipping isolated semi-colon in statement.\n");
//        consume_token(parser, TOKEN_SEMI_COLON);
////        return create_empty_node();
//        return NULL;
//    }

    if (currToken->type == TOKEN_IDENTIFIER) {
        ASTNode * assignmentNode = assignment_statement(parser);
//        consume_token(parser, TOKEN_SEMI_COLON);
        return assignmentNode;
    } else if (currToken->type == TOKEN_KEYWORD_PRINT) {
        ASTNode * node = print_statement(parser);
//        consume_token(parser, TOKEN_SEMI_COLON);
        return node;
    } else if (currToken->type == TOKEN_KEYWORD_WHILE) {
        ASTNode * whileNode = while_statement(parser);
        return whileNode;
    } else if (currToken->type == TOKEN_KEYWORD_FOR) {
        ASTNode * forNode = for_statement(parser);
        return forNode;
    } else if (currToken->type == TOKEN_LBRACE) {
        return block(parser);
    } else {
        return create_empty_node();
    }
}



/**
 *
 * Parses a list of statements separated by semicolons.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the list of statements.
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
    free(nodes);

    return stmtListNode;
}


/**
 *
 * Parses a `while(condition) { body }` loop.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the `while` loop.
 */

ASTNode *while_statement(Parser *parser) {
    // Consume the 'while' keyword
    consume_token(parser, TOKEN_KEYWORD_WHILE);

    // Consume the opening parenthesis '('
    consume_token(parser, TOKEN_LPAREN);

    // Analyze the loop condition
    ASTNode *condition = expr(parser);
    if (!condition) {
        fprintf(stderr, "Error: Failed to parse condition in while_statement.\n");
        exit(EXIT_FAILURE);
    }

    // Verify that the condition is not an EMPTY_NODE
    if (condition->type == EMPTY_NODE) {
        fprintf(stderr, "Error: Condition in while loop is invalid (EMPTY_NODE).\n");
        exit(EXIT_FAILURE);
    }

    // Consume the closing parenthesis ')'
    consume_token(parser, TOKEN_RPAREN);

    // Analyze the body of the loop
    ASTNode *body = statement(parser);
    if (!body) {
        fprintf(stderr, "Error: Failed to parse body in while_statement.\n");
        exit(EXIT_FAILURE);
    }

    // Verify that the body is not an EMPTY_NODE
    if (body->type == EMPTY_NODE) {
        fprintf(stderr, "Error: Body of while loop is invalid (EMPTY_NODE).\n");
        exit(EXIT_FAILURE);
    }

    // Create and return a node of type "while"
    ASTNode *whileNode = create_while_node(condition, body);
    if (!whileNode) {
        fprintf(stderr, "Error: Failed to create while node.\n");
        exit(EXIT_FAILURE);
    }

    return whileNode;
}



/**
 *
 * Parses a `for(initialization; condition; increment) { body }` loop.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the `for` loop.
 */

ASTNode * for_statement(Parser * parser) {


    consume_token(parser, TOKEN_KEYWORD_FOR);

    consume_token(parser, TOKEN_LPAREN);

    ASTNode *initialisation = statement(parser);
    if (!initialisation) {
        fprintf(stderr, "Error: Failed to parse 'initialisation' in for loop.\n");
        exit(EXIT_FAILURE);
    }

    if (parser->current_token->type == TOKEN_SEMI_COLON) {
        consume_token(parser, TOKEN_SEMI_COLON);
    } else {
        fprintf(stderr, "Error: Missing semi-colon after 'initialisation' in for loop. Current token: %d\n", parser->current_token->type);
        exit(EXIT_FAILURE);
    }   


    ASTNode *condition = expr(parser);
    if (!condition) {
        fprintf(stderr, "Error: Failed to parse 'condition' in for loop.\n");
        exit(EXIT_FAILURE);
    }

    if (parser->current_token->type == TOKEN_SEMI_COLON) {
        consume_token(parser, TOKEN_SEMI_COLON);
    } else {
        fprintf(stderr, "Error: Missing semi-colon after 'condition' in for loop. Current token: %d\n", parser->current_token->type);
        exit(EXIT_FAILURE);
    }

    ASTNode *incrementation = statement(parser);
    if (!incrementation) {
        fprintf(stderr, "Error: Failed to parse 'incrementation' in for loop.\n");
        exit(EXIT_FAILURE);
    }

    if (parser->current_token->type == TOKEN_RPAREN) {
        consume_token(parser, TOKEN_RPAREN);
    } else {
        fprintf(stderr, "Error: Missing closing parenthesis ')' in for loop. Current token: %d\n", parser->current_token->type);
        exit(EXIT_FAILURE);
    }

    
    ASTNode *body = NULL;
    if (parser->current_token->type == TOKEN_LBRACE) {
        body = block(parser);
    } else {
        body = statement(parser);
    }
    if (!body) {
        fprintf(stderr, "Error: Failed to parse 'body' in for loop.\n");
        exit(EXIT_FAILURE);
    }

    return create_for_node(initialisation, condition, incrementation, body);
}




/**
 *
 * Parses a block of code enclosed in `{}` containing multiple statements.
 *
 * @param parser - The Parser performing the analysis.
 * @return - An ASTNode representing the block of code.
 */


ASTNode * block(Parser * parser) {

    consume_token(parser, TOKEN_LBRACE);

    // Initialize the list of statements
    unsigned short capacity = 10;
    unsigned short size = 0;
    ASTNode **statements = malloc(capacity * sizeof(ASTNode *));
    if (!statements) {
        fprintf(stderr, "Error: Memory allocation failed for block statements.\n");
        exit(EXIT_FAILURE);
    }

    while (parser->current_token->type != TOKEN_RBRACE) {

        // Ignore isolated semicolons
        if (parser->current_token->type == TOKEN_SEMI_COLON) {
            consume_token(parser, TOKEN_SEMI_COLON);
            continue;
        }

        ASTNode *stmt = statement(parser);
        if (stmt->type != EMPTY_NODE) {
            statements[size++] = stmt;
        } else {
            fprintf(stderr, "Error: Unexpected EMPTY_NODE in block. Current token: %d.\n", parser->current_token->type);
            free(statements);
            exit(EXIT_FAILURE);
        }

        if (size >= capacity) {
            capacity *= 2;
            statements = realloc(statements, capacity * sizeof(ASTNode *));
            if (!statements) {
                fprintf(stderr, "Error: Memory reallocation failed in block.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    consume_token(parser, TOKEN_RBRACE);

    return create_statements_node_list(statements, capacity, size);
}
