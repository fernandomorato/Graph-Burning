g++ -o executables/gr code/gr.cpp
for filename in instances/*; do
	# echo $filename
	./executables/gr -alpha 0.51 -tl 600 -ip "$filename" -op output/out_gr.csv -lp logs/log_gr.log &
done