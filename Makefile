CC = gcc
CFLAGS = -Iinclude -Wall -std=c99

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

ifeq ($(OS),Windows_NT)
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lm
TARGET = main.exe
else
LDFLAGS = -lSDL2 -lm
TARGET = main
endif

MKDIR_CMD = mkdir -p $(OBJ_DIR)
COPY_CMD = cp -f
RM_CMD = rm -f
RMRF_CMD = rm -rf

SDL_DLL_PATH = SDL2-2.30.3/i686-w64-mingw32/bin/SDL2.dll

.PHONY: all clean copy_dll

all: $(OBJ_DIR) $(TARGET) copy_dll

$(OBJ_DIR):
	$(MKDIR_CMD)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ifeq ($(OS),Windows_NT)
copy_dll:
	@if [ -f "$(SDL_DLL_PATH)" ]; then \
		$(COPY_CMD) "$(SDL_DLL_PATH)" . && echo SDL2.dll copiado para a raiz.; \
	else \
		echo "SDL2.dll não encontrado em $(SDL_DLL_PATH), pulando cópia."; \
	fi
else
copy_dll:
	@echo Nenhuma cópia de DLL necessária no Unix.
endif

clean:
	$(RMRF_CMD) $(OBJ_DIR)
	$(RM_CMD) $(TARGET)