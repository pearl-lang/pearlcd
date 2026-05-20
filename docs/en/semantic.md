# Semantic Checks

The semantic layer verifies name and scope rules after parsing.

## Current checks

- duplicate names in the same scope
- unknown identifier usage
- invalid assignment targets
- unresolved call names

## Scope model

- Functions create their own scope.
- Blocks can create nested scopes.
- Lookup can walk outward through parent scopes.

## Things to know

- There is no type checking yet.
- Semantic checks cover meaning, not syntax.
- `print` and `justPrint` are treated as built-in calls.

## Example error

```prl
fun main() {
  justPrint(x)
}
```

This should raise an error if `x` is not defined.

## Design goal

The semantic layer should remain short, readable, and easy to extend.