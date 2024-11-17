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
        char *expression = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));

        // attach an even handler to SIGINT ( event emitted when pressing Ctrl + C)
        signal(SIGINT, handle_sigint);

        // initialize global scope
        GLOBAL_SCOPE * global_scope = init_global_scope(20);

        unsigned short iter = 0;

        // no file input was provided, behave as REPL
        while(running) {
            if(iter == 0){
                printf(">>> ");
            }else{
                printf("\n>>> ");
            }
            if(fgets(expression, MAX_EXPRESSION_LENGTH, stdin) == NULL){
                free(expression);
                break;
            };

            if(strlen(expression) <= 1){
                continue;
            }

            // check is user wants to exit either by typing exit
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

            int * res_val = interpret(interpreter,tree);

//            display_global_scope_variables(global_scope);

            // Free memory for interpreter, parser and lexer
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

            // File processing logic
            FILE *file = fopen(filepath, "r");
            if(file == NULL){
                printf("\nError opening file.\n");
                return 1;
            }

            GLOBAL_SCOPE * global_scope = init_global_scope(20);

            char curr_char = 0;
            unsigned short CHUNK_SIZE = 500;
            unsigned short capacity = CHUNK_SIZE;
            char * file_content = calloc(capacity, sizeof(char));
            unsigned short length = 0;
            curr_char = fgetc(file);

            while( curr_char != EOF){
                if(length  + 1 > capacity){
                    capacity += CHUNK_SIZE;
                    char * temp = realloc(file_content, capacity);
                    if(temp == NULL){
                        fprintf(stderr, "Memory reallocation failed for file content.");
                        free(file_content);
                        fclose(file);
                        exit(EXIT_FAILURE);
                    }
                    file_content = temp;
                }

                // append the character and update the length
                file_content[length] = curr_char;
                length++;

                curr_char = fgetc(file);
            }

            file_content[length] = '\0';

//            printf("file content : %s", file_content);

            Lexer * lexer = create_lexer(file_content);
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

            fclose(file);
            if(global_scope != NULL){
                free_global_scope(global_scope);
            }
        }

    }else if(argc > 2){
        printf("zlang command accepts no input or a filepath string as input. Either execute zlang without "
               "arguments or provide a a valid filepath string as argument");
    }

    return 0;
}

unsigned short validate_file_input(char * filepath){
    char * file_extension = strrchr(filepath, '.');
    if(file_extension == NULL || strcmp(file_extension, ".zl") != 0){
        return WRONG_MAIN_INPUT_FILE_EXTENSION;
    }

    // check if file exists
    FILE * f = fopen(filepath, "r");
    if(f==NULL){
        return FILE_DOES_NOT_EXIST;
    }

    return VALID_INPUT;

}
