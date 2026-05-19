#include "semantic.h"

#include "symbol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PearlSymbolEntry {
    PearlSymbol symbol;
    struct PearlSymbolEntry *next;
} PearlSymbolEntry;

typedef struct PearlScope {
    PearlSymbolEntry *symbols;
    struct PearlScope *parent;
} PearlScope;

struct PearlSymbolTable {
    PearlScope *current;
};

static PearlSymbolTable *g_last_table = NULL;
static const char *g_source_name = NULL;
static FILE *g_output = NULL;
static int g_error_count = 0;

static void report_error(const char *message) {
    if (g_output != NULL) {
        if (g_source_name != NULL) {
            fprintf(g_output, "%s: %s\n", g_source_name, message);
        } else {
            fprintf(g_output, "%s\n", message);
        }
    }

    ++g_error_count;
}

static PearlScope *scope_create(PearlScope *parent) {
    PearlScope *scope = calloc(1, sizeof(*scope));

    if (scope == NULL) {
        return NULL;
    }

    scope->parent = parent;
    return scope;
}

static void scope_free(PearlScope *scope) {
    PearlSymbolEntry *entry = NULL;
    PearlSymbolEntry *next = NULL;

    if (scope == NULL) {
        return;
    }

    entry = scope->symbols;

    while (entry != NULL) {
        next = entry->next;
        free(entry);
        entry = next;
    }

    free(scope);
}

static PearlSymbolEntry *scope_find(PearlScope *scope, const char *name) {
    PearlSymbolEntry *entry = NULL;

    if (scope == NULL || name == NULL) {
        return NULL;
    }

    entry = scope->symbols;

    while (entry != NULL) {
        if (strcmp(entry->symbol.name, name) == 0) {
            return entry;
        }

        entry = entry->next;
    }

    return NULL;
}

