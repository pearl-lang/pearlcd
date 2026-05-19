#ifndef PEARLCD_IR_H
#define PEARLCD_IR_H

#include <stdio.h>
#include "ast.h"

/* Lower AST to a tiny textual IR printed to `out`.
 * Returns 0 on success, non-zero on error.
 */
int pearl_lower_ast(PearlAstNode *ast, FILE *out);

#endif
