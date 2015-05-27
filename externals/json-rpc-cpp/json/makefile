include ../makefile.inc

JSON_OBJ = json_reader.o json_writer.o json_value.o
LIB_PATH = $(LIB_PATH) -L `pwd`

all: libjsoncpp.so libjsoncpp.a
	$(MOVE_CMD)

libjsoncpp.so: $(JSON_OBJ)
	$(CC) $(CFLAGS) -shared $+ -o $@
	
libjsoncpp.a: $(JSON_OBJ)
	ar -r $@ $+
	
clean:
	$(CLEAN_CMD)