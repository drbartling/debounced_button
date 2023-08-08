.PHONY: default
default: build
	cmake --build build

build:
	cmake -S . -B build -G Ninja

test: default
	cd build && ctest --rerun-failed --output-on-failure

clean:
	rm -rf build
	make --directory lib/common clean
