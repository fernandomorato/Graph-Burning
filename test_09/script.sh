for filename in instances/* ; do
	n_processes=$(ps -U fcpereira | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr)
	done
	echo "$filename"
   	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results.csv -criterio 2 > logs/log_"${filename#*/}" &
done