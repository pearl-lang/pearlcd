# Build, Test, and Release

## Requirements

- CMake
- Flex
- Bison
- a C compiler
- `pandoc` for documentation generation

## Local build

The shortest path is:

```bash
cmake -S . -B build
cmake --build build
```

Helper script:

```bash
bash scripts/build.sh
bash scripts/build.sh mingw
bash scripts/build.sh linux out/build-linux
```

## Documentation generation

English and Turkish docs are generated with `scripts/docs.sh`.

```bash
bash scripts/docs.sh en
bash scripts/docs.sh tr
bash scripts/docs.sh all
```

Outputs land under `docs/dist/`.

## Release packaging

Release packaging helper:

```bash
bash scripts/package_release.sh dist
```

The script:

- creates a dedicated build directory
- builds the binary
- writes `.tar.gz` for Linux
- writes `.zip` for Windows
- generates SHA256 checksum files

## CI/CD

- `ci.yml`: Linux and Windows build + smoke tests on push/pull request
- `release.yml`: builds Linux and Windows artifacts when a release is created and uploads them to GitHub Release

## Ignored outputs

- `build/`
- `build-release*/`
- `dist/`
- `docs/dist/`

These outputs should stay out of version control.