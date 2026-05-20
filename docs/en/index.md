# PearlCD Documentation

## Overview

PearlCD is a small and simple compiler prototype for the Pearl language. The goal is to keep the language readable, easy to test, and easy to evolve without unnecessary complexity. The project is C-based and uses CMake, Flex, and Bison for the build and front-end pipeline.

The core pipeline is:

1. The lexer splits source text into tokens.
2. The parser builds an AST (Abstract Syntax Tree).
3. The semantic layer performs name and scope checks.
4. The IR layer lowers the AST into a textual intermediate representation.
5. The C codegen layer emits C code from the AST.
6. The `--run` mode writes generated C to a temporary file, compiles it with the host compiler, and executes it.

The project follows a KISS approach: keep the feature set small, keep the behavior explicit, and avoid unnecessary abstractions.

## Repository Layout

Main directories:

- `src/`: primary source code
- `include/`: public headers
- `concept/`: architecture and language notes
- `docs/`: user-facing documentation sources
- `examples/`: sample Pearl programs
- `scripts/`: helper scripts for build, packaging, and docs
- `toolchain/`: platform-specific CMake toolchain files
- `tests/`: unit tests

## CLI

The CLI modes in `src/main.c` are:

- `--lex`: print tokens
- `--parse`: print the AST
- `--check`: run name/scope checks
- `--lower`: print the textual IR
- `--codegen`: emit C code
- `--run`: emit C, compile it, and run it

If no file is provided, input is read from `stdin`. A single file path can be passed, or `-` can be used for `stdin`.

Examples:

```bash
./build/pearlcd --parse examples/test.prl
./build/pearlcd --check examples/test.prl
./build/pearlcd --lower examples/test.prl
## Detailed Topics

- [Command Line](cli.md)
- [Syntax](syntax.md)
- [Lexer and Parser](lexer-parser.md)
- [AST](ast.md)
- [Semantic Checks](semantic.md)
- [IR](ir.md)
- [C Codegen](codegen.md)

## Build, Test, and Release

See [Build, Test, and Release](build-release.md) for the full workflow.
3. define AST nodes
4. add semantic checks
5. implement IR/C codegen support
6. add examples and tests

This keeps the codebase compact and easy to understand.
