#include "abstract_syntax_tree.h"
#include <stdlib.h>

/**
 * Creates a new AST node for a number.
 * @param tokenType - The type of token (e.g., integer, float).
 * @param valueType - The data type of the value (e.g., INT, DOUBLE).
 * @param value - The numeric value.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_number_node(TokenType tokenType, ValueType valueType, int value)
{
    NumberNode *numNode = malloc(sizeof(NumberNode));
    if (numNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node Line 10:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }

    char *intStrValue = calloc(24, sizeof(char));
    if (intStrValue == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node: Memory allocation "
                        "failed when trying to create string value.\n");
        free(numNode);
        exit(EXIT_FAILURE);
    }
    sprintf(intStrValue, "%d", value);
    Token *token = create_token(tokenType, valueType, intStrValue);
    free(intStrValue);

    if (token == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node: Failed to create token.\n");
        free(intStrValue);
        free(numNode);
        exit(EXIT_FAILURE);
    }
    numNode->token = token;
    numNode->value = value;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_number_node Line 23:"
                        "Memory allocation failed when trying to create node.");
        free_token(token);
        free(numNode);
        exit(EXIT_FAILURE);
    }
    node->type = NUMBER_NODE;
    node->node = malloc(sizeof(NumberNode));
    if (node->node == NULL)
    {
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
 * Creates a new AST node for a binary operation.
 * @param opToken - The token representing the operator.
 * @param left - The left operand.
 * @param right - The right operand.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_binary_operator_node(Token *opToken, ASTNode *left, ASTNode *right)
{
    BinaryOpNode *binOpNode = malloc(sizeof(BinaryOpNode));
    if (binOpNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 31:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    binOpNode->left = left;
    binOpNode->operator= opToken;
    binOpNode->right = right;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 42:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    node->type = BINARY_OPERATOR_NODE;
    node->node = malloc(sizeof binOpNode);
    if (node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_binary_operator_node Line 58:"
                        "Memory allocation failed when trying to create node.");
        exit(EXIT_FAILURE);
    }
    node->node->binaryOpNode = binOpNode;
    return node;
}

/**
 * Frees a binary operator node and all its children.
 * @param node - The binary operator node to free.
 */

void free_binary_operator_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != BINARY_OPERATOR_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        return;
    }

    if (node->node->binaryOpNode == NULL)
    {
        free(node->node);
        free(node);
        return;
    }

    if (node->node->binaryOpNode->operator!= NULL)
    {
        free_token(node->node->binaryOpNode->operator);
        node->node->binaryOpNode->operator= NULL;
    }

    if (node->node->binaryOpNode->left != NULL)
    {
        free_node(node->node->binaryOpNode->left);
        node->node->binaryOpNode->left = NULL;
    }

    if (node->node->binaryOpNode->right != NULL)
    {
        free_node(node->node->binaryOpNode->right);
        node->node->binaryOpNode->left = NULL;
    }

    free(node->node->binaryOpNode);
    free(node->node);
    free(node);
}

/**
 * Creates a new AST node for a unary operation.
 * @param token - The token representing the unary operator.
 * @param expression - The operand for the unary operator.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_unary_operator_node(Token *token, ASTNode *expression)
{
    UnaryOpNode *unaryOpNode = malloc(sizeof(UnaryOpNode));
    if (unaryOpNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 101:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    unaryOpNode->operator= token;
    unaryOpNode->expression = expression;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = UNARY_OPERATOR_NODE;
    node->node = malloc(sizeof(UnaryOpNode));
    if (node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_unary_operator_node Line 110:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->unaryOpNode = unaryOpNode;
    return node;
}

/**
 *
 * Frees a unary operator AST node and its child nodes.
 *
 * @param node - The unary operator AST node to free.
 */

