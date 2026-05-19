#ifndef PEARLCD_AST_H
#define PEARLCD_AST_H

typedef enum PearlAstKind {
	PEARL_AST_PROGRAM,
	PEARL_AST_FUNCTION,
	PEARL_AST_PARAMS,
	PEARL_AST_BLOCK,
	PEARL_AST_VAR_DECL,
	PEARL_AST_ASSIGNMENT,
	PEARL_AST_CALL,
	PEARL_AST_RETURN,
	PEARL_AST_IF,
	PEARL_AST_WHILE,
	PEARL_AST_FOR,
	PEARL_AST_BINARY,
	PEARL_AST_UNARY,
	PEARL_AST_IDENTIFIER,
	PEARL_AST_LITERAL,
} PearlAstKind;

typedef struct PearlAstNode {
	PearlAstKind kind;
	char *text;
	struct PearlAstNode *first_child;
	struct PearlAstNode *last_child;
	struct PearlAstNode *next;
} PearlAstNode;

PearlAstNode *pearl_ast_create(PearlAstKind kind, const char *text);
void pearl_ast_append_child(PearlAstNode *parent, PearlAstNode *child);
void pearl_ast_print(const PearlAstNode *node, int indent);
void pearl_ast_free(PearlAstNode *node);

#endif
