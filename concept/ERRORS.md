## Errors

Pearl should report errors in a simple and helpful way.

### Main Error Types

- Lexical errors: invalid characters or broken tokens
- Parse errors: invalid syntax order or missing parts
- Semantic errors: duplicate names or unknown identifiers
- Build or backend errors: issues during code generation or linking

### What Good Errors Should Show

- The file or location if possible
- A short message
- The part of the source that caused the problem
- A clear hint when the fix is obvious

### Small Compiler Rule

Keep error handling readable. A first version of the compiler should prioritize clear messages over fancy diagnostics.

### Design Goal

Errors should help the user understand what went wrong and how to fix it without extra complexity.