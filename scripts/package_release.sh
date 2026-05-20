#!/usr/bin/env bash
# Build and package pearlcd for the current platform. Produce archive and SHA256.
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
TOOLCHAIN=""
OUT_DIR="$ROOT_DIR/dist"
BUILD_DIR="$ROOT_DIR/build-release"

case "${1:-}" in
  linux|macos|mingw|msvc)
    TOOLCHAIN="$1"
    OUT_DIR=${2:-$ROOT_DIR/dist}
    BUILD_DIR=${3:-$ROOT_DIR/build-release-$TOOLCHAIN}
    ;;
  "")
    OUT_DIR=${2:-$ROOT_DIR/dist}
    BUILD_DIR=${3:-$ROOT_DIR/build-release}
    ;;
  *)
    OUT_DIR="$1"
    BUILD_DIR=${2:-$ROOT_DIR/build-release}
    ;;
esac

mkdir -p "$OUT_DIR"

echo "Building project (Release)..."
bash "$ROOT_DIR/scripts/build.sh" "$TOOLCHAIN" "$BUILD_DIR"

BIN="$BUILD_DIR/pearlcd"
if [ ! -f "$BIN" ]; then
  # maybe Windows executable
  BIN="$BUILD_DIR/pearlcd.exe"
fi

if [ ! -f "$BIN" ]; then
  echo "Built binary not found: $BIN" >&2
  exit 2
fi

pushd "$OUT_DIR" >/dev/null
if [[ "$BIN" == *.exe ]]; then
  ZIP=pearlcd-windows.zip
  zip -j "$ZIP" "$BIN"
  sha256=$(sha256sum "$ZIP" | awk '{print $1}')
  echo "$sha256" > "$ZIP.sha256"
  echo "Created $ZIP and $ZIP.sha256"
else
  TAR=pearlcd-linux.tar.gz
  tar -czf "$TAR" -C "$(dirname "$BIN")" "$(basename "$BIN")"
  sha256=$(sha256sum "$TAR" | awk '{print $1}')
  echo "$sha256" > "$TAR.sha256"
  echo "Created $TAR and $TAR.sha256"
fi
popd >/dev/null

echo "Packaging complete. Artifacts in $OUT_DIR"
