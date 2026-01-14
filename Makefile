.PHONY: all build clean

all: build

build:
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
	cd build && cmake --build .

clean:
	rm -rf build
