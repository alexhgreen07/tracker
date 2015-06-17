CC = gcc
CPP = g++
CFLAGS = -g
CPPFLAGS = -g -std=c++11
BIN_DIR = bin

.PHONY: all clean

components =

include_dirs = 

include externals/externals.mk
include core/core.mk
include database/database.mk
include network/network.mk
include app/app.mk

include mk/rules.mk
