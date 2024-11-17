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

            int * res_val = interpret(interpreter, tree);

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
        // check if file input is in correct format and file exists
        // get second argument which corresponds to first user input passed to the executable
        char * filepath = argv[1];
//        printf("\n filepath string provided as argument to the program : %s", filepath);
        unsigned short file_input_check = validate_file_input(filepath);
        if(file_input_check == WRONG_MAIN_INPUT_FILE_EXTENSION){
            printf("\nFile input must have a .zl extension.");
        }else if(file_input_check == FILE_DOES_NOT_EXIST){
            printf("\nFile does not exist");
        }else if(file_input_check == VALID_INPUT) printf("\n File input OK");
        printf("\n file input check is : %hu", file_input_check);



    }else if(argc > 2){
        printf("zlang command accepts no input or a filepath string as input. Either execute zlang without "
               "arguments or provide a a valid filepath string as argument");
    }

    return 0;
}

unsigned short validate_file_input(char * filepath){
    char * file_extension = strrchr(filepath, '.');
    printf("\n file extension checked in validate_field_input : %s", file_extension);
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

//
//void repl(){
////    char expression[MAX_EXPRESSION_LENGTH];
//    char *expression = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
//
//    // attach an even handler to SIGINT ( event emitted when pressing Ctrl + C)
//    signal(SIGINT, handle_sigint);
//
//    // no file input was provided, behave as REPL
//    while(running){
//        printf("\n>>> ");
//        if(fgets(expression, MAX_EXPRESSION_LENGTH, stdin) == NULL){
//            break;
//        };
//
//        // check is user wants to exit either by typing exit
//        if(strncmp(expression, "exit", 4) == 0 || strncmp(expression, "exit;", 5) == 0){
//            running = 0;
//        }
//
//        // if no exit was demanded by user, remove the carriage return character from the string expression before evaluation
//        char * idx = strrchr(expression, '\n');
//        *idx = '\0';
//
//        ExpressionValue * exprResult = eval_expr(&expression);
//        if(exprResult == NULL){
//            return;
//        }
//        printf("\nExpr result type : %u", exprResult->valueType);
//        if(exprResult->valueType==DOUBLE){
//            printf("\n %lf", (atof)(exprResult->value));
//        }else if(exprResult->valueType==LONG){
//            printf("\n %ld", (atol)(exprResult->value));
//        }
//
//        free(exprResult);
//    }
//}