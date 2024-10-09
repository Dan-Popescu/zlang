//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "constants.h"
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
}


/////////////////////////////////////////////////////////////////////////////////////////

int interpret(Interpreter * interpreter, ASTNode * node){
    if(node->type == ASSIGNMENT_NODE){
        visit_assign_node(interpreter, node);
        return 100;
    }else if(node->type == VARIABLE_NODE){
        return visit_var_node(interpreter, node);
    }else if(node->type == UNARY_OPERATOR_NODE){
        return visit_unary_op_node(interpreter, node);
    }
    if(node->type == NUMBER_NODE){
        return visit_number_node(interpreter, node);
    }else if(node->type == BINARY_OPERATOR_NODE){
        return visit_bin_op_node(interpreter, node);
    }else if(node->type == EOF){
        return 0;
    }else {

        printf("\nError: Invalid node type.\n");
        printf("\n node type is : %d", node->type);
        return -1;
//        exit(EXIT_FAILURE);
    }
}




int visit_bin_op_node( Interpreter * interpreter, ASTNode * node){
    int left_value = visit_node(interpreter, node->node->binaryOpNode->left);
    int right_value = visit_node(interpreter, node->node->binaryOpNode->right);

    switch (node->node->binaryOpNode->operator->type){
        case TOKEN_OPERATOR_PLUS:
            return left_value + right_value;
        case TOKEN_OPERATOR_MINUS:
            return left_value - right_value;
        case TOKEN_OPERATOR_MULT:
            return left_value * right_value;
        case TOKEN_OPERATOR_DIV:
            if (right_value == 0) {
                printf("Error : Division by zero\n");
                exit(EXIT_FAILURE);
            }
            return left_value / right_value;
        default:
            printf("Error : Unknown operation.\n");
            exit(EXIT_FAILURE);
    }
}

int visit_number_node( Interpreter * interpreter, ASTNode * node){
    NumberNode * numNode = node->node->numNode;
//    printf("%d ", numNode->value);
    return numNode->value;
}

int visit_unary_op_node( Interpreter * interpreter, ASTNode *node) {
    int expr_value = visit_node(interpreter, node->node->unaryOpNode->expression);

    switch (node->node->unaryOpNode->operator->valueType){
        case TOKEN_OPERATOR_PLUS:
            return +expr_value;
        case TOKEN_OPERATOR_MINUS:
            return -expr_value;
        default:
            printf("\nError : invalid unary operator.\n");
            exit(EXIT_FAILURE);
    }
}

void visit_assign_node( Interpreter * interpreter, ASTNode *node) {
    char *var_name = node->node->assignOpNode->identifier->node->variableNode->varToken->value.strValue;
    int value = visit_node(interpreter, node->node->assignOpNode->expression);
    GLOBAL_SCOPE * global_scope = interpreter->global_scope;
    // Create variable scope to add to global scope
    VariableScope * var_scope_to_add = malloc(sizeof(VariableScope));

    // Create variable node
    VariableNode * var_node_to_add = malloc(sizeof(VariableNode));
    var_node_to_add->valueType = INT;
    // create token representing the identifier
    Token * var_token = malloc(sizeof(Token));
    var_token->value.strValue = var_name;
    var_token->valueType = STRING;
    var_token->type = TOKEN_IDENTIFIER;
    var_node_to_add->varToken = var_token;

    // Update variable scope to add with newly created identifier token and value to store in the variable
    var_scope_to_add->variableNode = var_node_to_add;
    var_scope_to_add->value.intValue = value;

    set_variable_in_global_scope(global_scope, var_scope_to_add);
}

int visit_var_node( Interpreter * interpreter, ASTNode *node) {
    char * varName = node->node->variableNode->varToken->value.strValue;
    GLOBAL_SCOPE * global_scope = interpreter->global_scope;
    VariableScope * varScopeFound = find_variable_in_global_scope(global_scope, varName);
    int value = varScopeFound->value.intValue;
    return value;
    return 0;
}

int visit_node( Interpreter * interpreter, ASTNode * node){
    switch (node->type) {
        case NUMBER_NODE:
            return visit_number_node(interpreter, node);
        case BINARY_OPERATOR_NODE:
            return visit_bin_op_node(interpreter, node);
        case UNARY_OPERATOR_NODE:
            return visit_unary_op_node(interpreter, node);
        case ASSIGNMENT_NODE:
            visit_assign_node(interpreter, node);
            return 0;  // Les affectations ne retournent pas de valeur
        case VARIABLE_NODE:
            return visit_var_node(interpreter, node);
        default:
            fprintf(stderr, "\nError : invalid node type.\n");
            exit(EXIT_FAILURE);
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

    for(unsigned short idx; idx < globalScope->capacity; ++idx){
        globalScope->variables[idx] = malloc(sizeof(VariableScope));
    }

    return globalScope;
}


VariableScope * find_variable_in_global_scope(GLOBAL_SCOPE * globalScope, const char * varName) {
    for (unsigned short idx = 0; idx < globalScope->size; ++idx) {
        char * gl_scope_var_name = globalScope->variables[idx]->variableNode->varToken->value.strValue;
        if (strcmp(gl_scope_var_name, varName) == 0) {
            return globalScope->variables[idx];
        }
    }
    return NULL;
}

void set_variable_in_global_scope(GLOBAL_SCOPE * global_scope, VariableScope * var_scope){
    char * var_name_searched = var_scope->variableNode->varToken->value.strValue;
     printf("\n var name searched : %s", var_name_searched);
//    fflush(stdout);
    VariableScope * var_scope_found = find_variable_in_global_scope(global_scope, var_name_searched);

    if(var_scope_found){

        // update value with the new value
        free(var_scope_found->variableNode->varToken->value.strValue);
        free(var_scope_found->variableNode->varToken);
        free(var_scope_found->variableNode);
        if(var_scope_found->variableNode->valueType == STRING) free(var_scope_found->value.stringValue);
//        free(var_scope_found);
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
        printf("\nFreeing variable at index %d\n", idx);
        VariableScope * varScopeToFree = globalScope->variables[idx];
        if (varScopeToFree != NULL) {
            if (varScopeToFree->variableNode != NULL) {
                if (varScopeToFree->variableNode->varToken != NULL) {
                    if (varScopeToFree->variableNode->varToken->value.strValue != NULL) {
                        free(varScopeToFree->variableNode->varToken->value.strValue);
                    }
                    free(varScopeToFree->variableNode->varToken);
                }
                free(varScopeToFree->variableNode);
            }
            if (varScopeToFree->variableNode->valueType == STRING &&
                varScopeToFree->value.stringValue != NULL) {
                free(varScopeToFree->value.stringValue);
            }
            free(varScopeToFree);
        }
    }

    free(globalScope->variables);
    free(globalScope);
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
