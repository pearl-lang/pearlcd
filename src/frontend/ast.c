#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *pearl_strdup(const char *text) {
    size_t size = 0;
    char *copy = NULL;

    if (text == NULL) {
        return NULL;
    }

    size = strlen(text) + 1;
    copy = malloc(size);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, text, size);
    return copy;
}

static const char *kind_name(PearlAstKind kind) {
    switch (kind) {
        case PEARL_AST_PROGRAM: return "Program";
        case PEARL_AST_FUNCTION: return "Function";
        case PEARL_AST_PARAMS: return "Params";
        case PEARL_AST_BLOCK: return "Block";
        case PEARL_AST_VAR_DECL: return "VarDecl";
        case PEARL_AST_ASSIGNMENT: return "Assignment";
        case PEARL_AST_CALL: return "Call";
        case PEARL_AST_RETURN: return "Return";
        case PEARL_AST_IF: return "If";
        case PEARL_AST_WHILE: return "While";
        case PEARL_AST_FOR: return "For";
        case PEARL_AST_BINARY: return "Binary";
        case PEARL_AST_UNARY: return "Unary";
        case PEARL_AST_IDENTIFIER: return "Identifier";
        case PEARL_AST_LITERAL: return "Literal";
    }

    return "Node";
}

PearlAstNode *pearl_ast_create(PearlAstKind kind, const char *text) {
    PearlAstNode *node = calloc(1, sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->kind = kind;
    node->text = pearl_strdup(text);
    return node;
}

void pearl_ast_append_child(PearlAstNode *parent, PearlAstNode *child) {
    if (parent == NULL || child == NULL) {
        return;
    }

    if (parent->first_child == NULL) {
        parent->first_child = child;
        parent->last_child = child;
        return;
    }

    parent->last_child->next = child;
    parent->last_child = child;
}

static void print_indent(int indent) {
    for (int index = 0; index < indent; ++index) {
        fputs("  ", stdout);
    }
}

void pearl_ast_print(const PearlAstNode *node, int indent) {
    const PearlAstNode *child = NULL;

    if (node == NULL) {
        return;
    }

    print_indent(indent);

    if (node->text != NULL) {
        printf("%s %s\n", kind_name(node->kind), node->text);
    } else {
        printf("%s\n", kind_name(node->kind));
    }

    child = node->first_child;

    while (child != NULL) {
        pearl_ast_print(child, indent + 1);
        child = child->next;
    }
}

void pearl_ast_free(PearlAstNode *node) {
    PearlAstNode *child = NULL;
    PearlAstNode *next = NULL;

    if (node == NULL) {
        return;
    }

    child = node->first_child;

    while (child != NULL) {
        next = child->next;
        pearl_ast_free(child);
        child = next;
    }

    free(node->text);
    free(node);
}