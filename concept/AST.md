## Abstract Syntax Tree

The AST is the structured form of the program after parsing.

### Why It Exists

- It removes raw token details.
- It shows the real structure of the program.
- It gives later stages a clean input.

### Basic Nodes

- Program
- Function
- Block
- Variable declaration
- Assignment
- If statement
- While loop
- For loop
- Return statement
- Call expression
- Binary expression
- Literal
- Identifier

### Simple Shape

```text
Program
└── Function main
	├── VarDecl x = 5
	└── Call justPrint(x)
```

### Design Goal

The AST should be small, explicit, and easy to walk in later compiler stages.
