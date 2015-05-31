include ../makefile.inc

DIRS = ./http

compile: JsonRpcConnector.o
	for i in $(DIRS); do make -C $$i; done
	$(MOVE_CMD)

clean:
	for i in $(DIRS); do make -C $$i clean; done
	$(CLEAN_CMD)
	
JsonRpcConnector.o: JsonRpcConnector.cpp
	$(CC) $(CFLAGS) -c $<
	cp $@ ../