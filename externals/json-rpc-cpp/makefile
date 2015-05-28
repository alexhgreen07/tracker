#####
# written by Peter Spiess-Knafl
#####
include makefile.inc

OBJS = JsonRpcConnector.o JsonRpcErrors.o JsonRpcException.o JsonRpcProcedure.o JsonRpcRequestHandler.o JsonRpcServer.o 
DIRS = ./json ./connectors
LIBS = jsoncpp httpconnector
SO_LIB = $(LIBS:%=lib%.so)
AR_LIB = $(LIBS:%=lib%.a)

LINK_SO = $(LIBS:%=-l%)
LINK_AR = $(LIBS:%=lib%.a)
LD_PATH = -L"`pwd`"

compile: 
	for i in $(DIRS); do make -C $$i; done
	make libjson-rpc-cpp.so 
	make libjson-rpc-cpp.a

libjson-rpc-cpp.so: $(OBJS) $(SO_LIB)
	$(CC) $(CFLAGS) $(LD_PATH) -shared -o $@ $(OBJS) $(LINK_SO)
	
libjson-rpc-cpp.a: $(OBJS) $(AR_LIB)
	ar -r $@ $(OBJS) $(LINK_AR)

prepare: compile
	test -d lib || mkdir lib
	test -d include || mkdir include
	test -d include/json-rpc-cpp || mkdir -p include/json-rpc-cpp
	cp *.so lib
	cp *.a lib
	find -type f -name "*.h" -exec cp --parents {} ./include/json-rpc-cpp \;
	rm -r ./include/json-rpc-cpp/include
	
install: prepare
	cp -r ./include/json-rpc-cpp /usr/include
	#test -d /usr/lib/json-rpc-cpp || mkdir /usr/lib/json-rpc-cpp
	cp -r ./lib/* /usr/lib
	
uninstall:
	rm -rf /usr/include/json-rpc-cpp
	rm -rf /us/lib/json-rpc-cpp
	
clean:
	for i in $(DIRS); do make -C $$i clean; done
	$(CLEAN_CMD)
	rm -rf include
	rm -rf lib
