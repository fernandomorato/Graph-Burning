g++ -o executables/gr_bias code/gr_bias.cpp
for filename in instances/*; do
	# echo $filename
	a=$(basename $filename .in)
	./executables/gr_bias -alpha 0.42 -tl 600 -ip "$filename" -op output/important-info-bias.csv -lp logs/log_gr_bias/$a.log &
done
