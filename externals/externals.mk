# CppUTest Library
components += cpputest
cpputest_lib = true

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

# Sqlite3 Library
components += sqllite3
sqllite3_lib = true

include_dirs += \
	externals/sqllite3

sqllite3_source_files = \
	externals/sqllite3/sqlite3.c

# JSON-RPC CPP
components += json_rpc_cpp
json_rpc_cpp_lib = true

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
	externals/json-rpc-cpp/connectors/http/HTTPConnector.cpp \
	externals/json-rpc-cpp/connectors/http/mongoose.c



