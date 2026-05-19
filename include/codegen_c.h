#ifndef PEARLCD_CODEGEN_C_H
#define PEARLCD_CODEGEN_C_H

#include <stdio.h>
#include "ast.h"

/* Emit simple C code from AST to `out`. Returns 0 on success. */
int pearl_codegen_c(PearlAstNode *ast, FILE *out);

#endif
