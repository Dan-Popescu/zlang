


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "interpreter.h"
#include "lexer.h"


unsigned char running = 1;

void handle_sigint(int sig){
    printf("\nUser Keyboard Interrupt. Ctrl + C");
    running = 0;
}

int main(int argc, char ** argv) {
    if(argc == 1) {
        //        repl();
        char *expression = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));

        // attach an even handler to SIGINT ( event emitted when pressing Ctrl + C)
        signal(SIGINT, handle_sigint);

        // initialize global scope
        GLOBAL_SCOPE * global_scope = init_global_scope(20);


        // no file input was provided, behave as REPL
        while(running) {
            printf("\n>>> ");
            if(fgets(expression, MAX_EXPRESSION_LENGTH, stdin) == NULL){
                free(expression);
                break;
            };

            if(strlen(expression) <= 1){
                continue;
            }

            // check if user wants to exit either by typing exit
            if (strncmp(expression, "exit", 4) == 0 || strncmp(expression, "exit;", 5) == 0) {
                running = 0;
                continue;
            }

            // if no exit was demanded by user, remove the null character from the string expression before evaluation
            char *idx = strrchr(expression, '\n');
            *idx = '\0';


            Lexer * lexer = create_lexer(expression);

            Parser * parser = create_parser(lexer);
            Interpreter * interpreter = create_interpreter(parser, global_scope);


            ASTNode * tree = statements_list(parser);

            // printf("Node Type: %d\n", tree->type);
            // for ( int i=0; i < tree->node->stmtListNode->size; i++){
            //     printf("Node Type: %d\n", tree->node->stmtListNode->nodes[i]->type);
            // }

            int value = interpret(interpreter, tree);

            printf("%d", value);





        }
    }
}