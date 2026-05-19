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

### Current Implementation

The first semantic pass uses a small symbol table with nested scopes. It already checks:

- duplicate function names
- duplicate variable names in the same scope
- duplicate parameter names
- unknown identifiers in expressions and assignments

This is enough for a first KISS-friendly compiler pass.

Function names are collected first, then function bodies are checked. This keeps calls to other functions consistent with the global scope.

### Design Goal

Keep symbol handling simple and explicit. Do not add advanced features until the basic language works.