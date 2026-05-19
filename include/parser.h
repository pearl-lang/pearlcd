#ifndef PEARLCD_PARSER_H
#define PEARLCD_PARSER_H

#include <stdio.h>

#include "ast.h"

int pearl_parse_file(FILE *input, const char *source_name, PearlAstNode **out_ast);

#endif
