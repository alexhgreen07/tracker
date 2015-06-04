core_include_dirs = \
	core/include

core_source_files = \
	core/src/event.o \
	core/src/scheduler.o \
	core/src/task.o
core_source_objects = $(addprefix $(BIN_DIR)/,$(core_source_files:.cpp=.o))

core.a: $(BIN_DIR)/core.a

$(BIN_DIR)/core.a: $(core_source_objects)
	ar -cvq $@ $(core_source_objects)

$(core_source_objects): $(BIN_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(core_include_dirs) -c $< -o $@