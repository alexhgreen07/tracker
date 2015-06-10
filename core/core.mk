include_dirs += \
	core/include

core_source_files = \
	core/src/event.cpp \
	core/src/scheduler.cpp \
	core/src/task.cpp

core_obj_files = $(patsubst %,$(BIN_DIR)/%,$(core_source_files:.cpp=.o))

$(BIN_DIR)/core_lib.a: $(core_obj_files)

core_test_source_files = \
	core/test/test_event.cpp \
	core/test/test_scheduler.cpp \
	core/test/test_task.cpp \
	core/unittests.cpp

core_test_obj_files = $(patsubst %,$(BIN_DIR)/%,$(core_test_source_files:.cpp=.o))

$(BIN_DIR)/core_test.exe: $(core_test_obj_files) $(BIN_DIR)/core_lib.a $(BIN_DIR)/cpputest_lib.a