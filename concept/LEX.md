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
