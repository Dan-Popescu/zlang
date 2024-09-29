#include <stdio.h>
#include <string.h>
//#include <windows.h>
#include <signal.h>
#include "constants.h"
#include "tokenizer.h"
#include "stdlib.h"

unsigned short validate_file_input(char * filepath);
void repl();

unsigned char running = 1;

void handle_sigint(int sig){
    printf("\nUser Keyboard Interrupt. Ctrl + C");
    running = 0;
}

int main(int argc, char ** argv) {

    if(argc == 1){
        repl();

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


void repl(){
//    char expression[MAX_EXPRESSION_LENGTH];
    char *expression = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));

    // attach an even handler to SIGINT ( event emitted when pressing Ctrl + C)
    signal(SIGINT, handle_sigint);

    // no file input was provided, behave as REPL
    while(running){
        printf("\n>>> ");
        fgets(expression, MAX_EXPRESSION_LENGTH, stdin);

        // check is user wants to exit either by typing exit
        if(strncmp(expression, "exit", 4) == 0 || strncmp(expression, "exit;", 5) == 0){
            running = 0;
        }

        // if no exit was demanded by user, remove the null character from the string expression before evaluation
        char * idx = strrchr(expression, '\n');
        *idx = '\0';

        // evaluate expression
        // -> TODO function for executing a statement
//        char * exprPtr = expression;
//        Token * token = get_next_token(exprPtr);
//        printf("\ntoken value is : %s", token->value);
//        token = NULL;
//        exprPtr++;
//        token = get_next_token(exprPtr);
//        printf("\ntoken value is : %s", token->value);
//        token = NULL;
//        exprPtr++;
//        token = get_next_token(exprPtr);
//        printf("\ntoken value is : %s", token->value);

        Token * exprResult = evaluate_expr(expression);
        printf("\n %lf", (atof)(exprResult->value));

//        free(token->value);
//        free(token);
        free(exprResult);

    }
}