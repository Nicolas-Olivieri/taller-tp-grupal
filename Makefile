.PHONY: all test clean editor client common server build valgrind-client valgrind-server valgrind-editor valgrind-test

# Directorio sobre el que se construye el build
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
EDITOR_FLAGS= -platform xcb
TESTS_FLAGS= # vacío de momento, agregar los necesarios

# Archivos a borrar en rutina clean
CLEAN_FILES= ./resources/index.bin
CLEAN_FILES+= ./resources/playerdata.bin

# Identificadores para las bitácoras de eventos
SERVER_ID= "Argentum-Online-server"
CLIENT_ID= "Argentum-Online-client"

# Valgrind
VALGRIND_FLAGS= --tool=memcheck --leak-check=full --leak-resolution=med --show-reachable=yes --trace-children=yes --track-fds=yes --track-origins=no --time-stamp=yes --num-callers=20 --error-exitcode=42
VALGRIND_LOG_FILE= ./log_valgrind.txt

all: build

pre-commit:
	pre-commit run --hook-stage manual --all-files

build:
	cmake --build $(BUILD_DIR) >/dev/null

error:
	cmake --build $(BUILD_DIR)

client: build
	$(BUILD_DIR)/$(CLIENT_EXE) $(CLIENT_FLAGS)

server: build
	$(BUILD_DIR)/$(SERVER_EXE) $(SERVER_FLAGS)

editor: build
	$(BUILD_DIR)/$(EDITOR_EXE) $(EDITOR_FLAGS)

test: build
	$(BUILD_DIR)/$(TESTS_EXE) $(EDITOR_FLAGS)

valgrind-client: build
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(CLIENT_EXE) $(CLIENT_FLAGS)

valgrind-server: build
	@touch $(VALGRIND_LOG_FILE)
	@rm $(VALGRIND_LOG_FILE)
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(SERVER_EXE) $(SERVER_FLAGS) > $(VALGRIND_LOG_FILE) 2>&1
	@tail $(VALGRIND_LOG_FILE)

valgrind-editor: build
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(EDITOR_EXE) $(EDITOR_FLAGS)

valgrind-test: build
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(TESTS_EXE) $(EDITOR_FLAGS)

clean:
	@touch $(CLEAN_FILES)
	@rm $(CLEAN_FILES)

valgrind-clean:
	@touch $(VALGRIND_LOG_FILE)
	@rm $(VALGRIND_LOG_FILE)

log-server:
	journalctl -t $(SERVER_ID) -f

log-client:
	journalctl -t $(CLIENT_ID) -f