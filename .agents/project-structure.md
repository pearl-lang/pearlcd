The project structure is shown below. The repository root contains build, concept, docs, examples, include, LICENSE, README.md, scripts, src, and toolchain. Keep this structure mostly the same, but small additions are allowed when needed.

- build: Stores build files.
- CMakeLists.txt: The project's CMake configuration file.
- concept: Markdown files that explain the project's core ideas and design.
- docs: The project's documentation files.
- examples: Example source files for the project.
- include: Public header files for the project.
- LICENSE: The project's license file.
- README.md: The main project introduction in Markdown.
- scripts: Helper scripts for the project.
- src: The project's source code.
- toolchain: Build configuration files for different platforms.

.
├── build
├── CMakeLists.txt
├── concept
│   ├── AST.md
│   ├── BUILD.md
│   ├── CODEGEN.md
│   ├── ERRORS.md
│   ├── IR.md
│   ├── LEX.md
│   ├── PARSE.md
│   ├── PLATFORM.md
│   ├── RUNTIME.md
│   ├── STRUCT.md
│   ├── SYMBOLS.md
│   └── SYNTAX.md
├── docs
├── examples
│   ├── hello.prl
│   └── test.prl
├── include
│   ├── ast.h
│   ├── codegen_llvm.h
│   ├── lexer.h
│   ├── parser.h
│   └── symbol.h
├── LICENSE
├── README.md
├── scripts
│   ├── check-dep.sh
│   └── docs.sh
├── src
│   ├── backend
│   │   ├── codegen_llvm.c
│   │   └── codegen_llvm.h
│   ├── frontend
│   │   ├── ast.c
│   │   ├── ast.h
│   │   ├── lexer.l
│   │   └── parser.y
│   ├── main.c
│   └── util
│       └── debug.c
└── toolchain
    ├── linux.cmake
    ├── windows-mingw.cmake
    └── windows-msvc.cmake
