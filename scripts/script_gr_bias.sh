for filename in instances/*; do
	echo $filename
	g++ -o executables/gr_bias code/gr_bias.cpp && ./executables/gr_bias -seed 42 -alpha 0.5 -tl 600 -ip "$filename" -op output/output_gr_bias.csv -lp logs/log_gr_bias.log
done