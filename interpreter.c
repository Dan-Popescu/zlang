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

/**
 *
 * @param node
 */

int display_AST_RPN(ASTNode * node){
    if(node->type == NUMBER_NODE){
        NumberNode * numNode = node->node->numNode;
        printf("%d ", numNode->value);
        return numNode->value;
    }else if(node->type == BINARY_OPERATOR_NODE){
        BinaryOpNode * binaryOpNode = node->node->binaryOpNode;
        int left = display_AST_RPN(binaryOpNode->left);
        int right = display_AST_RPN(binaryOpNode->right);
//        printf("%d ", left);
//        printf("%d ", right);
        switch(binaryOpNode->operator->type){
            case TOKEN_OPERATOR_PLUS:
                printf("+ ");
                return left + right;
            case TOKEN_OPERATOR_MINUS:
                printf("- ");
                return left - right;
            case TOKEN_OPERATOR_MULT:
                printf("* ");
                return left * right;
            case TOKEN_OPERATOR_DIV:
                printf("/ ");
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
