## Intermediate Representation (IR)

This document describes the small, KISS textual Intermediate Representation used
by the project's `--lower` pipeline stage. The IR is deliberately simple and
human-readable so it is easy to inspect while we expand lowering and codegen.

Format

- File begins with `;; program` to identify the module.
- Functions are delimited with `func <name>` and `endfunc`.
- Instructions are one-per-line, textual, and easy to parse.

Minimal instruction set (current)

- `print <expr>` — print the evaluated expression or literal.
- `ret <expr>` — return from function (placeholder; may be extended).
- `assign <name> <expr>` — assign expression to a local (placeholder form).
- `call <name> [args...]` — call a function (arguments printed inline).

Example

```
;; program
func main
	print "Hello from pearl language!"
endfunc
```

Design notes & limitations

- This IR is a debugging/prototyping format; it is not SSA or register-based.
- Expressions are currently emitted as inline placeholders like `const 42` or
	`<expr>`; they are not yet lowered to temporaries.
- The IR covers a subset of constructs (functions, calls, prints, returns).

Next steps

- Expand expression lowering into temporaries (or SSA-like temps).
- Add arithmetic and comparison ops and explicit temporaries.
- Add control-flow instructions (`jmp`, `br`, `label`) to represent if/while.
- Stabilize the textual format or move to a structured on-disk format (JSON)
	if needed before implementing codegen.

Implementation note

- Current lowering lives in `src/backend/ir.c` and is invoked by the
	`--lower` CLI mode. Use that mode to inspect output while we expand lowering
	coverage and iterate on the IR design.

