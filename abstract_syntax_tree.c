//
// Created by Charara on 10/20/2024.
//

#include "abstract_syntax_tree.h"
#include <stdlib.h>

/**
 * Creates a new number node
 * @param tokenType : the type of the token
 * @param valueType : the value type of the token
 * @param value : the value of the token
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new number node and returns a pointer to it
 *
 *
 */
ASTNode * create_number_node(TokenType tokenType, ValueType valueType, int value){
    NumberNode * numNode = malloc(sizeof(NumberNode));
    if(numNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node Line 10:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }

    char * intStrValue = calloc(24, sizeof(char));
    if (intStrValue == NULL) {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node: Memory allocation "
                        "failed when trying to create string value.\n");
        free(numNode);
        exit(EXIT_FAILURE);
    }
    sprintf(intStrValue, "%d", value);
    Token * token = create_token(tokenType, valueType, intStrValue);
    if (token == NULL) {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node: Failed to create token.\n");
        free(intStrValue);
        free(numNode);
        exit(EXIT_FAILURE);
    }
    numNode->token = token;
    numNode->value = value;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node Line 23:"
                "Memory allocation failed when trying to create node.");
        free_token(token);
        free(numNode);
        exit(EXIT_FAILURE);
    }
    node->type = NUMBER_NODE;
    node->node = malloc(sizeof(NodeUnion)); // fixed this line
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node Line 30:"
                        "Memory allocation failed.");
        free_token(token);
        free(numNode);
        free(node);
        exit(EXIT_FAILURE);
    }
    node->node->numNode = numNode;

    return node;
}


/**
 * Creates a new binary operator node
 * @param opToken : the token representing the operator
 * @param left : the left operand
 * @param right : the right operand
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new binary operator node and returns a pointer to it
 *
 *
 */
ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right ){
    BinaryOpNode * binOpNode = malloc(sizeof(BinaryOpNode));
    if(binOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 31:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }


    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 42:"
                "Memory allocation failed when trying to create node.");
        free(binOpNode);
        exit(EXIT_FAILURE);
    }


    node->type = BINARY_OPERATOR_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 58:"
                        "Memory allocation failed when trying to create node.");
        free(binOpNode);
        free(node);
        exit(EXIT_FAILURE);
    }
    binOpNode->left = left;
    binOpNode->operator = opToken;
    binOpNode->right = right;
    node->node->binaryOpNode = binOpNode;
    return node;
}



/**
 * Creates a new unary operator node
 * @param token : the token representing the operator
 * @param expression : the expression to which the operator is applied
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new unary operator node and returns a pointer to it
 *
 *
 */
ASTNode * create_unary_operator_node(Token * token, ASTNode * expression){
    UnaryOpNode * unaryOpNode = malloc(sizeof(UnaryOpNode));
    if(unaryOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 101:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }


    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        free(unaryOpNode);
        exit(EXIT_FAILURE);
    }
    node->type = UNARY_OPERATOR_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        free(unaryOpNode);
        free(node);
        exit(EXIT_FAILURE);
    }
    unaryOpNode->operator = token;
    unaryOpNode->expression = expression;

    node->node->unaryOpNode = unaryOpNode;
    return node;
}


/**
 * Creates a new variable node
 * @param varToken : the token representing the variable
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new variable node and returns a pointer to it
 *
 *
 */
ASTNode * create_variable_node(Token * varToken){
    VariableNode * varNode = malloc(sizeof(VariableNode));
    if(varNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 156:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }


    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 164:"
                        "Memory allocation failed.");
        free(varNode);
        exit(EXIT_FAILURE);
    }
    node->type = VARIABLE_NODE;
    node->node= malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 171:"
                        "Memory allocation failed.");
        free(varNode);
        free(node);
        exit(EXIT_FAILURE);
    }
    varNode->varToken = varToken;
    node->node->variableNode = varNode;

    return node;
}


/**
 * Creates a new assignment node
 * @param left : the identifier of the variable being assigned to
 * @param assignmentToken : the token representing the assignment operator
 * @param right : the expression being assigned to the variable
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new assignment node and returns a pointer to it
 *
 *
 */
ASTNode * create_assignment_node(ASTNode * left, Token * assignmentToken, ASTNode * right){
    AssignOpNode * assignOpNode = malloc(sizeof(AssignOpNode));
    if(assignOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 208 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }


    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 218 :"
                        "Memory allocation failed.");
        free(assignOpNode);
        exit(EXIT_FAILURE);
    }
    node->type = ASSIGNMENT_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 225 :"
                        "Memory allocation failed.");
        free(assignOpNode);
        free(node);
        exit(EXIT_FAILURE);
    }

    assignOpNode->identifier = left;
    assignOpNode->assignmentToken = assignmentToken;
    assignOpNode->expression = right;

    node->node->assignOpNode = assignOpNode;

    return node;
}

