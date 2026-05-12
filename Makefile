.PHONY: all test clean editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

run-tests: compile-debug
	./build/taller_tests

all: clean run-tests

clean:
	rm -Rf build/

pre-commit:
	pre-commit run --hook-stage manual --files ./common/* ./client/* ./server/* ./editor/* ./tests/*

cmake:
	cmake --build cmake-build-debug