First, read project-structure.md and create a roadmap here with any clear name you want. Use that roadmap while working on the project, and keep it updated so progress and completed work can be tracked later. This file is important for future edits too. When the project needs more changes later, use the other files you create and also add the useful parts to project.md.

Steps:
- Read project.md, understand it, and create a roadmap based on the project structure.
- Read syntax.md.
- Read doc.md and docs.md.
- Read cicd.md.

After you understand everything that needs to be done for the project, start working in small steps, verify that each step works, and record your progress as you go.

Current status:
- lexer mode works and prints tokens with line numbers
- command-line input works with files and stdin
- build generation currently uses CMake and Flex
- parser mode works and prints a simple AST
- current focus is cleaner grammar, symbols/scopes, and a small semantic layer
- next larger step is CI/CD and release builds