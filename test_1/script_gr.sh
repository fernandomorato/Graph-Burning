g++ -o gr gr.cpp
for filename in *.in ; do
	
	n_processes=$(ps -U fcpereira | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr)
	done
	# a=$(basename $filename .in)
	echo "$filename"
	# > logs/logs_gr/log_"${filename#*/}" &
   	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/output_gr/$a.csv -lp logs/logs_gr/$a.log &
done
