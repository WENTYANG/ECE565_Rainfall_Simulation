THREAD_NUM=4

make clean
make parallel

PROGRAM_NAME="rainFall_para"
INPUT_FILE_PATH="./test_file"
OUTPUT_FILE_PATH="./para_output_file"
VALIDATION_FILE_PATH="./test_file"

if [ ! -d ${OUTPUT_FILE_PATH} ]; then
  mkdir ${OUTPUT_FILE_PATH}
fi

echo "running ./${PROGRAM_NAME} for different parameters"

# ./${PROGRAM_NAME} ${THREAD_NUM} 10 0.25 4 ${INPUT_FILE_PATH}/sample_4x4.in > ${OUTPUT_FILE_PATH}/sample_4x4.out
# ./${PROGRAM_NAME} ${THREAD_NUM} 20 0.5 16 ${INPUT_FILE_PATH}/sample_16x16.in >  ${OUTPUT_FILE_PATH}/sample_16x16.out
# ./${PROGRAM_NAME} ${THREAD_NUM} 20 0.5 32 ${INPUT_FILE_PATH}/sample_32x32.in >  ${OUTPUT_FILE_PATH}/sample_32x32.out 
# ./${PROGRAM_NAME} ${THREAD_NUM} 30 0.25 128 ${INPUT_FILE_PATH}/sample_128x128.in >  ${OUTPUT_FILE_PATH}/sample_128x128.out 
# ./${PROGRAM_NAME} ${THREAD_NUM} 30 0.75 512 ${INPUT_FILE_PATH}/sample_512x512.in >  ${OUTPUT_FILE_PATH}/sample_512x512.out 
# ./${PROGRAM_NAME} ${THREAD_NUM} 35 0.5 2048 ${INPUT_FILE_PATH}/sample_2048x2048.in >  ${OUTPUT_FILE_PATH}/sample_2048x2048.out
# ./${PROGRAM_NAME} ${THREAD_NUM} 50 0.5 4096 ${INPUT_FILE_PATH}/measurement_4096x4096.in >  ${OUTPUT_FILE_PATH}/measurement_4096x4096.out


# used for single test
# ./rainFall_para 4 10 0.25 4 ./test_file/sample_4x4.in
# ./rainFall_para 4 20 0.5 16 ./test_file/sample_16x16.in > para_output_file/sample_16x16.out 
# python3 ./test_file/check.py 16 ./test_file/sample_16x16.out ./para_output_file/sample_16x16.out 
# ./rainFall_para 4 20 0.5 32 ./test_file/sample_32x32.in
# ./rainFall_para 4 30 0.25 128 ./test_file/sample_128x128.in
# ./rainFall_para 4 30 0.75 512 ./test_file/sample_512x512.in > para_output_file/sample_512x512.out 
# python3 ./test_file/check.py 512 ./test_file/sample_512x512.out ./para_output_file/sample_512x512.out 

echo "begin checing results"
FILE_NAME=( "sample_4x4.out " "sample_16x16.out" "sample_32x32.out" "sample_128x128.out" "sample_512x512.out" "sample_2048x2048.out" "measurement_4096x4096.out" )
DIMENSION=( "4" "16" "32" "128" "512" "2048" "4096" )
len=${#FILE_NAME[@]}  # ignore the last test

for (( i=0; i<$len; i++ ))
do
    echo " -----------checking result for dimension ${DIMENSION[$i]} ${FILE_NAME[$i]}-----------"
    python3 ${VALIDATION_FILE_PATH}/check.py ${DIMENSION[$i]} ${VALIDATION_FILE_PATH}/${FILE_NAME[$i]} ${OUTPUT_FILE_PATH}/${FILE_NAME[$i]}
    echo " "
done