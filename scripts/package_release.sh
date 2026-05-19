#!/usr/bin/env bash
# Build and package pearlcd for the current platform. Produce archive and SHA256.
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
OUT_DIR=${1:-$ROOT_DIR/dist}
mkdir -p "$OUT_DIR"

echo "Building project (Release)..."
cmake -S "$ROOT_DIR" -B "$ROOT_DIR/build" -DCMAKE_BUILD_TYPE=Release
cmake --build "$ROOT_DIR/build" -- -j$(nproc || echo 2)

BIN="$ROOT_DIR/build/pearlcd"
if [ ! -f "$BIN" ]; then
  # maybe Windows executable
  BIN="$ROOT_DIR/build/pearlcd.exe"
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
