for filename in *.in; do
	echo $filename
	a=$(basename $filename .in)
	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/output_gr/all-info.csv -lp logs/logs_gr/$a.log &
done