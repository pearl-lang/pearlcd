Documentation should follow the format and content described in `doc.md`. Each section should provide the basic information about its topic and include examples or images when useful. Write the docs in language that users and developers can easily understand, and explain technical terms when needed. The overall structure should include both English and Turkish versions, and `scripts/docs.sh` should use `pandoc` to build PDF/HTML files for both languages. The docs must be in Markdown format. They should be detailed and clear, but not too complex. Include all information users and developers may need, while avoiding unnecessary detail. Each section should give the essential information for its topic and be supported by examples or images when needed.

Key doc locations:

- Concept notes: `concept/*.md` (AST.md, IR.md, CODEGEN.md, SYNTAX.md, etc.)
- User docs: `docs/` — rendered outputs live here or in CI artifacts.
- README: top-level `README.md` contains quick start and build helpers.

SYNTAX.md should describe the overall syntax structure and rules. It should explain how the syntax is written, which keywords are used, how variables are declared, and how functions are written. The other concept files should contain more detailed information about how the syntax is processed (parsing, AST shape, IR lowering, codegen strategies).