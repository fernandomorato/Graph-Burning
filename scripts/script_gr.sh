for filename in ../instances/*; do
	echo $filename
	g++ -o gr gr.cpp && ./gr -seed 42 -alpha 0.5 -tl 5 -ip "$filename" -op ../output/output_gr.csv
	# g++ -o gr gr.cpp && ./gr -seed 42 -alpha 0.5 -tl 600 -i "$filename" -o ../output/output_gr.csv >> ../logs/log_gr.log
done