build: configure-cmake
	cmake --build build

configure-cmake:
	cmake -S . -B build

test: build
	cd build && ctest --rerun-failed --output-on-failure

clean:
	rm -rf build
