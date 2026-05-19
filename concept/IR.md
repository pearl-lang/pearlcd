## Intermediate Representation

IR is a lower-level form of the program used between the AST and code generation.

### Purpose

- Make code generation easier.
- Remove language-specific details.
- Represent control flow in a more direct way.

### Small Compiler View

For this project, the IR should stay simple. It does not need to be a large or complicated system.

### Possible IR Ideas

- Load a value
- Store a value
- Add or subtract values
- Compare values
- Jump to a label
- Call a function
- Return a value

### Example

Source idea:

```prl
var z = x + y
```

IR idea:

- load `x`
- load `y`
- add
- store `z`

### Design Goal

The IR should be simple enough to generate and simple enough to inspect while debugging.
