%defines
/* parse.trace removed for cleaner runtime output */

%code requires {
    #include "ast.h"
}

%{
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex(void);
extern FILE *yyin;
extern int yylineno;
int yydebug = 0;

PearlAstNode *pearl_parser_root = NULL;
static const char *pearl_parser_source = NULL;

static PearlAstNode *make_node(PearlAstKind kind, const char *text) {
    return pearl_ast_create(kind, text);
}

static PearlAstNode *make_binary(const char *op, PearlAstNode *left, PearlAstNode *right) {
    PearlAstNode *node = make_node(PEARL_AST_BINARY, op);
    pearl_ast_append_child(node, left);
    pearl_ast_append_child(node, right);
    return node;
}

static PearlAstNode *make_unary(const char *op, PearlAstNode *expr) {
    PearlAstNode *node = make_node(PEARL_AST_UNARY, op);
    pearl_ast_append_child(node, expr);
    return node;
}

static PearlAstNode *make_call_node(const char *name, PearlAstNode *args) {
    PearlAstNode *node = make_node(PEARL_AST_CALL, name);
    pearl_ast_append_child(node, args);
    return node;
}

static PearlAstNode *wrap_literal(const char *text) {
    return make_node(PEARL_AST_LITERAL, text);
}

static PearlAstNode *wrap_identifier(const char *text) {
    return make_node(PEARL_AST_IDENTIFIER, text);
}

static void append_list(PearlAstNode *list, PearlAstNode *item) {
    pearl_ast_append_child(list, item);
}

static PearlAstNode *combine_binary(PearlAstNode *left, const char *op, PearlAstNode *right) {
    return make_binary(op, left, right);
}

int yyerror(const char *message) {
    if (pearl_parser_source != NULL) {
        fprintf(stderr, "%s:%d: %s\n", pearl_parser_source, yylineno, message);
    } else {
        fprintf(stderr, "parse error: %s\n", message);
    }

    return 0;
}
%}

%union {
    char *text;
    PearlAstNode *node;
}

/* Operator precedence to resolve expression ambiguities */
%right TOK_ASSIGN
%left TOK_EQ TOK_NE
%left TOK_LT TOK_LE TOK_GT TOK_GE
%left TOK_PLUS TOK_MINUS
%left TOK_STAR TOK_SLASH
%right TOK_BANG

%token <text> TOK_IDENT TOK_STRING TOK_NUMBER TOK_TRUE TOK_FALSE TOK_NULL
%token TOK_VAR TOK_FUN TOK_IF TOK_ELSE TOK_WHILE TOK_FOR TOK_RETURN TOK_PRINT TOK_JUSTPRINT
%token TOK_EQ TOK_NE TOK_LE TOK_GE TOK_ASSIGN TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_LT TOK_GT TOK_BANG
%token TOK_LPAREN TOK_RPAREN TOK_LBRACE TOK_RBRACE TOK_COMMA TOK_SEP TOK_UNKNOWN

%type <node> program atom function_list function_def params_opt params block statements sep_opt statements_opt statement var_decl assignment call_stmt return_stmt if_stmt while_stmt for_stmt expr arg_list_opt arg_list primary unary factor term comparison equality

%start program

%%

program
    : sep_opt function_list sep_opt
      {
          PearlAstNode *root = make_node(PEARL_AST_PROGRAM, NULL);
          pearl_ast_append_child(root, $2);
          pearl_parser_root = root;
      }
    ;

/* optional separator list used inside blocks/statements */
sep_opt
    :
      {
          $$ = NULL;
      }
    | TOK_SEP sep_opt
      {
          $$ = NULL;
      }
    ;

function_list
    : function_def
      {
          $$ = make_node(PEARL_AST_BLOCK, NULL);
          append_list($$, $1);
      }
    | function_list TOK_SEP function_def
      {
          $$ = $1;
          append_list($$, $3);
      }
    ;

