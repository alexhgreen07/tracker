include_dirs += \
	network/include

network_source_files = \
	network/src/server.cpp

network_obj_files = $(patsubst %,$(BIN_DIR)/%,$(network_source_files:.cpp=.o))

network_lib_deps = \
	$(BIN_DIR)/json_rpc_cpp_lib.a

$(BIN_DIR)/network_lib.a: $(network_obj_files) $(network_lib_deps)

network_test_source_files = \
	network/test/test_api.cpp \
	network/test/test_server.cpp \
	network/unittests.cpp

network_test_obj_files = $(patsubst %,$(BIN_DIR)/%,$(network_test_source_files:.cpp=.o))

network_test_deps = \
	$(network_lib_deps) \
	$(BIN_DIR)/network_lib.a \
	$(BIN_DIR)/cpputest_lib.a

$(BIN_DIR)/network_test.exe: $(network_test_obj_files) $(network_test_deps)