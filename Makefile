.PHONY: all test clean editor client common server build

all: build

pre-commit:
	pre-commit run --hook-stage manual --files ./common/* ./client/* ./server/* ./editor/* ./tests/*

build:
	cmake --build cmake-build-debug

client: build
	./cmake-build-debug/taller_client -platform xcb

server: build
	./cmake-build-debug/taller_server 5050

test: build
	./cmake-build-debug/taller_tests
