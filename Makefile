CC = gcc
CPP = g++
CFLAGS =
CPPFLAGS = -std=c++11
BIN_DIR = bin

.PHONY: all clean

all: app_test.exe core_test.exe database_test.exe network_test.exe

clean:
	rm -rf bin/

components =

include_dirs = 

include externals/externals.mk
include core/core.mk
include database/database.mk
include network/network.mk
include app/app.mk

include mk/rules.mk