/**
 * Creates a new statements list node
 * @param nodes : the list of nodes to be included in the list
 * @param capacity : the capacity of the list
 * @param size : the size of the list
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new statements list node and returns a pointer to it
 *
 *
 */
ASTNode * create_statements_node_list(ASTNode ** nodes, const unsigned short capacity, const unsigned short size){
    ASTNode * ast_node = malloc(sizeof(ASTNode));
    if(ast_node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 237 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    ast_node->type = STATEMENTS_LIST_NODE;
    ast_node->node = malloc(sizeof(NodeUnion));
    if(ast_node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 244 :"
                        "Memory allocation failed.");
        free(ast_node);
        exit(EXIT_FAILURE);
    }


    ast_node->node->stmtListNode->nodes = malloc(capacity * sizeof(ASTNode *));
    if(ast_node->node->stmtListNode->nodes == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 257 :"
                        "Memory allocation failed.");
        free(ast_node->node->stmtListNode);
        free(ast_node->node);
        free(ast_node);

        exit(EXIT_FAILURE);
    }
    for(unsigned short idx=0; idx < size; ++idx){
        ast_node->node->stmtListNode->nodes[idx] = nodes[idx];
    }

    ast_node->node->stmtListNode->capacity=capacity;
    ast_node->node->stmtListNode->size=size;

    return ast_node;
}


/**
 * Creates a new print node
 * @param exprNode : the expression to be printed
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new print node and returns a pointer to it
 *
 *
 */
ASTNode * create_print_node(ASTNode * exprNode){
    PrintNode * printNode = malloc(sizeof(PrintNode));
    if(printNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 274 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }


    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 282 :"
                        "Memory allocation failed.");
        free(printNode);
        exit(EXIT_FAILURE);
    }
    node->type = PRINT_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 289 :"
                        "Memory allocation failed.");
        free(node);
        free(printNode);
        exit(EXIT_FAILURE);
    }
    printNode->expression = exprNode;
    node->node->printNode = printNode;
    return node;
}


/**
 * Creates a new empty node
 * @return : a pointer to the newly created node
 *
 * @note : The function will exit the program if memory allocation fails
 *
 * this function creates a new empty node and returns a pointer to it
 *
 *
 */
ASTNode * create_empty_node(){
    EmptyNode * emptyNode = malloc(sizeof(EmptyNode));
    if(emptyNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 324 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 331 :"
                        "Memory allocation failed.");
        free(emptyNode);
        exit(EXIT_FAILURE);
    }
    node->type = EMPTY_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 338 :"
                        "Memory allocation failed.");
        free(emptyNode);
        free(node);
        exit(EXIT_FAILURE);
    }
    node->node->emptyNode = emptyNode;

    return node;
}


/**
 * Frees the memory allocated for a print node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a print node
 *
 * @note : the function returns if the node is NULL or if the node is not a print node
 */
void free_print_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != PRINT_NODE) return;
    if(node->node == NULL){
        free(node);
        return;
    }

    if(node->node->printNode == NULL){
        free(node->node);
        free(node);
        return;
    }

    if(node->node->printNode->expression != NULL){
        free_node(node->node->printNode->expression);
        node->node->printNode->expression = NULL;
    }

    free(node->node->printNode);
    free(node->node);
    free(node);
}

/**
 * Frees the memory allocated for a variable node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a variable node
 *
 * @note : the function returns if the node is NULL or if the node is not a variable node
 */
void free_variable_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != VARIABLE_NODE) return;
    if(node->node== NULL){
        free(node);
        return;
    }

    if(node->node->variableNode== NULL){
        free(node->node);
        free(node);
        return;
    }

    if(node->node->variableNode->varToken != NULL){
        free_token(node->node->variableNode->varToken);
    }

    free(node->node->variableNode);
    free(node->node);
    free(node);

}


/**
 * Frees the memory allocated for a unary operator node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a unary operator node
 *
 * @note : the function returns if the node is NULL or if the node is not a unary operator node
 */
void free_unary_operator_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != UNARY_OPERATOR_NODE) return;
    if(node->node == NULL){
        free(node);
        return;
    }
    if(node->node->unaryOpNode == NULL){
        free(node->node);
        free(node);
        return;
    }

    if(node->node->unaryOpNode->operator != NULL){
        free_token(node->node->unaryOpNode->operator);
    }

    if(node->node->unaryOpNode->expression != NULL){
        free_node(node->node->unaryOpNode->expression);
    }

    free(node->node->unaryOpNode);
    free(node->node);
    free(node);
}


/**
 * Frees the memory allocated for a binary operator node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a binary operator node
 *
 * @note : the function returns if the node is NULL or if the node is not a binary operator node
 */
