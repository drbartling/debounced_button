.PHONY: default
default: build
	cmake --build build

build:
	cmake -S . -B build

test: default
	cd build && ctest --rerun-failed --output-on-failure

clean:
	rm -rf build
