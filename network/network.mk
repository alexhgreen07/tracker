components += network
network_lib = true

include_dirs += \
	network/include

network_source_files = \
	network/src/server.cpp

network_deps = \
	$(BIN_DIR)/json_rpc_cpp_lib.a

components += network_test
network_test_exe = true

network_test_source_files = \
	network/test/test_server.cpp \
	network/unittests.cpp

network_test_deps = \
	$(network_deps) \
	$(BIN_DIR)/network_lib.a \
	$(BIN_DIR)/cpputest_lib.a