function_def
    : TOK_FUN TOK_IDENT TOK_LPAREN params_opt TOK_RPAREN block
      {
          PearlAstNode *function_node = make_node(PEARL_AST_FUNCTION, $2);
          pearl_ast_append_child(function_node, $4);
          pearl_ast_append_child(function_node, $6);
          $$ = function_node;
      }
    ;

params_opt
    :
      {
          $$ = make_node(PEARL_AST_PARAMS, NULL);
      }
    | params
      {
          $$ = $1;
      }
    ;

params
    : TOK_IDENT
      {
          $$ = make_node(PEARL_AST_PARAMS, NULL);
          append_list($$, wrap_identifier($1));
      }
    | params TOK_COMMA TOK_IDENT
      {
          $$ = $1;
          append_list($$, wrap_identifier($3));
      }
    ;

block
  : TOK_LBRACE sep_opt statements_opt TOK_RBRACE
    {
      PearlAstNode *block_node = make_node(PEARL_AST_BLOCK, NULL);
      if ($3 != NULL) {
        pearl_ast_append_child(block_node, $3);
      }
      $$ = block_node;
    }
  ;

statements_opt
  :
    {
      $$ = NULL;
    }
  | statements
    {
      $$ = $1;
    }
    ;

statements
    : statement
      {
          $$ = make_node(PEARL_AST_BLOCK, NULL);
          append_list($$, $1);
      }
    | statements TOK_SEP statement
      {
          $$ = $1;
          append_list($$, $3);
      }
    | statements TOK_SEP
      {
          $$ = $1;
      }
    ;

statement
    : var_decl
    | assignment
    | call_stmt
    | return_stmt
    | if_stmt
    | while_stmt
    | for_stmt
    | block
    ;

var_decl
    : TOK_VAR TOK_IDENT TOK_ASSIGN expr
      {
          PearlAstNode *node = make_node(PEARL_AST_VAR_DECL, $2);
          pearl_ast_append_child(node, $4);
          $$ = node;
      }
    ;

assignment
    : TOK_IDENT TOK_ASSIGN expr
      {
          PearlAstNode *node = make_node(PEARL_AST_ASSIGNMENT, $1);
          pearl_ast_append_child(node, $3);
          $$ = node;
      }
    ;

call_stmt
    : TOK_PRINT TOK_LPAREN arg_list_opt TOK_RPAREN
      {
          PearlAstNode *node = make_call_node("print", $3);
          $$ = node;
      }
    | TOK_JUSTPRINT TOK_LPAREN arg_list_opt TOK_RPAREN
      {
          PearlAstNode *node = make_call_node("justPrint", $3);
          $$ = node;
      }
    | TOK_IDENT TOK_LPAREN arg_list_opt TOK_RPAREN
      {
          PearlAstNode *node = make_call_node($1, $3);
          $$ = node;
      }
    ;

arg_list_opt
    :
      {
          $$ = make_node(PEARL_AST_PARAMS, NULL);
      }
    | arg_list
      {
          $$ = $1;
      }
    ;

arg_list
    : expr
      {
          $$ = make_node(PEARL_AST_PARAMS, NULL);
          append_list($$, $1);
      }
    | arg_list TOK_COMMA expr
      {
          $$ = $1;
          append_list($$, $3);
      }
    ;

return_stmt
    : TOK_RETURN expr
      {
          PearlAstNode *node = make_node(PEARL_AST_RETURN, NULL);
          pearl_ast_append_child(node, $2);
          $$ = node;
      }
    | TOK_RETURN
      {
          PearlAstNode *node = make_node(PEARL_AST_RETURN, NULL);
          $$ = node;
      }
    ;

if_stmt
    : TOK_IF TOK_LPAREN expr TOK_RPAREN block TOK_ELSE block
      {
          PearlAstNode *node = make_node(PEARL_AST_IF, NULL);
          pearl_ast_append_child(node, $3);
          pearl_ast_append_child(node, $5);
          pearl_ast_append_child(node, $7);
          $$ = node;
      }
    | TOK_IF TOK_LPAREN expr TOK_RPAREN block
      {
          PearlAstNode *node = make_node(PEARL_AST_IF, NULL);
          pearl_ast_append_child(node, $3);
          pearl_ast_append_child(node, $5);
          $$ = node;
      }
    ;

