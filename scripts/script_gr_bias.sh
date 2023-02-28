g++ -o executables/gr_bias code/gr_bias.cpp
for filename in instances/*; do
	# echo $filename
	./executables/gr_bias -alpha 0.42 -tl 600 -ip "$filename" -op output/out_gr_bias.csv -lp logs/log_gr_bias.log &
done
