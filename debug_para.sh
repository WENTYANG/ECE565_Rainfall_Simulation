make clean
make parallel

for i in {1..10000}; do
echo $i
# ./rainfall_pt 4 10 0.25 4 ./test_file/sample_4x4.in
# ./rainfall_pt 8 20 0.5 16 ./test_file/sample_16x16.in > para_output_file/sample_16x16.out 
# python3 ./test_file/check.py 16 ./test_file/sample_16x16.out ./para_output_file/sample_16x16.out 
./rainfall_pt 2 20 0.5 32 ./test_file/sample_32x32.in > para_output_file/sample_32x32.out 
res=`python3 ./test_file/check.py 32 ./test_file/sample_32x32.out ./para_output_file/sample_32x32.out`
  echo $res
  if [[ "$res" != "Output matches all expected values." ]]; then
    break;
  fi
# ./rainfall_pt 4 30 0.25 128 ./test_file/sample_128x128.in
# ./rainfall_pt 4 30 0.75 512 ./test_file/sample_512x512.in > para_output_file/sample_512x512.out 
# python3 ./test_file/check.py 512 ./test_file/sample_512x512.out ./para_output_file/sample_512x512.out 
((i=i+1))
done


