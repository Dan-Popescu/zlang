//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "constants.h"
#include "lexer.h"



Interpreter * create_interpreter(Lexer * lexer){
    Interpreter * interpreter = (Interpreter *)malloc(sizeof(Interpreter));
    interpreter->lexer = lexer;
    interpreter->current_token = get_next_token(lexer);
    return interpreter;
}


void consume_token(Interpreter * interpreter, TokenType tokenType){
    if(interpreter->current_token->type == tokenType){
        free(interpreter->current_token);
        interpreter->current_token = get_next_token(interpreter->lexer);
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
int factor(Interpreter * interpreter){

    Token * token = interpreter->current_token;
    if(token->type == TOKEN_NUMBER){
        int value = token->value;
        consume_token(interpreter, TOKEN_NUMBER);
        return value;
    }else if(token->type == TOKEN_LPAREN){
        consume_token(interpreter, TOKEN_LPAREN);
        int result = expr(interpreter);
        consume_token(interpreter, TOKEN_RPAREN);
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

int term(Interpreter * interpreter){
    int result = factor(interpreter);
    while(interpreter->current_token->type == TOKEN_OPERATOR_MULT ||
            interpreter->current_token->type == TOKEN_OPERATOR_DIV){
        Token * token = interpreter->current_token;
        if(token->type == TOKEN_OPERATOR_MULT){
            consume_token(interpreter, TOKEN_OPERATOR_MULT);
            result = result * factor(interpreter);
        }else if(token->type == TOKEN_OPERATOR_DIV){
            consume_token(interpreter, TOKEN_OPERATOR_DIV);
            result = result / factor(interpreter);
        }
    }
    return result;
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

int expr(Interpreter * interpreter){
    int result = term(interpreter);
    while(interpreter->current_token->type == TOKEN_OPERATOR_PLUS ||
            interpreter->current_token->type == TOKEN_OPERATOR_MINUS){
        Token * token = interpreter->current_token;
        if(token->type == TOKEN_OPERATOR_PLUS){
            consume_token(interpreter, TOKEN_OPERATOR_PLUS);
            result = result + term(interpreter);
        }else if(token->type == TOKEN_OPERATOR_MINUS){
            consume_token(interpreter, TOKEN_OPERATOR_MINUS);
            result = result - term(interpreter);
        }
    }
    return result;
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
