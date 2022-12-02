CC = g++
CFLAGS = -O3 -std=c++11
PARALLEL = -DPARALLEL -pthread
SEQUENTIAL = -DSEQUENTIAL -L. -ltimer
PROFILE = -DPROFILE


all: sequential parallel

sequential:  header.h
	$(CC) $(CFLAGS) -o rainFall_seq rainFall_seq.cpp utils.cpp ${SEQUENTIAL}

parallel: header.h
	$(CC) $(CFLAGS) -o rainFall_para rainFall_para.cpp utils.cpp  ${PARALLEL}

.PHONY:
	clean
clean:
	rm -rf *.o rainFall_seq rainFall_para  