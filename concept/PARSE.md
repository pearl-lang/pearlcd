## Parser

The parser takes tokens from the lexer and builds the abstract syntax tree.

### Main Job

- Check that the token order matches the language rules.
- Build a clear tree structure for programs, functions, statements, and expressions.
- Report syntax errors with simple and helpful messages.

### What It Should Understand

- Variable declarations
- Function definitions and calls
- If and else blocks
- While loops
- For loops
- Return statements
- Basic expressions like math and comparisons

### Simple Parsing Style

The parser should follow the current language shape closely. It should not try to be clever. Each rule should map to one clear syntax form.

### Example

Input:

```prl
fun main() {
		var x = 5
		justPrint(x)
}
```

Possible AST idea:

- Program
	- Function `main`
		- Variable declaration `x = 5`
		- Call `justPrint(x)`

### Design Goal

The parser should be simple enough that it is easy to debug and extend later.
