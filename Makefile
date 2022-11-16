CC = g++
CFLAGS = -O3
EXTRAFLAGS = -lpthread


all: sequential

sequential:  header.h
	$(CC) $(CFLAGS)  -o rainFall_seq rainFall_seq.cpp utils.cpp  $(EXTRAFLAGS)

.PHONY:
	clean
clean:
	rm -rf *.o rainFall_seq  