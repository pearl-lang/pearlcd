#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
LANGUAGE=${1:-all}
OUT_DIR=${2:-$ROOT_DIR/docs/dist}

build_doc() {
	local lang="$1"
	local src_root="$ROOT_DIR/docs/$lang"

	if [[ ! -d "$src_root" ]]; then
		echo "Documentation source not found: $src_root" >&2
		exit 2
	fi

	if ! command -v pandoc >/dev/null 2>&1; then
		echo "pandoc is required to build docs" >&2
		exit 2
	fi

	find "$src_root" -name '*.md' | while IFS= read -r src; do
		local rel_path="${src#"$src_root"/}"
		local rel_dir
		local base_name
		local out_dir

		rel_dir=$(dirname "$rel_path")
		base_name=${rel_path%.md}
		if [[ "$rel_dir" == "." ]]; then
			out_dir="$OUT_DIR/$lang"
		else
			out_dir="$OUT_DIR/$lang/$rel_dir"
		fi

		mkdir -p "$out_dir"
		pandoc "$src" -o "$out_dir/$(basename "$base_name").html"
		pandoc "$src" -o "$out_dir/$(basename "$base_name").pdf"
	done

	echo "Built $lang docs in $OUT_DIR/$lang"
}

case "$LANGUAGE" in
	tr|en)
		build_doc "$LANGUAGE"
		;;
	all)
		build_doc tr
		build_doc en
		;;
	*)
		echo "Usage: bash scripts/docs.sh [tr|en|all] [out-dir]" >&2
		exit 2
		;;
esac