void free_unary_operator_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != UNARY_OPERATOR_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        return;
    }
    if (node->node->unaryOpNode == NULL)
    {
        free(node->node);
        free(node);
        return;
    }

    if (node->node->unaryOpNode->operator!= NULL)
    {
        free_token(node->node->unaryOpNode->operator);
    }

    if (node->node->unaryOpNode->expression != NULL)
    {
        free_node(node->node->unaryOpNode->expression);
    }

    free(node->node->unaryOpNode);
    free(node->node);
    free(node);
}

/**
 * Creates a new AST node for a variable.
 * @param varToken - The token representing the variable.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_variable_node(Token *varToken)
{
    VariableNode *varNode = malloc(sizeof(VariableNode));
    if (varNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 156:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    varNode->varToken = varToken;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 164:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = VARIABLE_NODE;
    node->node = malloc(sizeof(VariableNode));
    if (node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_variable_node Line 171:"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->variableNode = varNode;

    return node;
}

/**
 * Frees a variable node.
 * @param node - The variable node to free.
 */

void free_variable_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != VARIABLE_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        return;
    }

    if (node->node->variableNode == NULL)
    {
        free(node->node);
        free(node);
        return;
    }

    if (node->node->variableNode->varToken != NULL)
    {
        free_token(node->node->variableNode->varToken);
    }

    free(node->node->variableNode);
    free(node->node);
    free(node);
}

/**
 * Creates a new AST node for an assignment operation.
 * @param left - The variable being assigned.
 * @param assignmentToken - The token representing the assignment (=).
 * @param right - The value to assign.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_assignment_node(ASTNode *left, Token *assignmentToken, ASTNode *right)
{
    AssignOpNode *assignOpNode = malloc(sizeof(AssignOpNode));
    if (assignOpNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 208 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    assignOpNode->identifier = left;
    assignOpNode->assignmentToken = assignmentToken;
    assignOpNode->expression = right;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 218 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = ASSIGNMENT_NODE;
    node->node = malloc(sizeof(AssignOpNode));
    if (node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_assignment_node Line 225 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->assignOpNode = assignOpNode;

    return node;
}

/**
 * Creates a new AST node for a list of statements.
 * @param nodes - Array of AST nodes representing statements.
 * @param capacity - The capacity of the array.
 * @param size - The current number of statements.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_statements_node_list(ASTNode **nodes, unsigned short capacity, unsigned short size)
{
    ASTNode *ast_node = malloc(sizeof(ASTNode));
    if (ast_node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 237 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    ast_node->type = STATEMENTS_LIST_NODE;
    ast_node->node = malloc(sizeof(NodeUnion));
    if (ast_node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 244 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    ast_node->node->stmtListNode = malloc(sizeof(StatementsListNode));
    if (ast_node->node->stmtListNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 250 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    ast_node->node->stmtListNode->nodes = malloc(capacity * sizeof(ASTNode *));
    if (ast_node->node->stmtListNode->nodes == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_statements_node_list Line 257 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    for (unsigned short idx = 0; idx < size; ++idx)
    {
        ast_node->node->stmtListNode->nodes[idx] = nodes[idx];
    }

    ast_node->node->stmtListNode->capacity = capacity;
    ast_node->node->stmtListNode->size = size;

    return ast_node;
}

/**
 * Creates a new AST node for a print statement.
 * @param exprNode - The expression to print.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_print_node(ASTNode *exprNode)
{
    PrintNode *printNode = malloc(sizeof(PrintNode));
    if (printNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 274 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    printNode->expression = exprNode;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_print_node Line 282 :"
                        "Memory allocation failed.");
        free(printNode->expression);
        free(printNode);
        exit(EXIT_FAILURE);
    }
    node->type = PRINT_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if (node->node == NULL)
    {
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

/**
 * Frees a print node and its children.
 * @param node - The print node to free.
 */

void free_print_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != PRINT_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        return;
    }

    if (node->node->printNode == NULL)
    {
        free(node->node);
        free(node);
        return;
    }

    if (node->node->printNode->expression)
    {
        free_node(node->node->printNode->expression);
        node->node->printNode->expression = NULL;
    }

    free(node->node->printNode);
    free(node->node);
    free(node);
}