void free_binary_operator_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != BINARY_OPERATOR_NODE) return;
    if(node->node== NULL){
        free(node);
        return;
    }

    if(node->node->binaryOpNode == NULL){
        free(node->node);
        free(node);
        return;
    }

    if(node->node->binaryOpNode->operator != NULL){
        free_token(node->node->binaryOpNode->operator);
        node->node->binaryOpNode->operator = NULL;
    }

    if(node->node->binaryOpNode->left != NULL){
        free_node(node->node->binaryOpNode->left);
        node->node->binaryOpNode->left = NULL;
    }

    if(node->node->binaryOpNode->right != NULL){
        free_node(node->node->binaryOpNode->right);
        node->node->binaryOpNode->left = NULL;
    }

    free(node->node->binaryOpNode);
    free(node->node);
    free(node);
}

/**
 * Frees the memory allocated for a number node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a number node
 *
 * @note : the function returns if the node is NULL or if the node is not a number node
 */
void free_number_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != NUMBER_NODE) return;
    if(node->node == NULL){
        printf("\n Unexpected node->node was null");
        free(node);
        return;
    }
    if(node->node->numNode == NULL){
        printf("\n Unexpected node->node->numNode was null");
        free(node->node);
        free(node);
        return;
    }
    if(node->node->numNode->token != NULL){
        free_token(node->node->numNode->token);
        node->node->numNode->token = NULL;
    }
    free(node->node->numNode);
    free(node->node);
    free(node);
}


/**
 * Frees the memory allocated for an assignment node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for an assignment node
 *
 * @note : the function returns if the node is NULL or if the node is not an assignment node
 */
void free_assignment_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != ASSIGNMENT_NODE) return;
    if(node->node == NULL){
        free(node);
        return;
    }
    if(node->node->assignOpNode == NULL){
        free(node->node);
        free(node);
        return;
    }
    if(node->node->assignOpNode->assignmentToken != NULL){
        free_token(node->node->assignOpNode->assignmentToken);
    }

    if(node->node->assignOpNode->identifier){
        free_node(node->node->assignOpNode->identifier);
    }
    if(node->node->assignOpNode->expression){
        free_node(node->node->assignOpNode->expression);
    }

    free(node->node->assignOpNode);
    free(node->node);
    free(node);
}


/**
 * Frees the memory allocated for a statements list node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for a statements list node
 *
 * @note : the function returns if the node is NULL or if the node is not a statements list node
 */
void free_statements_list_node(ASTNode * node){
    if(node == NULL) return;
    if(!node->type || node->type != STATEMENTS_LIST_NODE) return;
    if(node->node == NULL) return;
    if(node->node->stmtListNode == NULL) return;
    //const unsigned short capacity = node->node->stmtListNode->capacity;
    const unsigned short size = node->node->stmtListNode->size;
    // if( capacity < 0 || size < 0){
    //     if(node->node->stmtListNode->nodes != NULL) free(node->node->stmtListNode->nodes);
    //     free(node->node->stmtListNode);
    //     free(node->node);
    //     free(node);
    //     return;
    // }
    for(unsigned short idx = 0; idx < size; ++idx){
        ASTNode * node_to_free = node->node->stmtListNode->nodes[idx];

        if(node_to_free != NULL){
            free_node(node_to_free);
        }
    }

    free(node->node->stmtListNode->nodes);
    free(node->node->stmtListNode);
    free(node->node);
    free(node);
}

/**
 * Frees the memory allocated for an empty node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for an empty node
 *
 * @note : the function returns if the node is NULL or if the node is not an empty node
 */
void free_empty_node(ASTNode * node){
    if(node == NULL) return;
    if(node->type != EMPTY_NODE) return;
    if(node->node == NULL){
        free(node);
        return;
    }
    if(node->node->emptyNode == NULL){
        free(node->node);
        free(node);
        return;
    }
    free(node->node->emptyNode);
    free(node->node);
    free(node);
}


/**
 * Frees the memory allocated for a node
 * @param node : the node to be freed
 *
 * this function frees the memory allocated for an AST node
 *
 * @note : the function returns if the node is NULL
 */
void free_node(ASTNode * node){
    if(node == NULL) return;
    switch(node->type){
        case NUMBER_NODE:
            free_number_node(node); // check
            break;
        case BINARY_OPERATOR_NODE:
            free_binary_operator_node(node); // check
            break;
        case UNARY_OPERATOR_NODE:
            free_unary_operator_node(node); // check
            break;
        case VARIABLE_NODE:
            free_variable_node(node); // check
            break;
        case ASSIGNMENT_NODE:
            free_assignment_node(node); // check
            break;
        case PRINT_NODE:
            free_print_node(node); // check
            break;
        case STATEMENTS_LIST_NODE:
            free_statements_list_node(node); // check
            break;
        case EMPTY_NODE:
            free_empty_node(node); // check
        default:
            fprintf(stderr, "\n In free_node : Error : invalid node cannot be deallocated");
            exit(EXIT_FAILURE);
    }
}
