#!/usr/bin/env bash
# Simple helper to configure and build with optional toolchain selection.
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR=${2:-${ROOT_DIR}/build}
TOOLCHAIN=${1:-}

case "$TOOLCHAIN" in
  "")
    TOOLCHAIN_FILE=""
    ;;
  linux)
    TOOLCHAIN_FILE=""
    ;;
  macos)
    TOOLCHAIN_FILE="$ROOT_DIR/toolchain/macos.cmake"
    ;;
  mingw)
    TOOLCHAIN_FILE="$ROOT_DIR/toolchain/windows-mingw.cmake"
    ;;
  msvc)
    TOOLCHAIN_FILE="$ROOT_DIR/toolchain/windows-msvc.cmake"
    ;;
  *)
    echo "Unknown toolchain: $TOOLCHAIN" >&2
    exit 2
    ;;
esac

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

CM_CMD=(cmake -S "$ROOT_DIR" -B "$BUILD_DIR")
if [ -n "$TOOLCHAIN_FILE" ]; then
  CM_CMD+=( -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" )
fi

echo "Configuring with: ${CM_CMD[*]}"
"${CM_CMD[@]}"

echo "Building..."
cmake --build "$BUILD_DIR" -- -j$(nproc || echo 2)

echo "Build finished. To run: $BUILD_DIR/pearlcd --run <example.prl>"
echo "To package release artifacts locally: scripts/package_release.sh [outdir]"

