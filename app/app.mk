include_dirs += \
	app/include

app_source_files = \
	app/src/appdb.cpp \
	app/src/app.cpp

app_obj_files = $(patsubst %,$(BIN_DIR)/%,$(app_source_files:.cpp=.o))

app_lib_deps = \
	$(BIN_DIR)/core_lib.a \
	$(database_lib_deps) \
	$(BIN_DIR)/database_lib.a \
	$(network_lib_deps) \
	$(BIN_DIR)/network_lib.a

$(BIN_DIR)/app_lib.a: $(app_obj_files) $(app_lib_deps)

app_test_source_files = \
	app/test/test_app.cpp \
	app/test/test_appdb.cpp \
	app/unittests.cpp

app_test_obj_files = $(patsubst %,$(BIN_DIR)/%,$(app_test_source_files:.cpp=.o))

app_test_deps = \
	$(app_lib_deps) \
	$(BIN_DIR)/app_lib.a \
	$(BIN_DIR)/cpputest_lib.a

$(BIN_DIR)/app_test.exe: $(app_test_obj_files) $(app_test_deps)