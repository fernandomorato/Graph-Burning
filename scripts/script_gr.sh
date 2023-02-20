g++ -o executables/gr code/gr.cpp
for filename in instances/*; do
	# echo $filename
	 ./executables/gr -seed 42 -alpha 0.51 -tl 600 -ip "$filename" -op output/output_gr.csv -lp logs/log_gr.log &
done