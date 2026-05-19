# Simple Makefile wrapper around CMake and project scripts
BUILD_DIR ?= build
OUT_DIR ?= out
DIST_DIR ?= dist
JOBS ?= $(shell nproc 2>/dev/null || echo 2)

.PHONY: all configure build clean package dist check-deps test run parse lower codegen docs

all: build

configure:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR) -- -j$(JOBS)

run: build
	$(BUILD_DIR)/pearlcd --run examples/test.prl

parse: build
	$(BUILD_DIR)/pearlcd --parse examples/test.prl

lower: build
	$(BUILD_DIR)/pearlcd --lower examples/test.prl

codegen: build
	$(BUILD_DIR)/pearlcd --codegen examples/test.prl > $(DIST_DIR)/codegen.c

package: build
	bash scripts/package_release.sh $(DIST_DIR)

dist: package

check-deps:
	bash scripts/check-dep.sh

docs:
	bash scripts/docs.sh

test:
	bash tests/run_tests.sh

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR) $(OUT_DIR)
