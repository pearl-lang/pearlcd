# Command Line

`pearlcd` uses a single binary with multiple modes. All modes are selected in `src/main.c`.

## General behavior

- If no input file is given, the program reads from `stdin`.
- A single file path can be passed.
- The `-` argument also means `stdin`.
- Most modes print to the terminal; `--run` uses temporary files.

## Modes

### `--lex`

Tokenizes the source and prints the lexer output.

```bash
./build/pearlcd --lex examples/hello.prl
```

### `--parse`

Turns the token stream into an AST and prints it.

```bash
./build/pearlcd --parse examples/test.prl
```

### `--check`

Runs name and scope checks only. There is no type system yet.

```bash
./build/pearlcd --check examples/test.prl
```

### `--lower`

Lowers the AST into a textual IR.

```bash
./build/pearlcd --lower examples/test.prl
```

### `--codegen`

Generates C code and writes it to standard output.

```bash
./build/pearlcd --codegen examples/test.prl > out.c
```

### `--run`

Generates C, compiles it with the host compiler, and runs it. This mode requires a C compiler in the local environment.

```bash
./build/pearlcd --run examples/test.prl
```

## Error handling

- File open failures use `perror`.
- Invalid options print usage help.
- Parse errors include line numbers.

## Practical note

`--run` is a fast validation path, not a standalone VM. It depends on the system compiler.