while_stmt
    : TOK_WHILE TOK_LPAREN expr TOK_RPAREN block
      {
          PearlAstNode *node = make_node(PEARL_AST_WHILE, NULL);
          pearl_ast_append_child(node, $3);
          pearl_ast_append_child(node, $5);
          $$ = node;
      }
    ;

for_stmt
    : TOK_FOR TOK_LPAREN var_decl TOK_SEP expr TOK_SEP assignment TOK_RPAREN block
      {
          PearlAstNode *node = make_node(PEARL_AST_FOR, NULL);
          pearl_ast_append_child(node, $3);
          pearl_ast_append_child(node, $5);
          pearl_ast_append_child(node, $7);
          pearl_ast_append_child(node, $9);
          $$ = node;
      }
    ;

expr
    : equality
    ;

equality
    : comparison
      {
          $$ = $1;
      }
    | equality TOK_EQ comparison
      {
          $$ = combine_binary($1, "==", $3);
      }
    | equality TOK_NE comparison
      {
          $$ = combine_binary($1, "!=", $3);
      }
    ;

comparison
    : term
      {
          $$ = $1;
      }
    | comparison TOK_LT term
      {
          $$ = combine_binary($1, "<", $3);
      }
    | comparison TOK_GT term
      {
          $$ = combine_binary($1, ">", $3);
      }
    | comparison TOK_LE term
      {
          $$ = combine_binary($1, "<=", $3);
      }
    | comparison TOK_GE term
      {
          $$ = combine_binary($1, ">=", $3);
      }
    ;

term
    : factor
      {
          $$ = $1;
      }
    | term TOK_PLUS factor
      {
          $$ = combine_binary($1, "+", $3);
      }
    | term TOK_MINUS factor
      {
          $$ = combine_binary($1, "-", $3);
      }
    ;

factor
    : unary
      {
          $$ = $1;
      }
    | factor TOK_STAR unary
      {
          $$ = combine_binary($1, "*", $3);
      }
    | factor TOK_SLASH unary
      {
          $$ = combine_binary($1, "/", $3);
      }
    ;

unary
    : TOK_MINUS unary
      {
          $$ = make_unary("-", $2);
      }
    | TOK_BANG unary
      {
          $$ = make_unary("!", $2);
      }
    | primary
      {
          $$ = $1;
      }
    ;

atom
    : TOK_NUMBER
      {
          $$ = wrap_literal($1);
      }
    | TOK_STRING
      {
          $$ = wrap_literal($1);
      }
    | TOK_TRUE
      {
          $$ = wrap_literal("true");
      }
    | TOK_FALSE
      {
          $$ = wrap_literal("false");
      }
    | TOK_NULL
      {
          $$ = wrap_literal("null");
      }
    | TOK_IDENT
      {
          $$ = wrap_identifier($1);
      }
    | TOK_LPAREN expr TOK_RPAREN
      {
          $$ = $2;
      }
    ;

primary
    : atom
    | atom TOK_LPAREN arg_list_opt TOK_RPAREN
      {
          /* If atom is an identifier, form a normal call; otherwise create a
             call node with a placeholder callee and keep the atom as first arg. */
          PearlAstNode *callee = $1;
          if (callee->kind == PEARL_AST_IDENTIFIER) {
              PearlAstNode *name = callee;
              PearlAstNode *node = make_call_node(name->text, $3);
              pearl_ast_free(name);
              $$ = node;
          } else {
              PearlAstNode *node = make_call_node("<expr>", $3);
              pearl_ast_append_child(node, callee);
              $$ = node;
          }
      }
    ;

%%

int pearl_parse_file(FILE *input, const char *source_name, PearlAstNode **out_ast) {
    int result = 0;

    pearl_parser_source = source_name;
    pearl_parser_root = NULL;
    yyin = input;

  yydebug = 0;

    result = yyparse();

    if (result != 0) {
        return result;
    }

    if (out_ast != NULL) {
        *out_ast = pearl_parser_root;
    }

    return 0;
}