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