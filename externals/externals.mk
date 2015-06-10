# CppUTest Library
include_dirs += \
	externals/cpputest/include

cpputest_source_files = \
	externals/cpputest/src/CppUTest/CommandLineArguments.cpp \
	externals/cpputest/src/CppUTest/CommandLineTestRunner.cpp \
	externals/cpputest/src/CppUTest/JUnitTestOutput.cpp \
	externals/cpputest/src/CppUTest/MemoryLeakDetector.cpp \
	externals/cpputest/src/CppUTest/MemoryLeakWarningPlugin.cpp \
	externals/cpputest/src/CppUTest/SimpleMutex.cpp \
	externals/cpputest/src/CppUTest/SimpleString.cpp \
	externals/cpputest/src/CppUTest/TestFailure.cpp \
	externals/cpputest/src/CppUTest/TestFilter.cpp \
	externals/cpputest/src/CppUTest/TestHarness_c.cpp \
	externals/cpputest/src/CppUTest/TestMemoryAllocator.cpp \
	externals/cpputest/src/CppUTest/TestOutput.cpp \
	externals/cpputest/src/CppUTest/TestPlugin.cpp \
	externals/cpputest/src/CppUTest/TestRegistry.cpp \
	externals/cpputest/src/CppUTest/TestResult.cpp \
	externals/cpputest/src/CppUTest/Utest.cpp \
	externals/cpputest/src/Platforms/Gcc/UtestPlatform.cpp

cpputest_obj_files = $(patsubst %,$(BIN_DIR)/%,$(cpputest_source_files:.cpp=.o))

$(BIN_DIR)/cpputest_lib.a: $(cpputest_obj_files)

# Sqlite3 Library
include_dirs += \
	externals/sqllite3

sqllite3_source_files = \
	externals/sqllite3/sqlite3.c

sqllite3_obj_files = $(patsubst %,$(BIN_DIR)/%,$(sqllite3_source_files:.c=.o))

$(BIN_DIR)/sqllite3_lib.a: $(sqllite3_obj_files)

# JSON-RPC CPP

include_dirs += \
	externals/json-rpc-cpp

json_rpc_cpp_source_files = \
	externals/json-rpc-cpp/JsonRpcErrors.cpp \
	externals/json-rpc-cpp/JsonRpcException.cpp \
	externals/json-rpc-cpp/JsonRpcProcedure.cpp \
	externals/json-rpc-cpp/JsonRpcRequestHandler.cpp \
	externals/json-rpc-cpp/JsonRpcServer.cpp \
	externals/json-rpc-cpp/json/json_reader.cpp \
	externals/json-rpc-cpp/json/json_value.cpp \
	externals/json-rpc-cpp/json/json_writer.cpp \
	externals/json-rpc-cpp/connectors/JsonRpcConnector.cpp \
	externals/json-rpc-cpp/connectors/http/HTTPConnector.cpp

mongoose_source_files = \
	externals/json-rpc-cpp/connectors/http/mongoose.c

json_rpc_cpp_obj_files = $(patsubst %,$(BIN_DIR)/%,$(json_rpc_cpp_source_files:.cpp=.o))
json_rpc_cpp_obj_files += $(patsubst %,$(BIN_DIR)/%,$(mongoose_source_files:.c=.o))

$(BIN_DIR)/json_rpc_cpp_lib.a: $(json_rpc_cpp_obj_files)



