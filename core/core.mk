components += core
core_lib = true

include_dirs += \
	core/include

core_source_files = \
	core/src/event.cpp \
	core/src/scheduler.cpp \
	core/src/task.cpp

components += core_test
core_test_exe = true

core_test_source_files = \
	core/test/test_event.cpp \
	core/test/test_scheduler.cpp \
	core/test/test_task.cpp \
	core/unittests.cpp

core_test_deps = \
	$(BIN_DIR)/core_lib.a \
	$(BIN_DIR)/cpputest_lib.a

