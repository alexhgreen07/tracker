#defines all rules for a component
define COMPONENT_RULES

$(eval $(1)_obj_files_1 = $(patsubst %,$(BIN_DIR)/%,$($(1)_source_files:.c=.o)))
$(eval $(1)_obj_files = $($(1)_obj_files_1:.cpp=.o))

$(eval $(1)_deps_files = $($(1)_obj_files:.o=.d))
-include $($(1)_deps_files)

ifdef $(1)_lib
all: $(BIN_DIR)/$(1)_lib.a
.PHONY: $(1)_lib.a
$(1)_lib.a: $(BIN_DIR)/$(1)_lib.a
$(BIN_DIR)/$(1)_lib.a: $($(1)_obj_files) $($(1)_deps)
endif

ifdef $(1)_exe
all: $(BIN_DIR)/$(1).exe
.PHONY: $(1).exe
$(1).exe: $(BIN_DIR)/$(1).exe
$(BIN_DIR)/$(1).exe: $($(1)_obj_files) $($(1)_deps)
endif

endef

all: ;

#generate all component rules
$(foreach component,$(components),$(eval $(call COMPONENT_RULES,$(component))))

master_includes = $(patsubst %,-I %,$(include_dirs))

#compile c++ files to object
$(BIN_DIR)/%.o: %.cpp
	@echo CPP $@
	@mkdir -p $(dir $@)
	@$(CPP) -c -o $@ $< $(master_includes) $(CPPFLAGS)
	@$(CPP) -MM -MT $@ $< $(master_includes) $(CFLAGS) > $(BIN_DIR)/$*.d

#compile c files to object
$(BIN_DIR)/%.o: %.c
	@echo CC $@
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $< $(master_includes) $(CFLAGS)
	@$(CC) -MM -MT $@ $< $(master_includes) $(CFLAGS) > $(BIN_DIR)/$*.d

#compile object files to static library
$(BIN_DIR)/%.a:
	@echo AR $@
	@ar rcs $@ $^

#compile object/library files to executable
$(BIN_DIR)/%.exe:
	@echo EXE $@
	@$(CPP) -o $@ $^ $(master_includes) $(CFLAGS)

clean:
	rm -rf bin/
