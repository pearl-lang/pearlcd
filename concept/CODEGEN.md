## Code Generation

Code generation converts the AST or IR into machine code or a lower target form.

### Main Job

- Turn expressions into target instructions.
- Handle function calls and returns.
- Create control flow for if, while, and for.
- Use the platform rules for the final output.

### Simple Strategy

Keep code generation direct. Each language feature should map to a small and clear code pattern.

### Example Mapping

- `var x = 5` -> place a value in storage
- `x + y` -> load both values, add them, store or return the result
- `if` -> compare values and jump to the right block
- `while` -> create a loop with a start label and an end label

### Target Choice

The project should stay focused on one clear backend path first, then expand only if needed.

### Design Goal

Code generation should be predictable and easy to test with small examples.
