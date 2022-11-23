CC = g++
CFLAGS = -O3
LIBS = -ltimer
LIB_DIRS = -L.


all: sequential

sequential:  header.h
	$(CC) $(CFLAGS) -o rainFall_seq rainFall_seq.cpp utils.cpp $(LIB_DIRS) $(LIBS)

.PHONY:
	clean
clean:
	rm -rf *.o rainFall_seq  