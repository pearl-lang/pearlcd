# AST

The AST is the tree-shaped representation of a parsed program.

## Why it exists

- It removes token-level details.
- It exposes the structure of the program.
- It gives later compiler stages a clean input.

## Node kinds

The main kinds in `include/ast.h` are:

- `PROGRAM`
- `FUNCTION`
- `PARAMS`
- `BLOCK`
- `VAR_DECL`
- `ASSIGNMENT`
- `CALL`
- `RETURN`
- `IF`
- `WHILE`
- `FOR`
- `BINARY`
- `UNARY`
- `IDENTIFIER`
- `LITERAL`

## Simple shape

```text
PROGRAM
|-- FUNCTION main
    |-- PARAMS
    `-- BLOCK
        |-- VAR_DECL x = 5
        `-- CALL justPrint(x)
```

## Stored information

AST nodes usually carry:

- node kind
- optional text value
- child nodes

## Design goal

The AST should stay small, explicit, and easy to traverse.