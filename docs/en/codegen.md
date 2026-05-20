# C Codegen

The C backend emits C code directly from the AST.

## Core behavior

- variables are emitted as C `int`
- assignments become direct C assignments
- function calls become C calls
- `print` and `justPrint` are handled specially

## Output style

The generated code is intentionally simple and readable. The goal is to produce a useful intermediate form, not highly optimized output.

## `print` behavior

- single string argument uses `puts`
- numeric expressions use `printf`
- multiple arguments use a generated format string

## `--run` link

`--run` first generates C, then writes it to a temporary file, compiles it, and executes it. That means a host compiler is required.

## Design goal

The codegen layer should stay small, direct, and portable.