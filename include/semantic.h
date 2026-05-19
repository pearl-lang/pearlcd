#ifndef PEARLCD_SEMANTIC_H
#define PEARLCD_SEMANTIC_H

#include <stdio.h>

#include "ast.h"

int pearl_check_ast(FILE *output, const char *source_name, const PearlAstNode *root);

#endif