
THREAD_NUM=2

make clean
make

./rainFall_para ${THREAD_NUM} 10 0.25 4 ./test_file/sample_4x4.in