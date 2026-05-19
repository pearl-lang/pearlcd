#ifndef PEARLCD_SYMBOL_H
#define PEARLCD_SYMBOL_H

typedef enum PearlSymbolKind {
	PEARL_SYMBOL_VARIABLE,
	PEARL_SYMBOL_FUNCTION,
	PEARL_SYMBOL_PARAMETER,
} PearlSymbolKind;

typedef struct PearlSymbol {
	PearlSymbolKind kind;
	const char *name;
} PearlSymbol;

#endif
