//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "lexer.h"
#include "abstract_syntax_tree.h"



Interpreter * create_interpreter(Parser * parser, GLOBAL_SCOPE * global_scope){
    Interpreter * interpreter = (Interpreter *)malloc(sizeof(Interpreter));
    interpreter->parser = parser;
    if(global_scope) {
        interpreter->global_scope = global_scope;
    }
    return interpreter;
}

void free_interpreter(Interpreter * interpreter){
    if(interpreter == NULL) return;
    if(interpreter->parser != NULL) free_parser(interpreter->parser);
    free(interpreter);
}


int * interpret(Interpreter * interpreter, ASTNode * node){

    if(node == NULL){
        fprintf(stderr, "Received NULL node parameter in interpret function.");
//        exit(EXIT_FAILURE);
        return NULL;
    }

    if(node->type == STATEMENTS_LIST_NODE){
        return visit_node(interpreter, node);
    }
//    else if(node->type == ASSIGNMENT_NODE){
//        visit_assign_node(interpreter, node);
//        // get value for the variable to which a value was assigned
//        char *var_name = node->node->assignOpNode->identifier->node->variableNode->varToken->value.strValue;
//        VariableScope * var_scope_found = find_variable_in_global_scope(interpreter->global_scope, var_name);
//        int value = var_scope_found->value.intValue;
//        return value;
//    }else if(node->type == VARIABLE_NODE){
//        int * var_val = visit_var_node(interpreter, node);
//        if(var_val == NULL){
//            fprintf(stderr, "Variable not found.");
//            exit(EXIT_FAILURE);
//        }
//        return *var_val;
////        return visit_var_node(interpreter, node);
//    }else if(node->type == UNARY_OPERATOR_NODE){
//        return visit_unary_op_node(interpreter, node);
//    }
//    if(node->type == NUMBER_NODE){
//        return visit_number_node(interpreter, node);
//    }else if(node->type == BINARY_OPERATOR_NODE){
//        return visit_bin_op_node(interpreter, node);
//    }else if(node->type == EOF){
//        return 0;
//    }
    else {

        fprintf(stderr, "\nError: Invalid node type.\n");
//        fprintf(stderr, "\n node type is : %d", node->type);
        return NULL;
    }
}


int * visit_bin_op_node( Interpreter * interpreter, ASTNode * node){
    int * left_ptr = visit_node(interpreter, node->node->binaryOpNode->left);
    int * right_ptr = visit_node(interpreter, node->node->binaryOpNode->right);

    if(left_ptr == NULL) return NULL;
    if(right_ptr == NULL) return NULL;

    int left_value = *left_ptr;
    int right_value = *right_ptr;
    free(left_ptr);
    free(right_ptr);

    int * result_val_ptr = malloc(sizeof(int));

    switch (node->node->binaryOpNode->operator->type){
        case TOKEN_OPERATOR_PLUS:
            *result_val_ptr = left_value + right_value;
            return result_val_ptr;
        case TOKEN_OPERATOR_MINUS:
            *result_val_ptr = left_value - right_value;
            return result_val_ptr;
        case TOKEN_OPERATOR_MULT:
            *result_val_ptr = left_value * right_value;
            return result_val_ptr;
        case TOKEN_OPERATOR_DIV:
            if (right_value == 0) {
                printf("Error : Division by zero\n");
                return NULL;
            }
            *result_val_ptr = left_value / right_value;
            return result_val_ptr;
        default:
            printf("Error : Unknown operation.\n");
            return NULL;
    }
}

int * visit_number_node( Interpreter * interpreter, ASTNode * node){
    NumberNode * numNode = node->node->numNode;
    int * ptr_val = malloc(sizeof(int));
    *ptr_val = numNode->value;
    return ptr_val;
//    return numNode->value;
}

int * visit_unary_op_node( Interpreter * interpreter, ASTNode *node) {
    int * expr_value_ptr = visit_node(interpreter, node->node->unaryOpNode->expression);
    int * ptr_val = malloc(sizeof(int));

    if(expr_value_ptr == NULL){
        fprintf(stderr, "Error : Unary operator node doesn't have an integer value to be applied to.");
        return NULL;
    }

    int expr_value = *expr_value_ptr;
//    free(expr_value_ptr);

    unsigned int value_type = node->node->unaryOpNode->operator->valueType;
    if(value_type == TOKEN_OPERATOR_PLUS){
        *ptr_val = +expr_value;
    }else if(value_type == TOKEN_OPERATOR_MINUS){
        *ptr_val = -expr_value;
    }else{
        printf("\nError : invalid unary operator.\n");
        ptr_val = NULL;
    }

    return ptr_val;
}

