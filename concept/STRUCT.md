## Project Structure

This document explains how the project is organized.

### Main Idea

The project should keep a clean and simple layout so each part has one clear role.

### Current Structure

- `concept`: design and language notes
- `docs`: user-facing and developer-facing documentation
- `examples`: sample Pearl programs
- `include`: public headers
- `scripts`: helper scripts
- `src`: implementation code
- `toolchain`: platform build files

### Design Goal

The structure should make it easy to find the lexer, parser, AST, backend, and build logic without spreading code everywhere.
