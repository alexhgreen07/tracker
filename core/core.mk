include_dirs += \
	core/include

core_source_files = \
	core/src/event.o \
	core/src/scheduler.o \
	core/src/task.o

core_obj_files = $(patsubst %,$(BIN_DIR)/%,$(core_source_files))

$(BIN_DIR)/core_lib.a: $(core_obj_files)