int * visit_assign_node( Interpreter * interpreter, ASTNode *node) {
    char *var_name = node->node->assignOpNode->identifier->node->variableNode->varToken->value.strValue;
    int * ptr_value = visit_node(interpreter, node->node->assignOpNode->expression);
    int value = *ptr_value;
    free(ptr_value);
    GLOBAL_SCOPE * global_scope = interpreter->global_scope;
    // Create variable scope to add to global scope
    VariableScope * var_scope_to_add = malloc(sizeof(VariableScope));

    // Create variable node
    VariableNode * var_node_to_add = malloc(sizeof(VariableNode));
    var_node_to_add->valueType = INT;

    // create token representing the identifier

    Token * var_token = create_token(TOKEN_IDENTIFIER, STRING, var_name);
    var_node_to_add->varToken = var_token;

    // Update variable scope to add with newly created identifier token and value to store in the variable
    var_scope_to_add->variableNode = var_node_to_add;
    var_scope_to_add->value.intValue = value;

    set_variable_in_global_scope(global_scope, var_scope_to_add);

    int * success_ptr = malloc(sizeof(int));
    *success_ptr = 0;
    return success_ptr;
}


int * visit_var_node( Interpreter * interpreter, ASTNode *node) {
    char * varName = node->node->variableNode->varToken->value.strValue;
    GLOBAL_SCOPE * global_scope = interpreter->global_scope;
    VariableScope * varScopeFound = find_variable_in_global_scope(global_scope, varName);
    if(varScopeFound && varScopeFound->value.intValue){
        int value = varScopeFound->value.intValue;
        int * var_value = malloc(sizeof(int));
        *var_value = value;
        return var_value;
    }
    return NULL;
}

int * visit_print_node(Interpreter * interpreter, ASTNode * node){
    int * value = visit_node(interpreter, node->node->printNode->expression);
    if(value != NULL){
        printf("%d", *value);
        free(value);
        int * success_ptr = malloc(sizeof(int));
        *success_ptr = 0;
        return success_ptr;
    }else{
        return NULL;
    }
}

int * visit_statements_list(Interpreter * interpreter, ASTNode * node){
    if(node == NULL){
        fprintf(stderr, "Int visit_statements_list : node parameter pointer must be a non null "
                        "pointer to a valid ASTNode");
        return NULL;
    }
    if(node->type != STATEMENTS_LIST_NODE){
        fprintf(stderr, "Int visit_statements_list : Expected node of type STATEMENTS_LIST_NODE.");
        return NULL;
    }
    if(node->node->stmtListNode == NULL){
        fprintf(stderr, "Int visit_statements_list : node parameter pointer should point "
                        "to a valid ASTNode of type STATEMENTS_LIST_NODE that has a non null list of statement nodes");
        return NULL;
    }

    unsigned short size = node->node->stmtListNode->size;

    for(unsigned short idx=0; idx < size; ++idx){
        ASTNode * node_to_visit = node->node->stmtListNode->nodes[idx];
        int *res_val = visit_node(interpreter, node_to_visit);
        free(res_val);
    }

    int * success_ptr = malloc(sizeof(int));
    *success_ptr = 0;
    return success_ptr;
}

int * visit_node( Interpreter * interpreter, ASTNode * node){
    if(node->type == NUMBER_NODE){
        int *ptr_val = visit_number_node(interpreter, node);
        return ptr_val;
    }else if (node->type == BINARY_OPERATOR_NODE){
        return visit_bin_op_node(interpreter, node);
    }else if (node->type == UNARY_OPERATOR_NODE){
        return visit_unary_op_node(interpreter, node);
    }else if(node->type == ASSIGNMENT_NODE){
        return visit_assign_node(interpreter, node);
    }else if(node->type == VARIABLE_NODE){
        int * var_val = visit_var_node(interpreter, node);
        if(var_val == NULL){
            fprintf(stderr, "Undefined variable");
            return NULL;
        }
        return var_val;
    }else if(node->type == PRINT_NODE){
        return visit_print_node(interpreter, node);
    }else if(node->type == STATEMENTS_LIST_NODE){
        return visit_statements_list(interpreter, node);
    }else{
        fprintf(stderr, "\n Invalid node type encountered.\n");
        return NULL;
//        exit(EXIT_FAILURE);
    }
}




