CC = g++
CFLAGS = -std=c++11
BIN_DIR = bin

.PHONY: all clean

all: $(BIN_DIR)/core_test.exe

clean:
	rm -rf bin/

include_dirs = 

include externals/externals.mk
include core/core.mk

master_includes = $(patsubst %,-I %,$(include_dirs))

$(BIN_DIR)/%.o: %.cpp
	@echo CC $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(master_includes) $(CFLAGS)

$(BIN_DIR)/%.a:
	@echo AR $@
	@ar rcs $@ $^

$(BIN_DIR)/%.exe:
	@echo EXE $@
	@$(CC) -o $@ $^ $(master_includes) $(CFLAGS)