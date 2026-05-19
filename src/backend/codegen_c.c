#define _GNU_SOURCE
#include "codegen_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *expr_to_c(PearlAstNode *node);

static char *expr_to_c(PearlAstNode *node) {
    if (!node) return strdup("0");
    switch (node->kind) {
    case PEARL_AST_LITERAL: {
        const char *t = node->text ? node->text : "0";
        return strdup(t);
    }
    case PEARL_AST_IDENTIFIER:
        return strdup(node->text ? node->text : "<id>");
    case PEARL_AST_BINARY: {
        PearlAstNode *l = node->first_child;
        PearlAstNode *r = l ? l->next : NULL;
        char *ls = expr_to_c(l);
        char *rs = expr_to_c(r);
        const char *op = node->text ? node->text : "?";
        size_t n = strlen(ls) + strlen(rs) + strlen(op) + 8;
        char *out = malloc(n);
        snprintf(out, n, "(%s %s %s)", ls, op, rs);
        free(ls); free(rs);
        return out;
    }
    case PEARL_AST_UNARY: {
        PearlAstNode *r = node->first_child;
        char *rs = expr_to_c(r);
        const char *op = node->text ? node->text : "u?";
        size_t n = strlen(rs) + strlen(op) + 4;
        char *out = malloc(n);
        snprintf(out, n, "(%s%s)", op, rs);
        free(rs);
        return out;
    }
    case PEARL_AST_CALL: {
        const char *fn = node->text ? node->text : "<call>";
        PearlAstNode *params = node->first_child;
        // If first child is PARAMS, use its children
        PearlAstNode *args_list = (params && params->kind == PEARL_AST_PARAMS) ? params->first_child : params;
        
        size_t bufsize = 64;
        char *buf = malloc(bufsize);
        buf[0] = '\0';
        for (PearlAstNode *a = args_list; a; a = a->next) {
            char *as = expr_to_c(a);
            size_t current_len = strlen(buf);
            size_t as_len = strlen(as);
            size_t need = current_len + as_len + 2;
            if (need > bufsize) { bufsize = need + 64; buf = realloc(buf, bufsize); }
            if (buf[0] != '\0') strcat(buf, ",");
            strcat(buf, as);
            free(as);
        }
        size_t n = strlen(fn) + strlen(buf) + 8;
        char *out = malloc(n);
        snprintf(out, n, "%s(%s)", fn, buf);
        free(buf);
        return out;
    }
    case PEARL_AST_PARAMS: {
        // Handle unexpected PARAMS node in expr traversal by joining its children if any
        size_t bufsize = 64;
        char *buf = malloc(bufsize);
        buf[0] = '\0';
        for (PearlAstNode *a = node->first_child; a; a = a->next) {
            char *as = expr_to_c(a);
            size_t current_len = strlen(buf);
            size_t as_len = strlen(as);
            size_t need = current_len + as_len + 2;
            if (need > bufsize) { bufsize = need + 64; buf = realloc(buf, bufsize); }
            if (buf[0] != '\0') strcat(buf, ",");
            strcat(buf, as);
            free(as);
        }
        return buf;
    }
    default:
        return strdup("0");
    }
}

static void emit_block(PearlAstNode *block, FILE *out) {
    if (!block) return;
    for (PearlAstNode *s = block->first_child; s; s = s->next) {
        if (s->kind == PEARL_AST_BLOCK) {
            emit_block(s, out);
            continue;
        }
        switch (s->kind) {
        case PEARL_AST_VAR_DECL: {
            const char *name = s->text ? s->text : "v";
            char *val = expr_to_c(s->first_child);
            fprintf(out, "    int %s = %s;\n", name, val);
            free(val);
            break;
        }
        case PEARL_AST_ASSIGNMENT: {
            const char *name = s->text ? s->text : "v";
            char *val = expr_to_c(s->first_child);
            fprintf(out, "    %s = %s;\n", name, val);
            free(val);
            break;
        }
        case PEARL_AST_CALL: {
            const char *fn = s->text ? s->text : "call";
            if (strcmp(fn, "print") == 0 || strcmp(fn, "justPrint") == 0) {
                PearlAstNode *params = s->first_child;
                PearlAstNode *args_list = (params && params->kind == PEARL_AST_PARAMS) ? params->first_child : params;
                for (PearlAstNode *a = args_list; a; a = a->next) {
                    char *as = expr_to_c(a);
                    if (as[0] == '\"') {
                        fprintf(out, "    puts(%s);\n", as);
                    } else {
                        fprintf(out, "    printf(\"%%d\\n\", %s);\n", as);
                    }
                    free(as);
                }
            } else {
                char *call = expr_to_c(s);
                fprintf(out, "    %s;\n", call);
                free(call);
            }
            break;
        }
        case PEARL_AST_RETURN: {
            if (s->first_child) {
                char *v = expr_to_c(s->first_child);
                fprintf(out, "    return %s;\n", v);
                free(v);
            } else {
                fprintf(out, "    return 0;\n");
            }
            break;
        }
        case PEARL_AST_IF: {
            PearlAstNode *cond = s->first_child;
            PearlAstNode *thenb = cond ? cond->next : NULL;
            PearlAstNode *elseb = thenb ? thenb->next : NULL;
            char *c = expr_to_c(cond);
            fprintf(out, "    if (%s) {\n", c);
            free(c);
            emit_block(thenb, out);
            fprintf(out, "    }\n");
            if (elseb) {
                fprintf(out, "    else {\n");
                emit_block(elseb, out);
                fprintf(out, "    }\n");
            }
            break;
        }
        case PEARL_AST_WHILE: {
            PearlAstNode *cond = s->first_child;
            PearlAstNode *body = cond ? cond->next : NULL;
            char *c = expr_to_c(cond);
            fprintf(out, "    while (%s) {\n", c);
            free(c);
            emit_block(body, out);
            fprintf(out, "    }\n");
            break;
        }
        default:
            break;
        }
    }
}

int pearl_codegen_c(PearlAstNode *ast, FILE *out) {
    if (!ast || !out) return 1;
    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n\n");
    
    PearlAstNode *current = ast;
    if (current->kind == PEARL_AST_PROGRAM) {
        current = current->first_child;
    }
    
    // Skip optional Block wrapper
    if (current && current->kind == PEARL_AST_BLOCK) {
        current = current->first_child;
    }

    for (PearlAstNode *f = current; f; f = f->next) {
        if (f->kind != PEARL_AST_FUNCTION) continue;
        const char *name = f->text ? f->text : "func";
        PearlAstNode *params = f->first_child;
        PearlAstNode *body = (params && params->kind == PEARL_AST_PARAMS) ? params->next : params;
        
        fprintf(out, "int %s(", name);
        int first = 1;
        if (params && params->kind == PEARL_AST_PARAMS) {
            for (PearlAstNode *p = params->first_child; p; p = p->next) {
                if (!first) fprintf(out, ", ");
                fprintf(out, "int %s", p->text ? p->text : "p");
                first = 0;
            }
        }
        fprintf(out, ") {\n");
        emit_block(body, out);
        fprintf(out, "}\n\n");
    }
    return 0;
}
