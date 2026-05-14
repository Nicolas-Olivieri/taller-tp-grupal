.PHONY: all test clean editor client common server build

# Directorio sobre el que se contruye el build
BUILD_DIR= cmake-build-debug

# Todos los archivos con código fuente
SOURCE_CODE+= ./common/*
SOURCE_CODE+= ./client/*
SOURCE_CODE+= ./server/*
SOURCE_CODE+= ./editor/*
SOURCE_CODE+= ./tests/*

# Nombre de los compilados
CLIENT_EXE= taller_client
SERVER_EXE= taller_server
EDITOR_EXE= taller_editor
TESTS_EXE= taller_tests

# Argumentos para la ejecución de los compilados
CLIENT_FLAGS= -platform xcb
SERVER_FLAGS= 5050
EDITOR_FLAGS= # vacío de momento, agregar los necesarios
TESTS_FLAGS= # vacío de momento, agregar los necesarios

all: build

pre-commit:
	pre-commit run --hook-stage manual --files $(SOURCE_CODE)

build:
	cmake --build $(BUILD_DIR) >/dev/null

client: build
	$(BUILD_DIR)/$(CLIENT_EXE) $(CLIENT_FLAGS)

server: build
	$(BUILD_DIR)/$(SERVER_EXE) $(SERVER_FLAGS)

editor: build
	$(BUILD_DIR)/$(EDITOR_EXE) $(EDITOR_FLAGS)

test: build
	$(BUILD_DIR)/$(TESTS_EXE) $(EDITOR_FLAGS)
