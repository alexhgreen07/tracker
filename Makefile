CC = g++
CFLAGS = -std=c++11
BIN_DIR = bin

.PHONY: all clean

include_dirs = 

include core/core.mk

$(BIN_DIR)/%.o: %.cpp
	@echo CC $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< -I $(include_dirs) $(CFLAGS)

$(BIN_DIR)/%.a:
	@echo AR $@
	@ar rcs $@ $^

all: core_lib.a

clean:
	rm -rf bin/