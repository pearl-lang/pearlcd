#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ir.h"
#include "codegen_c.h"

int main(void) {
    FILE *f = fopen("examples/test.prl", "r");
    if (!f) {
        fprintf(stderr, "cannot open examples/test.prl\n");
        return 2;
    }
    PearlAstNode *ast = NULL;
    if (pearl_parse_file(f, "examples/test.prl", &ast) != 0) {
        fprintf(stderr, "parse failed\n");
        fclose(f);
        return 3;
    }
    fclose(f);

    /* Lower to IR into /dev/null (just ensure it succeeds) */
    FILE *devnull = fopen("/dev/null", "w");
    if (!devnull) devnull = stdout;
    if (pearl_lower_ast(ast, devnull) != 0) {
        fprintf(stderr, "lower failed\n");
        pearl_ast_free(ast);
        return 4;
    }

    /* Codegen to a temporary file */
    FILE *out = fopen("/tmp/pearl_test_codegen.c", "w");
    if (!out) {
        fprintf(stderr, "cannot open temp file for codegen\n");
        pearl_ast_free(ast);
        return 5;
    }
    if (pearl_codegen_c(ast, out) != 0) {
        fprintf(stderr, "codegen failed\n");
        fclose(out);
        pearl_ast_free(ast);
        return 6;
    }
    fclose(out);
    pearl_ast_free(ast);

    return 0;
}
