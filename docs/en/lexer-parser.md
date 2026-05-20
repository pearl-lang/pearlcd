# Lexer and Parser

This page details the boundary between the lexer and parser.

## Lexer

The lexer converts raw source text into tokens. Supported token groups are:

- keywords
- identifiers
- numbers
- string literals
- operators
- delimiters

Important tokens include:

- `var`
- `fun`
- `if`
- `else`
- `while`
- `for`
- `return`
- `print`
- `justPrint`
- `true`
- `false`
- `null`

## Parser

The parser converts tokens into an AST. Its main responsibilities are:

- matching token order to language rules
- building functions, blocks, and expressions
- reporting syntax errors with line numbers

## Operator precedence

The current precedence order is roughly:

1. assignment
2. equality
3. comparison
4. addition/subtraction
5. multiplication/division
6. unary

## Notes

- Comments start with `//`.
- Statements end with a newline or `;`.
- The parser aims to stay simple and direct.

## Example

```prl
fun main() {
  var x = 5
  var y = 10
  justPrint(x + y)
}
```