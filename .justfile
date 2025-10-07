default:
    just --list


build:
    {{ if "true" == path_exists("build") {""} else {"just cdebug"} }}
    cmake --build build

alias cdebug := configure-debug
configure-debug:
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug

alias crelease := configure-release
configure-release:
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release

enable-tests:
    {{ if "true" == path_exists("build") {""} else {"just cdebug"} }}
    cmake build -DFEATURE_TESTS=ON

test: enable-tests build
    cd build && \
        ctest --rerun-failed --output-on-failure
    gcovr

clean:
    rm -rf build
