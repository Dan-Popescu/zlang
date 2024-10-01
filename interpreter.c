//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "constants.h"
#include "lexer.h"
#include "abstract_syntax_tree.h"



Interpreter * create_interpreter(Parser * parser){
    Interpreter * interpreter = (Interpreter *)malloc(sizeof(Interpreter));
    interpreter->parser = parser;
    return interpreter;
}

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
        printf("\nLine 145 Error. Invalid syntax.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * factor : NUMBER | LPAREN expr RPAREN
 *
 * @param interpreter
 * @return
 */
ASTNode * factor(Parser * parser){

    Token * token = parser->current_token;
    if(token->type == TOKEN_NUMBER){
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
 * @param interpreter
 * @return
 */

ASTNode * term(Parser * parser){
    ASTNode * left = factor(parser);

    while(parser->current_token->type == TOKEN_OPERATOR_MULT ||
            parser->current_token->type == TOKEN_OPERATOR_DIV){

//        Token * token = parser->current_token;

        // create new token to avoid value mutation issues (make sure the copy of the value is not changed
        // by changes make to the token in the parser
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
        printf("\n L82 binary node created with operator : %c", node->node->binaryOpNode->operator->value);
        printf("\n L83 operator type is : %d", token->type);
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
 * @param interpreter
 * @return
 */

ASTNode *  expr(Parser * parser){
    ASTNode * left = term(parser);

    while(parser->current_token->type == TOKEN_OPERATOR_PLUS ||
            parser->current_token->type == TOKEN_OPERATOR_MINUS){
//        Token * token = parser->current_token;

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
        printf("\n L113 binary node created with operator : %c", node->node->binaryOpNode->operator->value);
        printf("\n L114 operator type is : %d", token->type);
        //the newly created node become the new left node
        left = node;
    }

    return left;
}


ASTNode * parse(Parser * parser){
    return expr(parser);
}


int interpret(ASTNode * node){
    if(node->type == NUMBER_NODE){
        NumberNode * numNode = node->node->numNode;
        return numNode->value;
    }else if(node->type == BINARY_OPERATOR_NODE){
        BinaryOpNode * binaryOpNode = node->node->binaryOpNode;
        int left = interpret(binaryOpNode->left);
        int right = interpret(binaryOpNode->right);
        printf("\n Left is : %d", left);
        printf("\n right is : %d", right);
        switch(binaryOpNode->operator->type){
            case TOKEN_OPERATOR_PLUS:
                printf("\n + operator");
                return left + right;
            case TOKEN_OPERATOR_MINUS:
                printf("\n - operator");
                return left - right;
            case TOKEN_OPERATOR_MULT:
                printf("\n * operator");
                return left * right;
            case TOKEN_OPERATOR_DIV:
                printf("\n / operator");
                if(right == 0){
                    printf("\nError : Division by zero. \n");
                    exit(EXIT_FAILURE);
                }
                return left / right;
            default:
                printf("\nError : Invalid operator : %c\n", binaryOpNode->operator->type);
                exit(EXIT_FAILURE);
        }
    }else {
        printf("\nError: Invalid node type.\n");
        exit(EXIT_FAILURE);
    }
}





void interpret_file(const char * filepath){
    FILE * f = fopen(filepath, "r");
    if(f == NULL) {
        printf("\nUnable to open file.");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_EXPRESSION_LENGTH] = {0};
    for(int i = 0; i < MAX_EXPRESSION_LENGTH; ++i){
        buffer[i] = '\0';
    }
    unsigned char inside_statement = 0;
    long charIdx = 0;

    char c = getc(f);
    while( c != EOF){

        buffer[charIdx] = c;
        ++charIdx;

        if(c == ';'){
            // TODO : call to an evaluate_statement function that needs to be coded

            // Reset buffer
            for(int i =0; i < MAX_EXPRESSION_LENGTH; ++i){
                buffer[i] = '\0';
            }
        }else{
            // If the instruction is not yet complete add space, to make it easier to parse tokens later
            buffer[charIdx] = ' ';
            ++charIdx;
        }

    }

    fclose(f);
}
