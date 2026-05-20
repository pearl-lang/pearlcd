# IR

The IR layer lowers the AST into a textual intermediate representation.

## Purpose

- make debugging easier
- provide a pre-codegen view
- simplify testing of compiler stages

## Shape

This IR is not a heavy SSA design. Readability comes first.

## Typical use

```bash
./build/pearlcd --lower examples/test.prl
```

## What it shows

- function structure
- statement flow
- expression relationships
- calls and assignments

## Note

The IR output does not need to match the source code one-to-one. Its job is to make the compiler pipeline visible.