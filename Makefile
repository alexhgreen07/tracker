CC = g++
CFLAGS = -std=c++11
BIN_DIR = bin

all: core.a
clean:
	rm -rf $(BIN_DIR)

include core/core.mk
