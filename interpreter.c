//
//
//

#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "lexer.h"
#include "abstract_syntax_tree.h"

/**
 * Creates an interpreter instance.
 * @param parser - The parser to use for generating the AST nodes.
 * @param global_scope - The global scope for managing variables across the program.
 * @return A pointer to the newly created Interpreter structure.
 */

Interpreter *create_interpreter(Parser *parser, GLOBAL_SCOPE *global_scope)
{
    Interpreter *interpreter = (Interpreter *)malloc(sizeof(Interpreter));
    interpreter->parser = parser;
    if (global_scope)
    {
        interpreter->global_scope = global_scope;
    }
    return interpreter;
}

/**
 * Frees the memory associated with the interpreter.
 * @param interpreter - The interpreter to be freed.
 */

void free_interpreter(Interpreter *interpreter)
{
    if (interpreter == NULL)
        return;
    if (interpreter->parser != NULL)
        free_parser(interpreter->parser);
    free(interpreter);
}

/**
 * Interprets the given AST node and executes its logic.
 * @param interpreter - The interpreter managing execution.
 * @param node - The AST node to interpret.
 * @return A pointer to the result of the interpretation or NULL in case of errors.
 */

int *interpret(Interpreter *interpreter, ASTNode *node)
{

    if (node == NULL)
    {
        fprintf(stderr, "Received NULL node parameter in interpret function.");
        return NULL;
    }

    if (node->type == STATEMENTS_LIST_NODE)
    {
        return visit_node(interpreter, node);
    }
    else
    {
        fprintf(stderr, "\nError: Invalid node type.\n");
        return NULL;
    }
}

/**
 * Visits a binary operation node and evaluates its result.
 * @param interpreter - The interpreter handling execution.
 * @param node - The binary operation AST node.
 * @return A pointer to the result of the binary operation.
 */

int *visit_bin_op_node(Interpreter *interpreter, ASTNode *node)
{
    int *left_ptr = visit_node(interpreter, node->node->binaryOpNode->left);
    int *right_ptr = visit_node(interpreter, node->node->binaryOpNode->right);

    if (left_ptr == NULL)
        return NULL;
    if (right_ptr == NULL)
        return NULL;

    int left_value = *left_ptr;
    int right_value = *right_ptr;
    free(left_ptr);
    free(right_ptr);

    int *result_val_ptr = malloc(sizeof(int));

    switch (node->node->binaryOpNode->operator->type)
    {
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
        if (right_value == 0)
        {
            printf("Error : Division by zero\n");
            return NULL;
        }
        *result_val_ptr = left_value / right_value;
        return result_val_ptr;
    case TOKEN_OPERATOR_LESS_THAN:
        *result_val_ptr = left_value < right_value;
        return result_val_ptr;

    case TOKEN_OPERATOR_GREATER_THAN:
        *result_val_ptr = left_value > right_value;
        return result_val_ptr;
    }
}

/**
 * Visits a number node and retrieves its value.
 * @param interpreter - The interpreter managing execution.
 * @param node - The number AST node.
 * @return A pointer to the value of the number node.
 */

int *visit_number_node(Interpreter *interpreter, ASTNode *node)
{
    NumberNode *numNode = node->node->numNode;
    int *ptr_val = malloc(sizeof(int));
    *ptr_val = numNode->value;
    return ptr_val;
}

/**
 * Visits a unary operation node and applies the operation.
 * @param interpreter - The interpreter handling execution.
 * @param node - The unary operation AST node.
 * @return A pointer to the result of the unary operation.
 */

