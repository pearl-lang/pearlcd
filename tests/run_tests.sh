#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$ROOT"

echo "Running dependency checks..."
bash scripts/check-dep.sh

echo "Building project..."
cmake -S . -B build
cmake --build build -- -j$(nproc || echo 2)

echo "Running unit tests..."
./build/pearl_tests || (echo "Unit tests failed" && exit 2)

echo "Running integration tests (CLI)..."
# test examples/test.prl run output should contain '15'
OUT=$(./build/pearlcd --run examples/test.prl)
echo "run output: $OUT"
if ! echo "$OUT" | grep -q "15"; then
  echo "Integration test failed: expected 15 in output" >&2
  exit 3
fi

# print_formats example
OUT2=$(./build/pearlcd --run examples/print_formats.prl)
echo "$OUT2"
if ! echo "$OUT2" | grep -q "Sum: 15"; then
  echo "Integration test failed for print_formats.prl" >&2
  exit 4
fi

echo "All tests passed."
