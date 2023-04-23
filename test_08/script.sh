for filename in instances_synthetic/* ; do
	
	n_processes=$(ps -U fcpereira | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr)
	done
	echo "$filename"
   	./gr -ip "$filename" > logs/log_"${filename#*/}" &

done