# Architecture

PearlCD follows a small compiler pipeline.

## Main flow

1. **Lexer** splits source text into tokens.
2. **Parser** builds an AST.
3. **Semantic** checks names and scopes.
4. **IR** lowers the AST into a textual intermediate form.
5. **C codegen** emits C code from the AST.
6. **Run mode** executes the generated C using the host compiler.

## AST

AST node kinds live in `include/ast.h`. The main nodes are:

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

The AST removes token-level details and exposes the structure of the program.

## Semantic layer

Semantic checks currently cover:

- duplicate names in the same scope
- unknown identifier usage
- invalid assignment targets
- unresolved calls

There is no type system yet. The focus is on name resolution and scope handling.

## IR

The IR layer emits a readable textual intermediate form. It is not a heavy SSA representation; it is meant to be easy to debug.

## C codegen

The C backend emits plain C. `print` and `justPrint` are handled as special cases. If `main` has no explicit return, `return 0` is added.

## Platform behavior

- Linux/macOS can use standard POSIX tools.
- Windows uses MSYS2 + MinGW.
- `--run` creates temporary files and depends on the host compiler.

## Design goal

Each layer should stay small, testable, and single-purpose.