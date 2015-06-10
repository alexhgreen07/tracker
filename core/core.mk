.PHONY: core_lib.a

include_dirs += \
	core/include

core_source_files = \
	core/src/event.cpp \
	core/src/scheduler.cpp \
	core/src/task.cpp

core_obj_files = $(patsubst %,$(BIN_DIR)/%,$(core_source_files:.cpp=.o))

$(BIN_DIR)/core_lib.a: $(core_obj_files)