for filename in instances/*; do
	echo $filename
	g++ -o executables/gr_reactive_bias code/gr_reactive_bias.cpp && ./executables/gr_reactive_bias -seed 42 -alpha 0.5 -tl 600 -ip "$filename" -op output/output_gr_reactive_bias.csv -lp logs/log_gr_reactive_bias.log -ap alpha_results.csv
done
