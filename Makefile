.PHONY: all build clean test

all: build

build:
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
	cd build && cmake --build .

test: build
	mkdir -p testdir
	cd build && ctest --build-config Debug --output-on-failure

clean:
	rm -rf build
