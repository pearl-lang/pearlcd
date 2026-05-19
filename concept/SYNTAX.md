## Syntax Overview

Pearl is designed to stay small and easy to read. The syntax should support only the basic features needed for a first compiler.

### Core Keywords

- `var` for variable declarations
- `fun` for function definitions
- `if` and `else` for branching
- `while` and `for` for loops
- `return` for returning a value from a function
- `print` and `justPrint` for output
- `true`, `false`, and `null` for basic literal values

### Current Language Pieces

The first implementation focuses on a very small set of syntax pieces:

- function declarations
- variable declarations
- function calls
- basic expressions
- string and number literals
- simple control flow with `if`, `else`, `while`, and `for`

This is enough for a starter compiler and keeps the language easy to grow later.

### Simple Rules

- Use curly braces for blocks.
- End statements with a clear line break or the project’s chosen statement rule.
- Keep expressions small and readable.
- Prefer plain function calls and simple arithmetic at the start.

### Basic Examples

```prl
fun main() {
	var x = 5
	var y = 10
	var z = x + y
	justPrint(z)
}
```

```prl
fun main() {
	var i = 0
	while (i < 5) {
		justPrint(i)
		i = i + 1
	}
}
```

### Design Goal

The syntax should be easy to parse and easy to explain. It should support a small, useful subset of programming ideas without becoming complex.
