## Symbols and Scope

Pearl needs a small symbol system to keep track of names in the program.

### What a Symbol Is

A symbol is a named item in the program, such as:

- a variable
- a function
- a parameter

### What the Symbol System Should Do

- Store declared names
- Track which scope a name belongs to
- Detect duplicate names in the same scope
- Help the parser or later passes resolve identifiers

### Simple Scope Model

Use a small scope stack:

- global scope for top-level names
- function scope for function-local names
- block scope if needed for nested blocks

### Why It Matters

The compiler needs a clear way to know what an identifier means before code generation starts.

### Design Goal

Keep symbol handling simple and explicit. Do not add advanced features until the basic language works.