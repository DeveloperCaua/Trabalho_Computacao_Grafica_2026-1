
CC = gcc
CFLAGS = -Iinclude -Wall -std=c99
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lm

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = main.exe

all: $(OBJ_DIR) $(TARGET) copy_dll

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

copy_dll:
	copy /Y SDL2-2.30.3\i686-w64-mingw32\bin\SDL2.dll .
	@echo SDL2.dll copiado para a raiz.

clean:
	rmdir /S /Q $(OBJ_DIR)
	del $(TARGET)
