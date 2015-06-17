components += app
app_lib = true

include_dirs += \
	app/include

app_source_files = \
	app/src/app_db.cpp \
	app/src/app.cpp

app_deps = \
	$(BIN_DIR)/core_lib.a \
	$(database_deps) \
	$(BIN_DIR)/database_lib.a \
	$(network_deps) \
	$(BIN_DIR)/network_lib.a

components += app_test
app_test_exe = true

app_test_source_files = \
	app/test/test_app.cpp \
	app/test/test_app_db.cpp \
	app/test/test_app_api.cpp \
	app/unittests.cpp

app_test_deps = \
	$(app_deps) \
	$(BIN_DIR)/app_lib.a \
	$(BIN_DIR)/cpputest_lib.a

components += tracker
tracker_exe = true

tracker_source_files = \
	app/main.cpp

tracker_deps = \
	$(app_deps) \
	$(BIN_DIR)/app_lib.a
