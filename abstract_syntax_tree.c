//
//
//

#include "abstract_syntax_tree.h"
#include <stdlib.h>


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
    node->node = malloc(sizeof(NumberNode));
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

ASTNode * create_binary_operator_node(Token * opToken, ASTNode * left, ASTNode * right ){
    BinaryOpNode * binOpNode = malloc(sizeof(BinaryOpNode));
    if(binOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 31:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    binOpNode->left = left;
    binOpNode->operator = opToken;
    binOpNode->right = right;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 42:"
                "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    node->type = BINARY_OPERATOR_NODE;
    node->node = malloc(sizeof binOpNode);
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 58:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    node->node->binaryOpNode = binOpNode;
    return node;
}

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

ASTNode * create_unary_operator_node(Token * token, ASTNode * expression){
    UnaryOpNode * unaryOpNode = malloc(sizeof(UnaryOpNode));
    if(unaryOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 101:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    unaryOpNode->operator = token;
    unaryOpNode->expression = expression;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = UNARY_OPERATOR_NODE;
    node->node = malloc(sizeof(UnaryOpNode));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->unaryOpNode = unaryOpNode;
    return node;
}

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



ASTNode * create_variable_node(Token * varToken){
    VariableNode * varNode = malloc(sizeof(VariableNode));
    if(varNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 156:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    varNode->varToken = varToken;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 164:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = VARIABLE_NODE;
    node->node= malloc(sizeof(VariableNode));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 171:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->variableNode = varNode;

    return node;
}

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


ASTNode * create_assignment_node(ASTNode * left, Token * assignmentToken, ASTNode * right){
    AssignOpNode * assignOpNode = malloc(sizeof(AssignOpNode));
    if(assignOpNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 208 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    assignOpNode->identifier = left;
    assignOpNode->assignmentToken = assignmentToken;
    assignOpNode->expression = right;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 218 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = ASSIGNMENT_NODE;
    node->node = malloc(sizeof(AssignOpNode));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 225 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->assignOpNode = assignOpNode;

    return node;
}

ASTNode * create_statements_node_list(ASTNode ** nodes, unsigned short capacity, unsigned short size){
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
        exit(EXIT_FAILURE);
    }
    ast_node->node->stmtListNode = malloc(sizeof(StatementsListNode));
    if(ast_node->node->stmtListNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 250 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    ast_node->node->stmtListNode->nodes = malloc(capacity * sizeof(ASTNode *));
    if(ast_node->node->stmtListNode->nodes == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 257 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    for(unsigned short idx=0; idx < size; ++idx){
        ast_node->node->stmtListNode->nodes[idx] = nodes[idx];
    }

    ast_node->node->stmtListNode->capacity=capacity;
    ast_node->node->stmtListNode->size=size;

    return ast_node;
}

ASTNode * create_print_node(ASTNode * exprNode){
    PrintNode * printNode = malloc(sizeof(PrintNode));
    if(printNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 274 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    printNode->expression = exprNode;

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 282 :"
                        "Memory allocation failed.");
        free(printNode->expression);
        free(printNode);
        exit(EXIT_FAILURE);
    }
    node->type = PRINT_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 289 :"
                        "Memory allocation failed.");
        free(node);
        free(printNode->expression);
        free(printNode);
        exit(EXIT_FAILURE);
    }
    node->node->printNode = printNode;
    return node;
}

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

    if(node->node->printNode->expression){
        free_node(node->node->printNode->expression);
        node->node->printNode->expression = NULL;
    }

    free(node->node->printNode);
    free(node->node);
    free(node);
}


ASTNode * create_empty_node(){
    EmptyNode * emptyNode = malloc(sizeof(EmptyNode *));
    if(emptyNode == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 324 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    ASTNode * node = malloc(sizeof(ASTNode));
    if(node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 331 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = EMPTY_NODE;
    node->node = malloc(sizeof(NodeUnion *));
    if(node->node == NULL){
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 338 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->emptyNode = emptyNode;

    return node;
}

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

void free_statements_list_node(ASTNode * node){
    if(node == NULL) return;
    if(!node->type || node->type != STATEMENTS_LIST_NODE) return;
    if(node->node == NULL) return;
    if(node->node->stmtListNode == NULL) return;
    unsigned short capacity = node->node->stmtListNode->capacity;
    unsigned short size = node->node->stmtListNode->size;
    if( capacity < 0 || size < 0){
        if(node->node->stmtListNode->nodes != NULL) free(node->node->stmtListNode->nodes);
        free(node->node->stmtListNode);
        free(node->node);
        free(node);
        return;
    }
    for(unsigned short idx = 0; idx < size; ++idx){
        ASTNode * node_to_free = node->node->stmtListNode->nodes[idx];
        if(node_to_free != NULL){
//            printf("\n Freing the node with index from the statements list: %d", idx);
//            printf("\n size of the node list is : %d", size);
//            printf("\n capacity of the node list is : %d", capacity);
            free_node(node_to_free);
            node_to_free = NULL;
        }
    }

    free(node->node->stmtListNode->nodes);
    free(node->node->stmtListNode);
    free(node->node);
    free(node);

    return;
}

void free_node(ASTNode * node){
    if(node == NULL) return;
    switch(node->type){
        case NUMBER_NODE:
            free_number_node(node);
            break;
        case BINARY_OPERATOR_NODE:
            free_binary_operator_node(node);
            break;
        case UNARY_OPERATOR_NODE:
            free_unary_operator_node(node);
            break;
        case VARIABLE_NODE:
            free_variable_node(node);
            break;
        case ASSIGNMENT_NODE:
            free_assignment_node(node);
            break;
        case PRINT_NODE:
            free_print_node(node);
            break;
        case STATEMENTS_LIST_NODE:
            free_statements_list_node(node);
            break;
        default:
            fprintf(stderr, "\n In free_node : Error : invalid node cannot be deallocated");
            exit(EXIT_FAILURE);
    }
}
