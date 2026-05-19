## Lexer

The lexer reads raw source code and turns it into tokens.

### What the Lexer Should Do

- Recognize keywords such as `var`, `fun`, `if`, and `return`.
- Recognize identifiers, numbers, strings, and symbols.
- Skip whitespace and comments.
- Report invalid characters clearly.

### Token Groups

- Keywords
- Identifiers
- Literals
- Operators
- Delimiters

### Current Implementation

The first working version runs in `--lex` mode and prints tokens with line numbers. It already recognizes:

- keywords like `fun`, `var`, `if`, `else`, `while`, `for`, `return`, `print`, `justPrint`, `true`, `false`, and `null`
- identifiers and numbers
- string literals
- simple operators and punctuation
- line comments starting with `//`

This first version is intentionally small. It is enough to show that source code can be read and broken into tokens.

### Simple Scope

Keep token rules small. The lexer should only support the syntax that the current language needs. Do not add complex preprocessing or advanced token tricks.

### Example Flow

Source code:

```prl
var x = 5
```

Tokens:

- `var`
- `identifier(x)`
- `=`
- `number(5)`

### Design Goal

The lexer should be fast, predictable, and easy to maintain.
