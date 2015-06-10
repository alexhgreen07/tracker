CC = gcc
CPP = g++
CFLAGS =
CPPFLAGS = -std=c++11
BIN_DIR = bin

.PHONY: all clean

all: $(BIN_DIR)/app_test.exe $(BIN_DIR)/core_test.exe $(BIN_DIR)/database_test.exe $(BIN_DIR)/network_test.exe

clean:
	rm -rf bin/

include_dirs = 

include externals/externals.mk
include core/core.mk
include database/database.mk
include network/network.mk
include app/app.mk

master_includes = $(patsubst %,-I %,$(include_dirs))

$(BIN_DIR)/%.o: %.cpp
	@echo CPP $@
	@mkdir -p $(dir $@)
	@$(CPP) -c -o $@ $< $(master_includes) $(CPPFLAGS)

$(BIN_DIR)/%.o: %.c
	@echo CC $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(master_includes) $(CFLAGS)

$(BIN_DIR)/%.a:
	@echo AR $@
	@ar rcs $@ $^

$(BIN_DIR)/%.exe:
	@echo EXE $@
	@$(CPP) -o $@ $^ $(master_includes) $(CFLAGS)
