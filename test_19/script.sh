mkdir logs
mkdir output
mkdir solutions
g++ ../code/gr.cpp -O2 -o gr
for filename in intances/*.in ; do
	n_processes=$(ps -U fcpereira | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr)
	done
	./gr -alpha 0.51 -seed 42 -tl 600 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}" -fix 0 >> logs/log_"${filename#*/}" &
done
