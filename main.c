#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "constants.h"
#include "stdlib.h"
#include "lexer.h"
#include "interpreter.h"

unsigned short validate_file_input(char * filepath);

unsigned char running = 1;

void handle_sigint(int sig){
    printf("\nUser Keyboard Interrupt. Ctrl + C");
    running = 0;
}

int main(int argc, char ** argv) {

    if(argc == 1){
        // REPL mode (sans fichier)
        char *expression = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));

        // attach an event handler to SIGINT (event emitted when pressing Ctrl + C)
        signal(SIGINT, handle_sigint);

        // initialize global scope
        GLOBAL_SCOPE * global_scope = init_global_scope(20);

        // No file input provided, behave as REPL
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
                free(expression);
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

            int * res_val = interpret(interpreter, tree);

            // Free memory for interpreter, parser, and lexer
            free_interpreter(interpreter);
            free(res_val);
            interpreter = NULL;
            free_node(tree);
            tree = NULL;
        }
        if(global_scope != NULL){
            free_global_scope(global_scope);
        }

    }else if(argc == 2){
        // Check if file input is correct
        char * filepath = argv[1];
        unsigned short file_input_check = validate_file_input(filepath);
        if(file_input_check == WRONG_MAIN_INPUT_FILE_EXTENSION){
            printf("\nFile input must have a .zl extension.");
        }else if(file_input_check == FILE_DOES_NOT_EXIST){
            printf("\nFile does not exist");
        }else if(file_input_check == VALID_INPUT){
            printf("\nFile input OK\n");

            // File processing logic
            FILE *file = fopen(filepath, "r");
            if(file == NULL){
                printf("\nError opening file.\n");
                return 1;
            }

            GLOBAL_SCOPE * global_scope = init_global_scope(20);
            char *line = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));

            printf("Processing file: %s\n", filepath);

            while(fgets(line, MAX_EXPRESSION_LENGTH, file)) {
                if(strlen(line) <= 1){
                    continue;
                }

                // Remove the newline character if present
                char *idx = strrchr(line, '\n');
                if (idx != NULL) *idx = '\0';

                Lexer * lexer = create_lexer(line);
                Parser * parser = create_parser(lexer);
                Interpreter * interpreter = create_interpreter(parser, global_scope);

                ASTNode * tree = statements_list(parser);

                int * res_val = interpret(interpreter, tree);

                // Affichage du résultat de l'interprétation
                if (res_val != NULL) {
                    printf("Result: %d\n", *res_val);
                } else {
                    printf("No result returned.\n");
                }

                // Free memory for interpreter, parser, and lexer
                free_interpreter(interpreter);
                free(res_val);
                interpreter = NULL;
                free_node(tree);
                tree = NULL;
            }

            free(line);
            fclose(file);
            if(global_scope != NULL){
                free_global_scope(global_scope);
            }
        }
    }else if(argc > 2){
        printf("zlang command accepts no input or a filepath string as input. Either execute zlang without "
               "arguments or provide a valid filepath string as argument");
    }

    return 0;
}

unsigned short validate_file_input(char * filepath){
    char * file_extension = strrchr(filepath, '.');
    if(file_extension == NULL || strcmp(file_extension, ".zl") != 0){
        return WRONG_MAIN_INPUT_FILE_EXTENSION;
    }

    // Check if file exists
    FILE * f = fopen(filepath, "r");
    if(f == NULL){
        return FILE_DOES_NOT_EXIST;
    }
    fclose(f);

    return VALID_INPUT;
}
