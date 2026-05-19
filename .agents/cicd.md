After the project is finished and confirmed to work locally, set up CI/CD so release builds are produced for both Windows and Linux. GitHub Actions is used by this repository; there are two workflows:

- `.github/workflows/ci.yml` — runs on `push`/`pull_request` on `main` and builds a matrix for `ubuntu-latest` and `windows-latest` (MinGW). It installs required packages, configures CMake, builds, and runs smoke tests (`--parse`, `--lower`, `--run`).
- `.github/workflows/release.yml` — runs when a Release is published; it builds Release artifacts for Linux and Windows, packages them (tar/zip), computes SHA256 checksums, and uploads artifacts to the Release.

Local testing hints:

- Use `bash scripts/build.sh [toolchain] [build-dir]` to configure and build locally.
- Use `bash scripts/package_release.sh [outdir]` to produce archives and SHA256 locally before publishing a Release.
- To simulate CI on a dev machine, consider using `act` (https://github.com/nektos/act) but note differences in runner environments.

Tips for maintenance:

- Keep the CI matrix minimal and fast; avoid heavy external downloads.
- Run `--run` smoke tests that compile generated C — this catches portability/runtime regressions early.
- If you add dependencies (e.g., LLVM toolchain), install them explicitly in CI steps.