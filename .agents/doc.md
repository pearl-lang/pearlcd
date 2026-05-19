The documentation must be in Markdown format. It should be detailed and clear, but not overly complex. The docs should include all information users and developers may need, without unnecessary detail. Each section should provide the basic information about its topic and include examples or images when useful. Write the docs in language that is easy for users and developers to understand, and explain technical terms when needed.

Doc build and formats:

- The repository provides `scripts/docs.sh` to render docs using `pandoc` (PDF and HTML). Keep the documentation source in `docs/` and the conceptual notes in `concept/`.
- Provide English and Turkish copies for key docs. The `docs.sh` script should accept a language flag and produce language-specific outputs.
- When releasing, consider attaching generated PDFs to GitHub Releases (optional).