GLOBAL_SCOPE * init_global_scope(unsigned short initialCapacity){
    GLOBAL_SCOPE * globalScope = malloc(sizeof(GLOBAL_SCOPE));
    if(globalScope == NULL){
        fprintf(stderr, "\nMemory allocation failed when initializing global variable scope");
        exit(EXIT_FAILURE);
    }
    globalScope->capacity = initialCapacity;
    globalScope->size = 0;

    globalScope->variables= malloc(initialCapacity * sizeof(VariableScope *));
    if(globalScope->variables == NULL){
        fprintf(stderr, "\nIn interpreter.c in function init_global_scope the following error has occurred : "
                        "Memory allocation failed for variables array");
        return NULL;
    }

    for(unsigned short idx = 0; idx < globalScope->capacity; ++idx){
        globalScope->variables[idx] = NULL;
    }

    return globalScope;
}


VariableScope * find_variable_in_global_scope(GLOBAL_SCOPE * globalScope, const char * varName) {
    for (unsigned short idx = 0; idx < globalScope->size; ++idx) {
        if(globalScope->variables[idx] != NULL && globalScope->variables[idx]->variableNode != NULL &&
           globalScope->variables[idx]->variableNode->varToken != NULL){
            char * gl_scope_var_name = globalScope->variables[idx]->variableNode->varToken->value.strValue;
            if (gl_scope_var_name != NULL && strcmp(gl_scope_var_name, varName) == 0) {
                return globalScope->variables[idx];
            }
        }

    }
    return NULL;
}

void set_variable_in_global_scope(GLOBAL_SCOPE * global_scope, VariableScope * var_scope){
    if (var_scope == NULL || var_scope->variableNode == NULL || var_scope->variableNode->varToken == NULL) {
        fprintf(stderr, "\nInvalid VariableScope passed to set_variable_in_global_scope");
        return;
    }

    char * var_name_searched = var_scope->variableNode->varToken->value.strValue;
    if (var_name_searched == NULL) {
        fprintf(stderr, "\nVariable name is NULL in set_variable_in_global_scope");
        return;
    }

    VariableScope * var_scope_found = find_variable_in_global_scope(global_scope, var_name_searched);

    if(var_scope_found){

        // update value with the new value
        if (var_scope_found->variableNode->varToken->value.strValue != NULL) {
            free(var_scope_found->variableNode->varToken->value.strValue);
        }

        if(var_scope_found->variableNode->valueType == STRING) {
            free(var_scope_found->value.stringValue);
        }
        free(var_scope_found->variableNode);

        var_scope_found->variableNode = var_scope->variableNode;
        var_scope_found->value = var_scope->value;
        var_scope_found->variableNode->valueType = var_scope->variableNode->valueType;

    }else{
        // add variable to the global scope
        add_variable_to_global_scope(global_scope, var_scope);
    }
}

void add_variable_to_global_scope(GLOBAL_SCOPE * globalScope,
                                  VariableScope * varScope){

    if (varScope == NULL || varScope->variableNode == NULL || varScope->variableNode->varToken == NULL) {
        fprintf(stderr, "\nInvalid VariableScope passed to add_variable_to_global_scope");
        return;
    }

    unsigned short capacity = globalScope->capacity;
    unsigned short size = globalScope->size;

    if(size >= capacity){
        // double capacity and copy elements
        capacity = capacity * 2;
        VariableScope ** newVariableScopes = malloc(capacity * sizeof(VariableScope *));
        if (newVariableScopes == NULL) {
            fprintf(stderr, "Memory allocation failed when expanding global scope.\n");
            exit(EXIT_FAILURE);
        }
        for(unsigned short idx = 0; idx < size; ++idx){
            newVariableScopes[idx] = globalScope->variables[idx];
        }

        free(globalScope->variables);
        globalScope->variables = newVariableScopes;
        globalScope->capacity = capacity;
    }

    // add the new variable to the global scope
    globalScope->variables[size] = varScope;
    globalScope->size = globalScope->size + 1;
}