int *visit_unary_op_node(Interpreter *interpreter, ASTNode *node)
{
    int *expr_value_ptr = visit_node(interpreter, node->node->unaryOpNode->expression);
    int *ptr_val = malloc(sizeof(int));

    if (expr_value_ptr == NULL)
    {
        fprintf(stderr, "Error : Unary operator node doesn't have an integer value to be applied to.");
        return NULL;
    }

    int expr_value = *expr_value_ptr;
    //    free(expr_value_ptr);

    unsigned int value_type = node->node->unaryOpNode->operator->valueType;
    if (value_type == TOKEN_OPERATOR_PLUS)
    {
        *ptr_val = +expr_value;
    }
    else if (value_type == TOKEN_OPERATOR_MINUS)
    {
        *ptr_val = -expr_value;
    }
    else
    {
        printf("\nError : invalid unary operator.\n");
        ptr_val = NULL;
    }

    return ptr_val;
}

/**
 * Visits an assignment node and updates the variable in the global scope.
 * @param interpreter - The interpreter managing execution.
 * @param node - The assignment AST node.
 * @return A pointer to success status (0 if successful).
 */

int *visit_assign_node(Interpreter *interpreter, ASTNode *node)
{
    char *var_name = node->node->assignOpNode->identifier->node->variableNode->varToken->value.strValue;
    int *ptr_value = visit_node(interpreter, node->node->assignOpNode->expression);
    int value = *ptr_value;
    free(ptr_value);
    GLOBAL_SCOPE *global_scope = interpreter->global_scope;
    // Create variable scope to add to global scope
    VariableScope *var_scope_to_add = malloc(sizeof(VariableScope));

    // Create variable node
    VariableNode *var_node_to_add = malloc(sizeof(VariableNode));
    var_node_to_add->valueType = INT;

    // create token representing the identifier
    Token *var_token = create_token(TOKEN_IDENTIFIER, STRING, var_name);
    var_node_to_add->varToken = var_token;

    // Update variable scope to add with newly created identifier token and value to store in the variable
    var_scope_to_add->variableNode = var_node_to_add;
    var_scope_to_add->value.intValue = value;

    set_variable_in_global_scope(global_scope, var_scope_to_add);

    int *success_ptr = malloc(sizeof(int));
    *success_ptr = 0;
    return success_ptr;
}

/**
 * Visits a variable node and retrieves its value from the global scope.
 * @param interpreter - The interpreter managing execution.
 * @param node - The variable AST node.
 * @return A pointer to the value of the variable.
 */

int *visit_var_node(Interpreter *interpreter, ASTNode *node)
{
    char *varName = node->node->variableNode->varToken->value.strValue;
    GLOBAL_SCOPE *global_scope = interpreter->global_scope;
    VariableScope *varScopeFound = find_variable_in_global_scope(global_scope, varName);
    if (varScopeFound && varScopeFound->value.intValue)
    {
        int value = varScopeFound->value.intValue;
        int *var_value = malloc(sizeof(int));
        *var_value = value;
        return var_value;
    }
    return NULL;
}

/**
 * Visits a print node and prints the evaluated value.
 * @param interpreter - The interpreter managing execution.
 * @param node - The print AST node.
 * @return A pointer to success status (0 if successful).
 */

int *visit_print_node(Interpreter *interpreter, ASTNode *node)
{
    int *value = visit_node(interpreter, node->node->printNode->expression);
    if (value != NULL)
    {
        printf("%d\n", *value);
        free(value);
        int *success_ptr = malloc(sizeof(int));
        *success_ptr = 0;
        return success_ptr;
    }
    else
    {
        return NULL;
    }
}

/**
 * Visits a statements list node and executes all statements sequentially.
 * @param interpreter - The interpreter managing execution.
 * @param node - The statements list AST node.
 * @return A pointer to success status (0 if successful).
 */

