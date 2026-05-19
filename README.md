# pearlcd

`pearlcd` is a small compiler project for the Pearl language.

Current state:

- the project builds with CMake and Flex
- the project also builds with Bison for the parser
- the CLI accepts input files and a `--lex` mode
- the lexer prints tokens with line numbers
- the parser works in `--parse` mode and prints a simple AST
- the concept docs describe the current KISS-first direction

Quick start:

```bash
cmake -S . -B build
cmake --build build
./build/pearlcd --lex examples/hello.prl
./build/pearlcd --parse examples/test.prl
```

The current focus is cleaner grammar rules, a slightly richer AST shape, and a small semantic layer for names and scopes. After that, CI/CD and release builds will be added.

Repository note:

- `build/` is ignored so generated files do not pollute the working tree.
