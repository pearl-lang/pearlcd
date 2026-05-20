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
		# Always try to build HTML. PDF may fail if LaTeX is not installed; don't abort the whole build for PDF failures.
		pandoc "$src" -o "$out_dir/$(basename "$base_name").html"
		if ! pandoc "$src" -o "$out_dir/$(basename "$base_name").pdf" 2>/tmp/pandoc-pdf.err; then
			echo "Warning: PDF generation failed for $src (continuing):" >&2
			sed -n '1,120p' /tmp/pandoc-pdf.err >&2 || true
		fi
	done

	echo "Built $lang docs in $OUT_DIR/$lang"

	# Build a combined book (single HTML and an attempted PDF) from all markdown files in this language.
	{
		mapfile -t md_files < <(find "$src_root" -name '*.md' | sort)
		if [[ ${#md_files[@]} -gt 0 ]]; then
			local book_out_dir="$OUT_DIR/$lang/book"
			mkdir -p "$book_out_dir"
			# Combined HTML
			pandoc "${md_files[@]}" -s -o "$book_out_dir/pearlcd-$lang-book.html" || echo "Warning: combined HTML generation failed for $lang"
			# Combined PDF (best-effort; do not fail the script if LaTeX missing)
			if ! pandoc "${md_files[@]}" -s -o "$book_out_dir/pearlcd-$lang-book.pdf" 2>/tmp/pandoc-book-pdf.err; then
				echo "Warning: combined PDF generation failed for $lang (continuing):" >&2
				sed -n '1,120p' /tmp/pandoc-book-pdf.err >&2 || true
			fi
			echo "Built combined book for $lang in $book_out_dir"
		fi
	} || true
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