/**
 * Creates a new empty AST node.
 * @return A pointer to the created ASTNode.
 */

ASTNode *create_empty_node()
{
    EmptyNode *emptyNode = malloc(sizeof(EmptyNode));
    if (emptyNode == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 350 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 357 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->type = EMPTY_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if (node->node == NULL)
    {
        fprintf(stderr, "\nIn abstract_syntax_tree.c in function create_empty_node Line 366 :"
                        "Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    node->node->emptyNode = emptyNode;

    return node;
}

/**
 *
 * Frees a number AST node and its associated token.
 *
 * @param node - The number AST node to free.
 */

void free_number_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != NUMBER_NODE)
        return;
    if (node->node == NULL)
    {
        printf("\n Unexpected node->node was null");
        free(node);
        return;
    }
    if (node->node->numNode == NULL)
    {
        printf("\n Unexpected node->node->numNode was null");
        free(node->node);
        free(node);
        return;
    }
    if (node->node->numNode->token != NULL)
    {
        free_token(node->node->numNode->token);
        node->node->numNode->token = NULL;
    }
    free(node->node->numNode);
    free(node->node);
    free(node);
}

/**
 * Frees an assignment node and its children.
 * @param node - The assignment node to free.
 */

void free_assignment_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (node->type != ASSIGNMENT_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        return;
    }
    if (node->node->assignOpNode == NULL)
    {
        free(node->node);
        free(node);
        return;
    }
    if (node->node->assignOpNode->assignmentToken != NULL)
    {
        free_token(node->node->assignOpNode->assignmentToken);
    }

    if (node->node->assignOpNode->identifier)
    {
        free_node(node->node->assignOpNode->identifier);
    }
    if (node->node->assignOpNode->expression)
    {
        free_node(node->node->assignOpNode->expression);
    }

    free(node->node->assignOpNode);
    free(node->node);
    free(node);
}

/**
 * Frees a statement list node and its child nodes.
 * @param node - The statement list node to free.
 */

