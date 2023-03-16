g++ -o gr_bias gr_bias.cpp
for filename in *.in ; do
	
	n_processes=$(ps -U fcpereira | grep -c gr_bias)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr_bias)
	done
	# a=$(basename $filename .in)
	echo "$filename"
	# > logs/logs_gr/log_"${filename#*/}" &
   	./gr_bias -alpha 0.51 -tl 600 -ip "$filename" -op output/output_gr_bias/$a.csv -lp logs/logs_gr_bias/$a.log &
done
