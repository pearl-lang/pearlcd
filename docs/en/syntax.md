# Syntax

Pearl syntax is intentionally small and direct. The parser treats a program as a top-level list of function definitions.

## Core rules

- A program consists of one or more function definitions.
- Functions use `fun name(param1, param2) { ... }`.
- Blocks use curly braces.
- Statements are separated by newlines or `;`.
- Comments start with `//`.

## Keywords

- `var`
- `fun`
- `if` / `else`
- `while`
- `for`
- `return`
- `print`
- `justPrint`
- `true`, `false`, `null`

## Statement types

### Variable declaration

```prl
var x = 5
```

### Assignment

```prl
x = x + 1
```

### Call

```prl
print(x)
justPrint(x, y)
foo(bar)
```

### Return

```prl
return x
return
```

### Conditional

```prl
if (x > 0) {
  print(x)
} else {
  print(0)
}
```

### Loops

```prl
while (i < 10) {
  i = i + 1
}

for (var i = 0; i < 10; i = i + 1) {
  justPrint(i)
}
```

## Expressions

Expressions are parsed with this precedence order:

- assignment
- equality `==`, `!=`
- comparison `<`, `<=`, `>`, `>=`
- addition/subtraction `+`, `-`
- multiplication/division `*`, `/`
- unary `!`

Example:

```prl
var total = x + y * 2
```

## Literals

- number: `42`
- string: `"hello"`
- boolean: `true`, `false`
- null: `null`

## Current parser note

The current parser expects parenthesized conditions and loop expressions, such as `if (expr) { ... }` and `while (expr) { ... }`.

## Short example

```prl
fun main() {
  var x = 5
  var y = 10
  var total = x + y
  if (total > 10) {
    justPrint(total)
  } else {
    print(total)
  }
}
```