PearlSymbolTable *pearl_symbol_table_create(void) {
    PearlSymbolTable *table = calloc(1, sizeof(*table));

    if (table == NULL) {
        return NULL;
    }

    table->current = scope_create(NULL);

    if (table->current == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

void pearl_symbol_table_free(PearlSymbolTable *table) {
    PearlScope *scope = NULL;
    PearlScope *next = NULL;

    if (table == NULL) {
        return;
    }

    scope = table->current;

    while (scope != NULL) {
        next = scope->parent;
        scope_free(scope);
        scope = next;
    }

    free(table);
}

int pearl_symbol_table_push_scope(PearlSymbolTable *table) {
    PearlScope *scope = NULL;

    if (table == NULL) {
        return 1;
    }

    scope = scope_create(table->current);

    if (scope == NULL) {
        return 1;
    }

    table->current = scope;
    return 0;
}

int pearl_symbol_table_pop_scope(PearlSymbolTable *table) {
    PearlScope *scope = NULL;

    if (table == NULL || table->current == NULL || table->current->parent == NULL) {
        return 1;
    }

    scope = table->current;
    table->current = scope->parent;
    scope_free(scope);
    return 0;
}

int pearl_symbol_table_declare(PearlSymbolTable *table, PearlSymbolKind kind, const char *name) {
    PearlSymbolEntry *entry = NULL;

    if (table == NULL || table->current == NULL || name == NULL) {
        return 1;
    }

    if (scope_find(table->current, name) != NULL) {
        return 1;
    }

    entry = calloc(1, sizeof(*entry));

    if (entry == NULL) {
        return 1;
    }

    entry->symbol.kind = kind;
    entry->symbol.name = name;
    entry->next = table->current->symbols;
    table->current->symbols = entry;
    return 0;
}

int pearl_symbol_table_resolve(PearlSymbolTable *table, const char *name) {
    PearlScope *scope = NULL;

    if (table == NULL || name == NULL) {
        return 0;
    }

    scope = table->current;

    while (scope != NULL) {
        if (scope_find(scope, name) != NULL) {
            return 1;
        }

        scope = scope->parent;
    }

    return 0;
}

static int check_expr(const PearlAstNode *node, PearlSymbolTable *table);
static int check_block(const PearlAstNode *node, PearlSymbolTable *table, int new_scope);

static int check_call(const PearlAstNode *node, PearlSymbolTable *table) {
    const PearlAstNode *args = NULL;

    if (node == NULL || node->text == NULL) {
        return 0;
    }

    if (strcmp(node->text, "print") != 0 && strcmp(node->text, "justPrint") != 0 && !pearl_symbol_table_resolve(table, node->text)) {
        report_error("unknown function or symbol in call");
        return 1;
    }

    args = node->first_child;

    while (args != NULL) {
        if (check_expr(args, table) != 0) {
            return 1;
        }

        args = args->next;
    }

    return 0;
}

static int check_expr(const PearlAstNode *node, PearlSymbolTable *table) {
    const PearlAstNode *child = NULL;

    if (node == NULL) {
        return 0;
    }

    switch (node->kind) {
        case PEARL_AST_LITERAL:
            return 0;
        case PEARL_AST_IDENTIFIER:
            if (!pearl_symbol_table_resolve(table, node->text)) {
                report_error("unknown identifier");
                return 1;
            }

            return 0;
        case PEARL_AST_BINARY:
        case PEARL_AST_UNARY:
            child = node->first_child;

            while (child != NULL) {
                if (check_expr(child, table) != 0) {
                    return 1;
                }

                child = child->next;
            }

            return 0;
        case PEARL_AST_CALL:
            return check_call(node, table);
        default:
            child = node->first_child;

            while (child != NULL) {
                if (check_expr(child, table) != 0) {
                    return 1;
                }

                child = child->next;
            }

            return 0;
    }
}

static int check_statement(const PearlAstNode *node, PearlSymbolTable *table) {
    const PearlAstNode *child = NULL;

    if (node == NULL) {
        return 0;
    }

    switch (node->kind) {
        case PEARL_AST_VAR_DECL:
            if (pearl_symbol_table_declare(table, PEARL_SYMBOL_VARIABLE, node->text) != 0) {
                report_error("duplicate variable name in the same scope");
                return 1;
            }

            return check_expr(node->first_child, table);
        case PEARL_AST_ASSIGNMENT:
            if (!pearl_symbol_table_resolve(table, node->text)) {
                report_error("assignment to unknown name");
                return 1;
            }

            return check_expr(node->first_child, table);
        case PEARL_AST_CALL:
            return check_call(node, table);
        case PEARL_AST_RETURN:
            return check_expr(node->first_child, table);
        case PEARL_AST_IF:
        case PEARL_AST_WHILE:
        case PEARL_AST_FOR:
        case PEARL_AST_BLOCK:
            child = node->first_child;

            while (child != NULL) {
                if (check_statement(child, table) != 0) {
                    return 1;
                }

                child = child->next;
            }

            return 0;
        default:
            child = node->first_child;

            while (child != NULL) {
                if (check_statement(child, table) != 0) {
                    return 1;
                }

                child = child->next;
            }

            return 0;
    }
}

static int check_block(const PearlAstNode *node, PearlSymbolTable *table, int new_scope) {
    const PearlAstNode *child = NULL;

    if (node == NULL) {
        return 0;
    }

    if (new_scope && pearl_symbol_table_push_scope(table) != 0) {
        report_error("failed to create scope");
        return 1;
    }

    child = node->first_child;

    while (child != NULL) {
        if (check_statement(child, table) != 0) {
            if (new_scope) {
                pearl_symbol_table_pop_scope(table);
            }

            return 1;
        }

        child = child->next;
    }

    if (new_scope) {
        pearl_symbol_table_pop_scope(table);
    }

    return 0;
}

static int declare_function(const PearlAstNode *node, PearlSymbolTable *table) {
    if (node == NULL || node->text == NULL) {
        return 0;
    }

    if (pearl_symbol_table_declare(table, PEARL_SYMBOL_FUNCTION, node->text) != 0) {
        report_error("duplicate function name");
        return 1;
    }

    return 0;
}

static int check_function(const PearlAstNode *node, PearlSymbolTable *table) {
    const PearlAstNode *params = NULL;
    const PearlAstNode *body = NULL;
    const PearlAstNode *param = NULL;

    if (node == NULL || node->text == NULL) {
        return 0;
    }

    if (pearl_symbol_table_push_scope(table) != 0) {
        report_error("failed to create function scope");
        return 1;
    }

    params = node->first_child;
    body = params != NULL ? params->next : NULL;

    if (params != NULL) {
        param = params->first_child;

        while (param != NULL) {
            if (pearl_symbol_table_declare(table, PEARL_SYMBOL_PARAMETER, param->text) != 0) {
                report_error("duplicate parameter name");
                pearl_symbol_table_pop_scope(table);
                return 1;
            }

            param = param->next;
        }
    }

    if (check_block(body, table, 1) != 0) {
        pearl_symbol_table_pop_scope(table);
        return 1;
    }

    pearl_symbol_table_pop_scope(table);
    return 0;
}

int pearl_check_ast(FILE *output, const char *source_name, const PearlAstNode *root) {
    const PearlAstNode *functions = NULL;
    const PearlAstNode *function = NULL;
    PearlSymbolTable *table = NULL;
    int result = 0;

    g_output = output;
    g_source_name = source_name;
    g_error_count = 0;

    table = pearl_symbol_table_create();

    if (table == NULL) {
        report_error("failed to create symbol table");
        return 1;
    }

    g_last_table = table;

    if (root == NULL || root->first_child == NULL) {
        pearl_symbol_table_free(table);
        return 0;
    }

    functions = root->first_child;
    function = functions->first_child;

    while (function != NULL) {
        if (function->kind == PEARL_AST_FUNCTION) {
            if (declare_function(function, table) != 0) {
                result = 1;
            }
        }

        function = function->next;
    }

    if (result != 0) {
        pearl_symbol_table_free(table);
        g_last_table = NULL;
        return result;
    }

    function = functions->first_child;

    while (function != NULL) {
        if (function->kind == PEARL_AST_FUNCTION) {
            if (check_function(function, table) != 0) {
                result = 1;
                break;
            }
        }

        function = function->next;
    }

    pearl_symbol_table_free(table);
    g_last_table = NULL;

    return result;
}