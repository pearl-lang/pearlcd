## Build

The build system should keep the project simple and repeatable.

### Main Tools

- CMake for project configuration
- Flex for the lexer
- Bison for the parser
- clang for compilation
- LLVM for code generation support
- pandoc for documentation output

### Build Goals

- Build the project with one clear command path
- Keep platform support easy to manage
- Make release and debug builds predictable
- Support simple checks during development

### Small-Compiler Rule

Do not make the build system more complex than the language itself. Add only what is needed for this compiler to work well.

### Design Goal

The build should be easy to understand, easy to repeat, and easy to automate later.
