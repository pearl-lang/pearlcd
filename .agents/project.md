Project technologies:

- Flex
- Bison
- clang
- CMake
- LLVM
- pandoc
- C11 standard

Pearl is designed as a simple compiler. Its main parts are:
- Lexer: Reads source code and splits it into tokens.
- Parser: Takes tokens and builds an abstract syntax tree (AST).
- AST: Stores the structural form of the source code.
- Platform: Lets the compiler work on different operating systems and architectures.
- Codegen: Takes the AST and produces machine code for the target platform.
- Runtime: Provides the library code and support code needed by compiled programs.
- Build: Manages and automates the build process.

Keep the project as simple as possible and follow the KISS principle. Avoid unnecessary details. Define the project structure and coding rules, then follow them. Write the project in a way that is easy for users and developers to understand, and explain technical terms when needed. Keep the project modular, with clear responsibilities for each module. Make the project testable and easy to maintain. Release it under an open-source license. Develop it in a way that fits CI/CD workflows. The documentation should be good and should include all information users and developers may need, but without unnecessary detail. Each section should give the basic information about its topic and use examples or images when helpful.

Testing should happen in small steps during development. Keep tests simple and verify that the project works. Write everything in clear language and explain technical terms when needed. The documentation should cover the needed information, stay focused, and give basic context for each section.

Current implementation status (short):

- Lexer (`src/frontend/lexer.l`) and parser (`src/frontend/parser.y`) are functional and produce `PearlAstNode` ASTs.
- AST utilities exist in `src/frontend/ast.c` / `include/ast.h`.
- Semantic checks implemented in `src/frontend/semantic.c` (symbol checks).
- IR lowering implemented in `src/backend/ir.c` and header `include/ir.h`.
- Simple C codegen implemented in `src/backend/codegen_c.c` and header `include/codegen_c.h`.
- CLI (`src/main.c`) includes `--lex`, `--parse`, `--check`, `--lower`, `--codegen`, `--run` modes.
- CI (`.github/workflows/ci.yml`) and Release workflows (`.github/workflows/release.yml`) added.

Remaining/next improvements:

- Pretty-print codegen output and extend `print` semantics (format strings and multi-arg support).
- Remove remaining implicit-declaration warnings and make builds clean under `-Wall -Wextra`.
- Expand examples and tests under `examples/` and add automated example runs to CI.

Keep changes minimal and consistent with the KISS philosophy.