default:
  just --list


build:
    cmake --build build

alias cdebug := configure-debug
configure-debug:
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug \
        -DFEATURE_TESTS=ON

alias crelease := configure-release
configure-release:
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DFEATURE_TESTS=ON

test: build
    cd build && \
        ctest --rerun-failed --output-on-failure
    gcovr

clean:
    rm -rf build
