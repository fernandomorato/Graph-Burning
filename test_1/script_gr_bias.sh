# g++ -o executables/gr_bias code/gr_bias.cpp
for filename in *.in; do
	# echo $filename
	a=$(basename $filename .in)
	./gr_bias -alpha 0.42 -tl 600 -ip "$filename" -op output/output_gr_bias/$a.csv -lp logs/logs_gr_bias/$a.log &
done
