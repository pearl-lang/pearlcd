%option noyywrap nodefault yylineno never-interactive

%{
#include "parser.tab.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int pearl_trace_lex = 0;

static char *copy_text(void) {
	size_t length = strlen(yytext) + 1;
	char *copy = malloc(length);

	if (copy == NULL) {
		return NULL;
	}

	memcpy(copy, yytext, length);
	return copy;
}

void pearl_lexer_set_trace(int enabled) {
	pearl_trace_lex = enabled;
}

static int emit_token(int token, const char *kind) {
	if (pearl_trace_lex) {
		printf("%d %-12s %s\n", yylineno, kind, yytext);
	}

	return token;
}

static int emit_ident(void) {
	yylval.text = copy_text();
	return emit_token(TOK_IDENT, "IDENT");
}

static int emit_text_token(int token, const char *kind) {
	yylval.text = copy_text();
	return emit_token(token, kind);
}
%}

%%

[ \t\r]+                         ;
\n                               { return emit_token(TOK_SEP, "SEP"); }
"//"[^\n]*                      ;
"var"                           { return emit_token(TOK_VAR, "KW_VAR"); }
"fun"                           { return emit_token(TOK_FUN, "KW_FUN"); }
"if"                            { return emit_token(TOK_IF, "KW_IF"); }
"else"                          { return emit_token(TOK_ELSE, "KW_ELSE"); }
"while"                         { return emit_token(TOK_WHILE, "KW_WHILE"); }
"for"                           { return emit_token(TOK_FOR, "KW_FOR"); }
"return"                        { return emit_token(TOK_RETURN, "KW_RETURN"); }
"justPrint"                     { return emit_token(TOK_JUSTPRINT, "KW_JUSTPRINT"); }
"print"                         { return emit_token(TOK_PRINT, "KW_PRINT"); }
"true"                          { return emit_text_token(TOK_TRUE, "KW_TRUE"); }
"false"                         { return emit_text_token(TOK_FALSE, "KW_FALSE"); }
"null"                          { return emit_text_token(TOK_NULL, "KW_NULL"); }
"=="                            { return emit_token(TOK_EQ, "OP_EQ"); }
"!="                            { return emit_token(TOK_NE, "OP_NE"); }
"<="                            { return emit_token(TOK_LE, "OP_LE"); }
">="                            { return emit_token(TOK_GE, "OP_GE"); }
"="                             { return emit_token(TOK_ASSIGN, "OP_ASSIGN"); }
"+"                             { return emit_token(TOK_PLUS, "OP_ADD"); }
"-"                             { return emit_token(TOK_MINUS, "OP_SUB"); }
"*"                             { return emit_token(TOK_STAR, "OP_MUL"); }
"/"                             { return emit_token(TOK_SLASH, "OP_DIV"); }
"<"                             { return emit_token(TOK_LT, "OP_LT"); }
">"                             { return emit_token(TOK_GT, "OP_GT"); }
"!"                             { return emit_token(TOK_BANG, "OP_NOT"); }
"("                             { return emit_token(TOK_LPAREN, "LPAREN"); }
")"                             { return emit_token(TOK_RPAREN, "RPAREN"); }
"{"                             { return emit_token(TOK_LBRACE, "LBRACE"); }
"}"                             { return emit_token(TOK_RBRACE, "RBRACE"); }
","                             { return emit_token(TOK_COMMA, "COMMA"); }
";"                             { return emit_token(TOK_SEP, "SEP"); }
"\""([^\"\\\n]|\\.)*"\"" { return emit_text_token(TOK_STRING, "STRING"); }
[0-9]+                          { return emit_text_token(TOK_NUMBER, "NUMBER"); }
[A-Za-z_][A-Za-z0-9_]*          { return emit_ident(); }
.                               { return emit_token(TOK_UNKNOWN, "UNKNOWN"); }

%%
