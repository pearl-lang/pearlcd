#define _GNU_SOURCE
#include "codegen_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *pearl_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    if (r) memcpy(r, s, n);
    return r;
}

static char *expr_to_c(PearlAstNode *node);

static int is_numeric_literal(const char *text) {
    if (!text || *text == '\0') return 0;
    const char *s = text;
    if (*s == '-') s++;
    int has_digit = 0;
    while (*s) {
        if (*s >= '0' && *s <= '9') has_digit = 1;
        else return 0;
        s++;
    }
    return has_digit;
}

static int has_return(PearlAstNode *node) {
    if (!node) return 0;
    for (PearlAstNode *c = node->first_child; c; c = c->next) {
        if (c->kind == PEARL_AST_RETURN) return 1;
        if (c->kind == PEARL_AST_BLOCK || c->kind == PEARL_AST_IF || c->kind == PEARL_AST_WHILE || c->kind == PEARL_AST_FOR) {
            if (has_return(c)) return 1;
        }
    }
    return 0;
}

static char *expr_to_c(PearlAstNode *node) {
    if (!node) return pearl_strdup("0");
    switch (node->kind) {
    case PEARL_AST_LITERAL: {
        const char *t = node->text ? node->text : "0";
        return pearl_strdup(t);
    }
    case PEARL_AST_IDENTIFIER:
        return pearl_strdup(node->text ? node->text : "<id>");
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
        return pearl_strdup("0");
    }
}

static void emit_indent(FILE *out, int indent) {
    for (int i = 0; i < indent; ++i) fputs("    ", out);
}

static void emit_block_aux(PearlAstNode *block, FILE *out, int indent);

static void emit_block(PearlAstNode *block, FILE *out) {
    emit_block_aux(block, out, 1);
}

static void emit_block_aux(PearlAstNode *block, FILE *out, int indent) {
    if (!block) return;
    for (PearlAstNode *s = block->first_child; s; s = s->next) {
        if (s->kind == PEARL_AST_BLOCK) {
            emit_block_aux(s, out, indent);
            continue;
        }
        switch (s->kind) {
        case PEARL_AST_VAR_DECL: {
            const char *name = s->text ? s->text : "v";
            char *val = expr_to_c(s->first_child);
            emit_indent(out, indent);
            fprintf(out, "int %s = %s;\n", name, val);
            free(val);
            break;
        }
        case PEARL_AST_ASSIGNMENT: {
            const char *name = s->text ? s->text : "v";
            char *val = expr_to_c(s->first_child);
            emit_indent(out, indent);
            fprintf(out, "%s = %s;\n", name, val);
            free(val);
            break;
        }
        case PEARL_AST_CALL: {
            const char *fn = s->text ? s->text : "call";
            PearlAstNode *params = s->first_child;
            PearlAstNode *args_list = (params && params->kind == PEARL_AST_PARAMS) ? params->first_child : params;
            if (strcmp(fn, "print") == 0 || strcmp(fn, "justPrint") == 0) {
                /* handle formatted first-arg string: printf(format, ...)
                   otherwise build a format string for numeric args */
                int argc = 0;
                for (PearlAstNode *a = args_list; a; a = a->next) argc++;
                if (argc == 0) {
                    emit_indent(out, indent);
                    fprintf(out, "puts(\"\");\n");
                } else {
                    PearlAstNode *first = args_list;
                    if (first->kind == PEARL_AST_LITERAL && first->text && strchr(first->text, '%')) {
                        /* use as format */
                        emit_indent(out, indent);
                        fprintf(out, "printf(%s", first->text);
                        for (PearlAstNode *a = first->next; a; a = a->next) {
                            char *as = expr_to_c(a);
                            fprintf(out, ", %s", as);
                            free(as);
                        }
                        fprintf(out, ");\n");
                    } else if (argc == 1) {
                        /* single arg: string -> puts, number/expr -> printf("%d\n", expr) */
                        PearlAstNode *a = first;
                        if (a->kind == PEARL_AST_LITERAL && a->text && a->text[0] == '"') {
                            emit_indent(out, indent);
                            fprintf(out, "puts(%s);\n", a->text);
                        } else if (a->kind == PEARL_AST_LITERAL && a->text && is_numeric_literal(a->text)) {
                            emit_indent(out, indent);
                            fprintf(out, "printf(\"%%d\\n\", %s);\n", a->text);
                        } else {
                            char *as = expr_to_c(a);
                            emit_indent(out, indent);
                            fprintf(out, "printf(\"%%d\\n\", %s);\n", as);
                            free(as);
                        }
                    } else {
                        /* multiple args: build format "%d %d ...\n" and list expressions */
                        size_t fmt_size = argc * 4 + 16;
                        char *fmt = malloc(fmt_size);
                        fmt[0] = '\0';
                        for (int i = 0; i < argc; ++i) {
                            strcat(fmt, "%d");
                            if (i + 1 < argc) strcat(fmt, " ");
                        }
                        strcat(fmt, "\\n");
                        emit_indent(out, indent);
                        fprintf(out, "printf(\"%s\"", fmt);
                        free(fmt);
                        for (PearlAstNode *a = args_list; a; a = a->next) {
                            char *as = expr_to_c(a);
                            fprintf(out, ", %s", as);
                            free(as);
                        }
                        fprintf(out, ");\n");
                    }
                }
            } else {
                char *call = expr_to_c(s);
                emit_indent(out, indent);
                fprintf(out, "%s;\n", call);
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
        /* ensure main returns 0 when no explicit return present */
        if (strcmp(name, "main") == 0 && !has_return(body)) {
            fprintf(out, "    return 0;\n");
        }
        fprintf(out, "}\n\n");
    }
    return 0;
}
