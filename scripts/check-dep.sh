#!/usr/bin/env bash
set -euo pipefail

missing=0
check_cmd() {
	if ! command -v "$1" >/dev/null 2>&1; then
		echo "MISSING: $1"
		missing=1
	else
		echo "OK: $1 -> $(command -v $1)"
	fi
}

echo "Checking required build tools..."
check_cmd cmake
check_cmd gcc
check_cmd make
check_cmd bison
check_cmd flex
check_cmd tar
check_cmd zip || true
check_cmd sha256sum || check_cmd shasum || true

if [ "$missing" -ne 0 ]; then
	echo "One or more required commands are missing. Please install them and re-run this script." >&2
	exit 2
fi

echo "All required commands appear present."
