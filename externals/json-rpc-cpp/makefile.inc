CC = g++
RM = rm -rf
CFLAGS = -fpic
LIBS = 
LIB_PATH = -L `pwd`

#Makros
MOVE_CMD = cp *.so ../; cp *.a ../;
CLEAN_CMD = rm -f *.o; rm -f *.so; rm -f *.a;

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
