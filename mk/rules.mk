#defines all rules for a component
define COMPONENT_RULES

$(eval $(1)_obj_files_1 = $(patsubst %,$(BIN_DIR)/%,$($(1)_source_files:.c=.o)))
$(eval $(1)_obj_files = $($(1)_obj_files_1:.cpp=.o))

ifdef $(1)_lib
.PHONY: $(1)_lib.a
$(1)_lib.a: $(BIN_DIR)/$(1)_lib.a
$(BIN_DIR)/$(1)_lib.a: $($(1)_obj_files) $($(1)_deps)
endif

ifdef $(1)_exe
.PHONY: $(1).exe
$(1).exe: $(BIN_DIR)/$(1).exe
$(BIN_DIR)/$(1).exe: $($(1)_obj_files) $($(1)_deps)
endif

endef

#generate all component rules
$(foreach component,$(components),$(eval $(call COMPONENT_RULES,$(component))))

master_includes = $(patsubst %,-I %,$(include_dirs))

#compile c++ files to object
$(BIN_DIR)/%.o: %.cpp
	@echo CPP $@
	@mkdir -p $(dir $@)
	@$(CPP) -c -o $@ $< $(master_includes) $(CPPFLAGS)

#compile c files to object
$(BIN_DIR)/%.o: %.c
	@echo CC $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(master_includes) $(CFLAGS)

#compile object files to static library
$(BIN_DIR)/%.a:
	@echo AR $@
	@ar rcs $@ $^

#compile object/library files to executable
$(BIN_DIR)/%.exe:
	@echo EXE $@
	@$(CPP) -o $@ $^ $(master_includes) $(CFLAGS)