void free_statements_list_node(ASTNode *node)
{
    if (node == NULL)
        return;
    if (!node->type || node->type != STATEMENTS_LIST_NODE)
        return;
    if (node->node == NULL)
        return;
    if (node->node->stmtListNode == NULL)
        return;
    unsigned short capacity = node->node->stmtListNode->capacity;
    unsigned short size = node->node->stmtListNode->size;

    if (capacity < 0 || size < 0)
    {
        if (node->node->stmtListNode->nodes != NULL)
            free(node->node->stmtListNode->nodes);
        free(node->node->stmtListNode);
        free(node->node);
        free(node);
        return;
    }
    for (unsigned short idx = 0; idx < size; ++idx)
    {
        ASTNode *node_to_free = node->node->stmtListNode->nodes[idx];
        if (node_to_free != NULL)
        {
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

void free_empty_node(ASTNode *node)
{
    printf("\nFreeing empty node");
    if (node == NULL)
        return;
    if (node->type != EMPTY_NODE)
        return;
    if (node->node == NULL)
    {
        free(node);
        node = NULL;
        return;
    }
    if (node->node->emptyNode == NULL)
    {
        free(node->node);
        node->node = NULL;
        free(node);
        node = NULL;
        return;
    }

    free(node->node->emptyNode);
    node->node->emptyNode = NULL;
    free(node->node);
    node->node = NULL;
    free(node);
    node = NULL;
}

/**
 * Frees an AST node, determining its type to properly free child nodes.
 * @param node - The AST node to free.
 */

void free_node(ASTNode *node) {
    if (!node) return;

    // Verify if type is valid
    if (node->type < NUMBER_NODE || node->type > FOR_NODE)
    {
        fprintf(stderr, "Error: invalid node type (%d) cannot be deallocated.\n", node->type);
        return;
    }

    switch (node->type)
    {
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
    case EMPTY_NODE:
        free_empty_node(node);
    case WHILE_NODE:
        free_while_node(node);
        break;
    case FOR_NODE:
        free_for_node(node);
        break;
    default:
        fprintf(stderr, "Error: invalid node type (%d) cannot be deallocated.\n", node->type);
    }
}

/**
 *
 * Creates a new AST node for a while loop.
 *
 * @param condition - The condition AST node for the while loop.
 * @param body - The body AST node (statements to execute while condition is true).
 * @return - A pointer to the created ASTNode.
 */

ASTNode *create_while_node(ASTNode *condition, ASTNode *body)
{
    if (!condition || !body)
    {
        fprintf(stderr, "Error: Invalid parameters passed to create_while_node.\n");
        return NULL;
    }

    WhileNode *whileNode = malloc(sizeof(WhileNode));
    if (!whileNode)
    {
        fprintf(stderr, "Memory allocation failed for WhileNode.\n");
        return NULL;
    }
    whileNode->condition = condition;
    whileNode->body = body;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
    {
        fprintf(stderr, "Memory allocation failed for ASTNode.\n");
        free(whileNode);
        return NULL;
    }

    node->type = WHILE_NODE;

    node->node = malloc(sizeof(NodeUnion));
    if (!node->node)
    {
        fprintf(stderr, "Memory allocation failed for NodeUnion.\n");
        free(whileNode);
        free(node);
        return NULL;
    }

    node->node->whileNode = whileNode;

    return node;
}

/**
 *
 * Creates a new AST node for a for loop.
 *
 * @param initialisation - The initialization AST node for the loop.
 * @param condition - The condition AST node for the loop continuation.
 * @param incrementation - The increment AST node for the loop variable.
 * @param body - The body AST node (statements to execute in each iteration).
 * @return - A pointer to the created ASTNode.
 */

ASTNode *create_for_node(ASTNode *initialisation, ASTNode *condition, ASTNode *incrementation, ASTNode *body)
{
    if (!initialisation || !condition || !incrementation || !body)
    {
        return NULL;
    }

    ForNode *forNode = malloc(sizeof(ForNode));
    if (!forNode)
    {
        exit(EXIT_FAILURE);
    }

    forNode->initialisation = initialisation;
    forNode->condition = condition;
    forNode->incrementation = incrementation;
    forNode->body = body;

    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
    {
        free(forNode);
        exit(EXIT_FAILURE);
    }

    node->type = FOR_NODE;
    node->node = malloc(sizeof(NodeUnion));
    if (!node->node)
    {
        free(forNode);
        free(node);
        exit(EXIT_FAILURE);
    }

    node->node->forNode = forNode;
    return node;
}

/**
 *
 * Frees a for loop AST node and its child nodes.
 *
 * @param node - The for loop AST node to free.
 */

void free_for_node(ASTNode *node)
{
    if (!node || node->type != FOR_NODE || !node->node || !node->node->forNode)
    {
        return;
    }

    ForNode *forNode = node->node->forNode;

    if (forNode->initialisation)
    {
        free_node(forNode->initialisation);
    }
    if (forNode->condition)
    {
        free_node(forNode->condition);
    }
    if (forNode->incrementation)
    {
        free_node(forNode->incrementation);
    }
    if (forNode->body)
    {
        free_node(forNode->body);
    }

    free(forNode);
    free(node->node);
    free(node);
}

/**
 *
 * Frees a while loop AST node and its child nodes.
 *
 * @param node - The while loop AST node to free.
 */

void free_while_node(ASTNode *node)
{

    if (!node || node->type != WHILE_NODE || !node->node || !node->node->whileNode)
    {
        fprintf(stderr, "Error: invalid WHILE_NODE cannot be deallocated.\n");
        return;
    }

    WhileNode *whileNode = node->node->whileNode;

    if (whileNode->condition)
    {
        free_node(whileNode->condition);
    }

    if (whileNode->body)
    {
        free_node(whileNode->body);
    }

    free(whileNode);

    free(node->node);
    free(node);
}
