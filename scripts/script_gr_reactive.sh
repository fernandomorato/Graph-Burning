for filename in instances/*; do
	echo $filename
	g++ -o executables/gr_reactive code/gr_reactive.cpp && ./executables/gr_reactive -seed 42 -alpha 0.5 -tl 60 -ip "$filename" -op output/output_gr_reactive.csv -lp logs/log_gr_reactive.log
done