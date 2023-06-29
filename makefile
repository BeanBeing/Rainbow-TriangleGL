CC=g++

CFLAGS=-Wall -pedantic

LFLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

GLAD_DIR=glad
SRC_DIR=src
BUILD_DIR=build

OBJS=$(SRC_DIR)/main.cpp $(GLAD_DIR)/src/glad.c $(SRC_DIR)/shader.cpp

C_OUT = app.bin

.PHONY: test clean

app: $(SRC_DIR)/main.cpp
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(BUILD_DIR)/$(C_OUT)

test: $(BUILD_DIR)/$(C_OUT)
	./$(BUILD_DIR)/$(C_OUT)

clean: $(BUILD_DIR)/$(C_OUT)
	rm -rf $(BUILD_DIR)/$(C_OUT)
