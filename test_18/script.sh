mkdir logs
mkdir output
mkdir solutions
g++ ../code/gr.cpp -o gr
for filename in instances/*.in ; do
	for i in $(seq 0 6) ; do
	# n_processes=$(ps -U fcpereira | grep -c gr)
	# while [ $n_processes -ge 30 ]
	# do
	# 	sleep 10
	# 	n_processes=$(ps -U fcpereira | grep -c gr)
	# done
		echo "$filename fixando $i vertices"
		./gr -alpha 0.51 -seed 42 -tl 600 -ip "$filename" -op output/results.csv -sp solutions/sol_"$i_${filename#*/}" -fix $i -fsp solutions/fixed_solution.sol -stp stats/stats_"${i}".txt >> logs/log_$i_"${filename#*/}" &
	done
done