int *visit_statements_list(Interpreter *interpreter, ASTNode *node)
{
    if (node == NULL)
    {
        fprintf(stderr, "Int visit_statements_list : node parameter pointer must be a non null "
                        "pointer to a valid ASTNode");
        return NULL;
    }
    if (node->type != STATEMENTS_LIST_NODE)
    {
        fprintf(stderr, "Int visit_statements_list : Expected node of type STATEMENTS_LIST_NODE.");
        return NULL;
    }
    if (node->node->stmtListNode == NULL)
    {
        fprintf(stderr, "Int visit_statements_list : node parameter pointer should point "
                        "to a valid ASTNode of type STATEMENTS_LIST_NODE that has a non null list of statement nodes");
        return NULL;
    }

    unsigned short size = node->node->stmtListNode->size;

    for (unsigned short idx = 0; idx < size; ++idx)
    {
        ASTNode *node_to_visit = node->node->stmtListNode->nodes[idx];
        int *res_val = visit_node(interpreter, node_to_visit);
        free(res_val);
    }

    int *success_ptr = malloc(sizeof(int));
    *success_ptr = 0;
    return success_ptr;
}

int *visit_while_node(Interpreter *interpreter, ASTNode *node)
{
    WhileNode *whileNode = node->node->whileNode;

    // Évaluez la condition au début
    int *condition_value = visit_node(interpreter, whileNode->condition);

    while (*condition_value)
    {
        free(condition_value); // Libérez la mémoire utilisée pour la condition précédente

        // Exécutez le corps de la boucle
        visit_node(interpreter, whileNode->body);

        // Réévaluez la condition
        condition_value = visit_node(interpreter, whileNode->condition);

        // Sécurité : assurez-vous que la condition n'est pas NULL
        if (condition_value == NULL)
        {
            fprintf(stderr, "Error: While loop condition returned NULL.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Libérez la mémoire finale de la condition
    free(condition_value);

    // Indiquez le succès
    int *success = malloc(sizeof(int));
    if (success == NULL)
    {
        fprintf(stderr, "Memory allocation failed for success in visit_while_node.\n");
        exit(EXIT_FAILURE);
    }
    *success = 0;
    return success;
}

int *visit_for_node(Interpreter *interpreter, ASTNode *node)
{
    if (!node || node->type != FOR_NODE || !node->node || !node->node->forNode)
        return NULL;

    ForNode *forNode = node->node->forNode;

    visit_node(interpreter, forNode->initialisation);
    int *condition_value = visit_node(interpreter, forNode->condition);

    while (condition_value && *condition_value)
    {
        free(condition_value);
        visit_node(interpreter, forNode->body);
        visit_node(interpreter, forNode->incrementation);
        condition_value = visit_node(interpreter, forNode->condition);
    }

    free(condition_value);
    int *success = malloc(sizeof(int));
    *success = 0;
    return success;
}

/**
 * Dispatch function that determines which visit function to call based on the node type.
 * @param interpreter - The interpreter managing execution.
 * @param node - The AST node to visit.
 * @return A pointer to the result of the visit function.
 */

int *visit_node(Interpreter *interpreter, ASTNode *node)
{
    if (!node)
    {
        fprintf(stderr, "Error: NULL node passed to visit_node.\n");
        return NULL;
    }

    if (node->type == NUMBER_NODE)
    {
        int *ptr_val = visit_number_node(interpreter, node);
        return ptr_val;
    }
    else if (node->type == BINARY_OPERATOR_NODE)
    {
        return visit_bin_op_node(interpreter, node);
    }
    else if (node->type == UNARY_OPERATOR_NODE)
    {
        return visit_unary_op_node(interpreter, node);
    }
    else if (node->type == ASSIGNMENT_NODE)
    {
        return visit_assign_node(interpreter, node);
    }
    else if (node->type == VARIABLE_NODE)
    {
        int *var_val = visit_var_node(interpreter, node);
        if (var_val == NULL)
        {
            fprintf(stderr, "Undefined variable");
            return NULL;
        }
        return var_val;
    }
    else if (node->type == PRINT_NODE)
    {
        return visit_print_node(interpreter, node);
    }
    else if (node->type == STATEMENTS_LIST_NODE)
    {
        return visit_statements_list(interpreter, node);
    }
    else if (node->type == WHILE_NODE)
    {
        return visit_while_node(interpreter, node);
    }
    else if (node->type == FOR_NODE)
    {
        return visit_for_node(interpreter, node);
    }
    else if (node->type == EMPTY_NODE)
    {
//        printf("Visiting EMPTY_NODE...\n");
        int *success = malloc(sizeof(int));
        *success = 0;
        return success;
    }
    else
    {
        fprintf(stderr, "\n Invalid node type encountered.\n");
        return NULL;
    }
}

/**
 * Initializes a global scope to store variables and their values.
 * @param initialCapacity - The initial capacity of the variables array.
 * @return A pointer to the newly created GLOBAL_SCOPE structure.
 */

GLOBAL_SCOPE *init_global_scope(unsigned short initialCapacity)
{
    GLOBAL_SCOPE *globalScope = malloc(sizeof(GLOBAL_SCOPE));
    if (globalScope == NULL)
    {
        fprintf(stderr, "\nMemory allocation failed when initializing global variable scope");
        exit(EXIT_FAILURE);
    }
    globalScope->capacity = initialCapacity;
    globalScope->size = 0;

    globalScope->variables = malloc(initialCapacity * sizeof(VariableScope *));
    if (globalScope->variables == NULL)
    {
        fprintf(stderr, "\nIn interpreter.c in function init_global_scope the following error has occurred : "
                        "Memory allocation failed for variables array");
        return NULL;
    }

    for (unsigned short idx = 0; idx < globalScope->capacity; ++idx)
    {
        globalScope->variables[idx] = NULL;
    }

    return globalScope;
}

/**
 * Searches for a variable in the global scope by name.
 * @param globalScope - The global scope to search in.
 * @param varName - The name of the variable to find.
 * @return A pointer to the VariableScope if found, or NULL if not found.
 */

VariableScope *find_variable_in_global_scope(GLOBAL_SCOPE *globalScope, const char *varName)
{
    for (unsigned short idx = 0; idx < globalScope->size; ++idx)
    {
        if (globalScope->variables[idx] != NULL && globalScope->variables[idx]->variableNode != NULL &&
            globalScope->variables[idx]->variableNode->varToken != NULL)
        {
            char *gl_scope_var_name = globalScope->variables[idx]->variableNode->varToken->value.strValue;
            if (gl_scope_var_name != NULL && strcmp(gl_scope_var_name, varName) == 0)
            {
                return globalScope->variables[idx];
            }
        }
    }
    return NULL;
}

/**
 * Adds or updates a variable in the global scope.
 * @param global_scope - The global scope to modify.
 * @param var_scope - The variable and its value to add or update.
 */

void set_variable_in_global_scope(GLOBAL_SCOPE *global_scope, VariableScope *var_scope)
{
    if (var_scope == NULL || var_scope->variableNode == NULL || var_scope->variableNode->varToken == NULL)
    {
        fprintf(stderr, "\nInvalid VariableScope passed to set_variable_in_global_scope");
        return;
    }

    char *var_name_searched = var_scope->variableNode->varToken->value.strValue;
    if (var_name_searched == NULL)
    {
        fprintf(stderr, "\nVariable name is NULL in set_variable_in_global_scope");
        return;
    }

    VariableScope *var_scope_found = find_variable_in_global_scope(global_scope, var_name_searched);

    if (var_scope_found)
    {

        // update value with the new value
        if (var_scope_found->variableNode->varToken->value.strValue != NULL)
        {
            free(var_scope_found->variableNode->varToken->value.strValue);
        }

        if (var_scope_found->variableNode->valueType == STRING)
        {
            free(var_scope_found->value.stringValue);
        }
        free(var_scope_found->variableNode);

        var_scope_found->variableNode = var_scope->variableNode;
        var_scope_found->value = var_scope->value;
        var_scope_found->variableNode->valueType = var_scope->variableNode->valueType;
    }
    else
    {
        // add variable to the global scope
        add_variable_to_global_scope(global_scope, var_scope);
    }
}

/**
 * Adds a new variable to the global scope.
 * @param globalScope - The global scope to modify.
 * @param varScope - The variable to add.
 */

void add_variable_to_global_scope(GLOBAL_SCOPE *globalScope,
                                  VariableScope *varScope)
{

    if (varScope == NULL || varScope->variableNode == NULL || varScope->variableNode->varToken == NULL)
    {
        fprintf(stderr, "\nInvalid VariableScope passed to add_variable_to_global_scope");
        return;
    }

    unsigned short capacity = globalScope->capacity;
    unsigned short size = globalScope->size;

    if (size >= capacity)
    {
        // double capacity and copy elements
        capacity = capacity * 2;
        VariableScope **newVariableScopes = malloc(capacity * sizeof(VariableScope *));
        if (newVariableScopes == NULL)
        {
            fprintf(stderr, "Memory allocation failed when expanding global scope.\n");
            exit(EXIT_FAILURE);
        }
        for (unsigned short idx = 0; idx < size; ++idx)
        {
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

/**
 * Displays all variables and their values in the global scope.
 * @param global_scope - The global scope to display.
 */

void display_global_scope_variables(GLOBAL_SCOPE *global_scope)
{
    for (unsigned short idx = 0; idx < global_scope->size; ++idx)
    {
        VariableScope *varScopeToDisplay = global_scope->variables[idx];
        char *varName = varScopeToDisplay->variableNode->varToken->value.strValue;
        VariableNode *varNodeTodisplay = varScopeToDisplay->variableNode;
        if (varNodeTodisplay->valueType == INT)
        {
            int valueToDisplay = varScopeToDisplay->value.intValue;
            printf("\n Value of the variable %s from global scope is : %d", varName, valueToDisplay);
        }
        else if (varNodeTodisplay->valueType == LONG)
        {
            long valueToDisplay = varScopeToDisplay->value.longValue;
            printf("\n Value of the variable %s from global scope is : %ld", varName, valueToDisplay);
        }
        else if (varNodeTodisplay->valueType == FLOAT)
        {
            float valueToDisplay = varScopeToDisplay->value.floatValue;
            printf("\n Value of the variable %s from global scope is : %f", varName, valueToDisplay);
        }
        else if (varNodeTodisplay->valueType == DOUBLE)
        {
            double valueToDisplay = varScopeToDisplay->value.doubleValue;
            printf("\n Value of the variable %s from global scope is : %lf", varName, valueToDisplay);
        }
        else if (varNodeTodisplay->valueType == CHAR)
        {
            char valueToDisplay = varScopeToDisplay->value.charValue;
            printf("\n Value of the variable %s from global scope is : %c", varName, valueToDisplay);
        }
        else if (varNodeTodisplay->valueType == STRING)
        {
            char *valueToDisplay = varScopeToDisplay->value.stringValue;
            printf("\n Value of the variable %s from global scope is : %s", varName, valueToDisplay);
        }
    }
}

/**
 * Frees all memory associated with the global scope and its variables.
 * @param globalScope - The global scope to free.
 */

void free_global_scope(GLOBAL_SCOPE *globalScope)
{
    if (globalScope == NULL)
        return;

    for (unsigned short idx = 0; idx < globalScope->size; ++idx)
    {
        //        printf("\nFreeing variable at index %d\n", idx);
        VariableScope *varScopeToFree = globalScope->variables[idx];
        if (varScopeToFree != NULL)
        {
            if (varScopeToFree->variableNode != NULL)
            {
                if (varScopeToFree->variableNode->varToken != NULL)
                {
                    if (varScopeToFree->variableNode->varToken->value.strValue != NULL)
                    {
                        free(varScopeToFree->variableNode->varToken->value.strValue);
                        varScopeToFree->variableNode->varToken->value.strValue = NULL;
                    }
                    free(varScopeToFree->variableNode->varToken);
                    varScopeToFree->variableNode->varToken = NULL;
                }
            }
            if (varScopeToFree->variableNode->valueType == STRING &&
                varScopeToFree->value.stringValue != NULL)
            {
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