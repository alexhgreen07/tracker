include_dirs += \
	database/include

database_source_files = \
	database/src/database.cpp

database_obj_files = $(patsubst %,$(BIN_DIR)/%,$(database_source_files:.cpp=.o))

database_lib_deps = \
	$(BIN_DIR)/sqllite3_lib.a

$(BIN_DIR)/database_lib.a: $(database_obj_files) $(database_lib_deps)

database_test_source_files = \
	database/test/test_database.cpp \
	database/unittests.cpp

database_test_obj_files = $(patsubst %,$(BIN_DIR)/%,$(database_test_source_files:.cpp=.o))

database_test_deps = \
	$(database_lib_deps) \
	$(BIN_DIR)/database_lib.a \
	$(BIN_DIR)/cpputest_lib.a

$(BIN_DIR)/database_test.exe: $(database_test_obj_files) $(database_test_deps)