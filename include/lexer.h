#ifndef PEARLCD_LEXER_H
#define PEARLCD_LEXER_H

#include <stdio.h>

void pearl_lexer_set_trace(int enabled);
int pearl_run_lexer(FILE *input, const char *source_name);

#endif
