CC = g++
CFLAGS = -O3 -std=c++11
PARALLEL = -DPARALLEL -pthread
SEQUENTIAL = -DSEQUENTIAL -L. -ltimer
PROFILE = -DPROFILE


all: rainfall_seq rainfall_pt

rainfall_seq:  header.h
	$(CC) $(CFLAGS) -o rainfall_seq rainFall_seq.cpp utils.cpp ${SEQUENTIAL} ${PROFILE}

rainfall_pt: header.h
	$(CC) $(CFLAGS) -o rainfall_pt rainFall_para.cpp utils.cpp  ${PARALLEL}

.PHONY:
	clean
clean:
	rm -rf *.o rainFall_seq rainFall_para  