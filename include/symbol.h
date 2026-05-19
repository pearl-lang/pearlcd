#ifndef PEARLCD_SYMBOL_H
#define PEARLCD_SYMBOL_H

#include <stddef.h>

typedef enum PearlSymbolKind {
	PEARL_SYMBOL_VARIABLE,
	PEARL_SYMBOL_FUNCTION,
	PEARL_SYMBOL_PARAMETER,
} PearlSymbolKind;

typedef struct PearlSymbol {
	PearlSymbolKind kind;
	const char *name;
} PearlSymbol;

typedef struct PearlSymbolTable PearlSymbolTable;

PearlSymbolTable *pearl_symbol_table_create(void);
void pearl_symbol_table_free(PearlSymbolTable *table);
int pearl_symbol_table_push_scope(PearlSymbolTable *table);
int pearl_symbol_table_pop_scope(PearlSymbolTable *table);
int pearl_symbol_table_declare(PearlSymbolTable *table, PearlSymbolKind kind, const char *name);
int pearl_symbol_table_resolve(PearlSymbolTable *table, const char *name);

#endif