void display_global_scope_variables(GLOBAL_SCOPE * global_scope){
    for(unsigned short idx=0; idx < global_scope->size; ++idx){
        VariableScope * varScopeToDisplay = global_scope->variables[idx];
        char * varName = varScopeToDisplay->variableNode->varToken->value.strValue;
        VariableNode * varNodeTodisplay = varScopeToDisplay->variableNode;
        if(varNodeTodisplay->valueType == INT){
            int valueToDisplay = varScopeToDisplay->value.intValue;
            printf("\n Value of the variable %s from global scope is : %d", varName, valueToDisplay);
        }else if(varNodeTodisplay->valueType == LONG){
            long valueToDisplay = varScopeToDisplay->value.longValue;
            printf("\n Value of the variable %s from global scope is : %ld", varName, valueToDisplay);
        }else if(varNodeTodisplay->valueType == FLOAT){
            float valueToDisplay = varScopeToDisplay->value.floatValue;
            printf("\n Value of the variable %s from global scope is : %f", varName, valueToDisplay);
        }else if(varNodeTodisplay->valueType == DOUBLE){
            double valueToDisplay = varScopeToDisplay->value.doubleValue;
            printf("\n Value of the variable %s from global scope is : %lf", varName, valueToDisplay);
        }else if(varNodeTodisplay->valueType == CHAR){
            char valueToDisplay = varScopeToDisplay->value.charValue;
            printf("\n Value of the variable %s from global scope is : %c", varName, valueToDisplay);
        } else if(varNodeTodisplay->valueType == STRING){
            char * valueToDisplay = varScopeToDisplay->value.stringValue;
            printf("\n Value of the variable %s from global scope is : %s", varName, valueToDisplay);
        }

    }
}



void free_global_scope(GLOBAL_SCOPE * globalScope){
    if (globalScope == NULL) return;

    for (unsigned short idx = 0; idx < globalScope->size; ++idx) {
//        printf("\nFreeing variable at index %d\n", idx);
        VariableScope * varScopeToFree = globalScope->variables[idx];
        if (varScopeToFree != NULL) {
            if (varScopeToFree->variableNode != NULL) {
                if (varScopeToFree->variableNode->varToken != NULL) {
                    if (varScopeToFree->variableNode->varToken->value.strValue != NULL) {
                        free(varScopeToFree->variableNode->varToken->value.strValue);
                        varScopeToFree->variableNode->varToken->value.strValue = NULL;
                    }
                    free(varScopeToFree->variableNode->varToken);
                    varScopeToFree->variableNode->varToken = NULL;
                }
            }
            if (varScopeToFree->variableNode->valueType == STRING &&
                varScopeToFree->value.stringValue != NULL) {
                free(varScopeToFree->value.stringValue);
            }
            free(varScopeToFree->variableNode);
            free(varScopeToFree);
            varScopeToFree = NULL;
        }
    }

    free(globalScope->variables);
    free(globalScope);
}


//void interpret_file(const char * filepath){
//    FILE * f = fopen(filepath, "r");
//    if(f == NULL) {
//        printf("\nUnable to open file.");
//        exit(EXIT_FAILURE);
//    }
//
//    char buffer[MAX_EXPRESSION_LENGTH] = {0};
//    for(int i = 0; i < MAX_EXPRESSION_LENGTH; ++i){
//        buffer[i] = '\0';
//    }
//    unsigned char inside_statement = 0;
//    long charIdx = 0;
//
//    char c = getc(f);
//    while( c != EOF){
//
//        buffer[charIdx] = c;
//        ++charIdx;
//
//        if(c == ';'){
//            // TODO : call to an evaluate_statement function that needs to be coded
//
//            // Reset buffer
//            for(int i =0; i < MAX_EXPRESSION_LENGTH; ++i){
//                buffer[i] = '\0';
//            }
//        }else{
//            // If the instruction is not yet complete add space, to make it easier to parse tokens later
//            buffer[charIdx] = ' ';
//            ++charIdx;
//        }
//
//    }
//
//    fclose(f);
//}
