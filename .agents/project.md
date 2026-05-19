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