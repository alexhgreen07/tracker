CC = g++
CFLAGS = -std=c++11
BIN_DIR = bin

include_dirs = 

include core/core.mk

$(BIN_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< -I $(include_dirs) $(CFLAGS)

$(BIN_DIR)/%.a:
	ar rcs $@ $^

.PHONY: all
all: core_lib.a

.PHONY: clean
clean:
	rm -rf bin/