components += database
database_lib = true

include_dirs += \
	database/include

database_source_files = \
	database/src/database.cpp

database_deps = \
	$(BIN_DIR)/sqllite3_lib.a

components += database_test
database_test_exe = true

database_test_source_files = \
	database/test/test_database.cpp \
	database/unittests.cpp

database_test_deps = \
	$(database_deps) \
	$(BIN_DIR)/database_lib.a \
	$(BIN_DIR)/cpputest_lib.a

