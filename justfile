default: build

build:
    cc src/*.c -o main

fmt:
    clang-format -i src/*.c src/*.h

run: build
    ./main
