#include "ir.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char *pearl_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    if (r) memcpy(r, s, n);
    return r;
}

static int temp_counter = 0;
static int label_counter = 0;

static char *new_temp(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "t%d", ++temp_counter);
    return pearl_strdup(buf);
}

static char *new_label(const char *base) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s%d", base, ++label_counter);
    return pearl_strdup(buf);
}

/* Lower an expression and return a string naming the value (temp or literal/name).
 * Caller must free the returned string.
 */
static char *lower_expr(PearlAstNode *node, FILE *out) {
    if (!node) return pearl_strdup("<nil>");

    switch (node->kind) {
    case PEARL_AST_LITERAL:
        return pearl_strdup(node->text ? node->text : "<lit>");
    case PEARL_AST_IDENTIFIER:
        return pearl_strdup(node->text ? node->text : "<id>");
    case PEARL_AST_BINARY: {
        PearlAstNode *l = node->first_child;
        PearlAstNode *r = l ? l->next : NULL;
        char *left = lower_expr(l, out);
        char *right = lower_expr(r, out);
        char *tmp = new_temp();
        const char *op = node->text ? node->text : "?";
        fprintf(out, "%s = %s %s %s\n", tmp, left, op, right);
        free(left);
        free(right);
        return tmp;
    }
    case PEARL_AST_UNARY: {
        PearlAstNode *r = node->first_child;
        char *val = lower_expr(r, out);
        char *tmp = new_temp();
        const char *op = node->text ? node->text : "u?";
        fprintf(out, "%s = %s %s\n", tmp, op, val);
        free(val);
        return tmp;
    }
    case PEARL_AST_CALL: {
        const char *fn = node->text ? node->text : "<call>";
        /* collect args */
        int argc = 0;
        for (PearlAstNode *a = node->first_child; a; a = a->next) argc++;
        char **args = calloc(argc, sizeof(char*));
        int i = 0;
        for (PearlAstNode *a = node->first_child; a; a = a->next) {
            args[i++] = lower_expr(a, out);
        }
        if (strcmp(fn, "print") == 0 || strcmp(fn, "justPrint") == 0) {
            fprintf(out, "print");
            for (int j = 0; j < argc; ++j) fprintf(out, " %s", args[j]);
            fprintf(out, "\n");
            for (int j = 0; j < argc; ++j) free(args[j]);
            free(args);
            return pearl_strdup("<void>");
        } else {
            char *tmp = new_temp();
            fprintf(out, "%s = call %s", tmp, fn);
            for (int j = 0; j < argc; ++j) fprintf(out, " %s", args[j]);
            fprintf(out, "\n");
            for (int j = 0; j < argc; ++j) free(args[j]);
            free(args);
            return tmp;
        }
    }
    default:
        return pearl_strdup("<expr>");
    }
}

static void lower_node(PearlAstNode *node, FILE *out);

static void lower_block(PearlAstNode *block, FILE *out) {
    for (PearlAstNode *c = block ? block->first_child : NULL; c; c = c->next) {
        lower_node(c, out);
    }
}

static void lower_node(PearlAstNode *node, FILE *out) {
    if (!node) return;

    switch (node->kind) {
    case PEARL_AST_PROGRAM: {
        fprintf(out, ";; program\n");
        for (PearlAstNode *c = node->first_child; c; c = c->next) lower_node(c, out);
        break;
    }
    case PEARL_AST_FUNCTION: {
        const char *name = node->text ? node->text : "<anon>";
        fprintf(out, "func %s\n", name);
        /* params are first child, body second */
        PearlAstNode *params = node->first_child;
        PearlAstNode *body = params ? params->next : NULL;
        /* list params */
        fprintf(out, "  ; params");
        for (PearlAstNode *p = params ? params->first_child : NULL; p; p = p->next) {
            if (p->text) fprintf(out, " %s", p->text);
        }
        fprintf(out, "\n");
        lower_block(body, out);
        fprintf(out, "endfunc\n");
        break;
    }
    case PEARL_AST_BLOCK: {
        lower_block(node, out);
        break;
    }
    case PEARL_AST_VAR_DECL: {
        const char *name = node->text ? node->text : "<var>";
        PearlAstNode *expr = node->first_child;
        char *val = lower_expr(expr, out);
        fprintf(out, "var %s %s\n", name, val);
        free(val);
        break;
    }
    case PEARL_AST_ASSIGNMENT: {
        const char *name = node->text ? node->text : "<var>";
        PearlAstNode *expr = node->first_child;
        char *val = lower_expr(expr, out);
        fprintf(out, "set %s %s\n", name, val);
        free(val);
        break;
    }
    case PEARL_AST_CALL: {
        /* expression-call lowering handled in lower_expr where needed */
        /* but if call appears as statement, just evaluate and drop result */
        char *res = lower_expr(node, out);
        if (res) free(res);
        break;
    }
    case PEARL_AST_RETURN: {
        PearlAstNode *expr = node->first_child;
        if (expr) {
            char *val = lower_expr(expr, out);
            fprintf(out, "ret %s\n", val);
            free(val);
        } else {
            fprintf(out, "ret\n");
        }
        break;
    }
    case PEARL_AST_IF: {
        PearlAstNode *cond = node->first_child;
        PearlAstNode *thenb = cond ? cond->next : NULL;
        PearlAstNode *elseb = thenb ? thenb->next : NULL;
        char *c = lower_expr(cond, out);
        fprintf(out, "if %s then\n", c);
        free(c);
        lower_block(thenb, out);
        if (elseb) {
            fprintf(out, "else\n");
            lower_block(elseb, out);
        }
        fprintf(out, "endif\n");
        break;
    }
    case PEARL_AST_WHILE: {
        PearlAstNode *cond = node->first_child;
        PearlAstNode *body = cond ? cond->next : NULL;
        char *start = new_label("L");
        char *end = new_label("L");
        fprintf(out, "%s:\n", start);
        char *c = lower_expr(cond, out);
        fprintf(out, "br_if %s %s\n", c, end);
        free(c);
        lower_block(body, out);
        fprintf(out, "jmp %s\n", start);
        fprintf(out, "%s:\n", end);
        free(start);
        free(end);
        break;
    }
    case PEARL_AST_FOR: {
        /* children: init, cond, step, body */
        PearlAstNode *init = node->first_child;
        PearlAstNode *cond = init ? init->next : NULL;
        PearlAstNode *step = cond ? cond->next : NULL;
        PearlAstNode *body = step ? step->next : NULL;
        if (init) lower_node(init, out);
        char *start = new_label("L");
        char *end = new_label("L");
        fprintf(out, "%s:\n", start);
        if (cond) {
            char *c = lower_expr(cond, out);
            fprintf(out, "br_if %s %s\n", c, end);
            free(c);
        }
        lower_block(body, out);
        if (step) lower_node(step, out);
        fprintf(out, "jmp %s\n", start);
        fprintf(out, "%s:\n", end);
        free(start);
        free(end);
        break;
    }
    default: {
        /* fallback: recurse children */
        for (PearlAstNode *c = node->first_child; c; c = c->next) lower_node(c, out);
    }
    }
}

int pearl_lower_ast(PearlAstNode *ast, FILE *out) {
    if (!ast || !out) return 1;
    temp_counter = 0;
    label_counter = 0;
    lower_node(ast, out);
    return 0;
}
