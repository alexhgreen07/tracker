include ../../makefile.inc


compile: libhttpconnector.so libhttpconnector.a
	$(MOVE_CMD)

libhttpconnector.so: mongoose.o HTTPConnector.o ../JsonRpcConnector.o
	$(CC) $(CFLAGS) -shared $+ -o $@

libhttpconnector.a: mongoose.o HTTPConnector.o ../JsonRpcConnector.o
	ar -r $@ $+
	
mongoose.o:
	gcc $(CFLAGS) -ldl -c mongoose.c -o mongoose.o
	
clean:
	$(CLEAN_CMD)