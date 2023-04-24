mkdir logs
mkdir output
mkdir solutions
g++ ../code/testes/gr-test-felipe.cpp -o gr
for filename in instances/*.in ; do
	n_processes=$(ps -U fcpereira | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U fcpereira | grep -c gr)
	done
	echo "$filename"
	# for i in $(seq 1 3) ; do
   		./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results_0$i.csv -sp solutions/sol_"${filename#*/}" -criterio 2 >> logs/log_"${filename#*/}" &
   	# done
done