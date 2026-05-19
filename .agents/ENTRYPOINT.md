First, read project-structure.md and create a roadmap here with any clear name you want. Use that roadmap while working on the project, and keep it updated so progress and completed work can be tracked later. This file is important for future edits too. When the project needs more changes later, use the other files you create and also add the useful parts to project.md.

Steps:
- Read project.md, understand it, and create a roadmap based on the project structure.
- Read syntax.md.
- Read doc.md and docs.md.
- Read cicd.md.
After you understand everything that needs to be done for the project, start working in small steps, verify that each step works, and record your progress as you go.

Roadmap (short-term, KISS):

- 1. Stabilize parser & lexer: finish grammar cleanup, reduce bison conflicts (`src/frontend/parser.y`, `src/frontend/lexer.l`).
- 2. Finish codegen parity: make `pearl_codegen_c` produce nicely formatted C and support multi-arg `print` semantics (`src/backend/codegen_c.c`).
- 3. Add smoke tests and CI matrix (Linux + Windows MinGW) — already present in `.github/workflows/ci.yml`.
- 4. Add release packaging and checksums, and a `scripts/package_release.sh` helper — already present.
- 5. Fix remaining compiler warnings and platform portability issues (e.g. implicit declarations).
- 6. Add docs build (pandoc) and attach docs to releases if desired (`scripts/docs.sh`).

How to use this roadmap while working:

- Pick one numbered item above and open the corresponding file(s).
- Make a small, focused change and run the local build/test commands (see `README.md`).
- Update this file and the top-level `TODO` when a step is completed.
- When ready, create a PR — CI will run on push/PR and validate builds and smoke tests.

Current status (short):

- Parser, lexer, AST, IR lowering, and a simple C codegen are implemented.
- `--run` mode compiles generated C and executes it, used in CI smoke tests.
- CI matrix and Release workflows were added; packaging and SHA256 generation included.
- Remaining work: pretty-printing codegen output, richer `print` semantics, and fixing a few compiler warnings.

If you need me to continue, tell me which roadmap item to pick next and I'll update the todos and begin changes.