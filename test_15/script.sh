mkdir logs
mkdir output
mkdir solutions
g++ ../code/gr.cpp -o gr
for i in 1 2 ; do
	for filename in instances/*.in ; do
		n_processes=$(ps -U fcpereira | grep -c gr)
		while [ $n_processes -ge 30 ]
		do
			sleep 10
			n_processes=$(ps -U fcpereira | grep -c gr)
		done
		echo "$filename"
		./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}" -criterio 2 -centralidade $i >> logs/log_"${filename#*/}" &
	done
done
