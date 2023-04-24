# make check
# ./check < instances/grid10.in > solutions/grid10_sol4_desc.txt
# exit
mkdir logs
mkdir output
mkdir solutions
# g++ gr-test.cpp -o gr
# i=0
g++ ../code/testes/gr-test.cpp -o gr
for i in $(seq 0 5) ; do
	for filename in instances/*.in ; do
		n_processes=$(ps -U morato | grep -c gr)
		while [ $n_processes -ge 6 ]
		do
			sleep 10
			n_processes=$(ps -U morato | grep -c gr)
		done
		echo "$filename"
	   	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results.csv -sp solutions/"${filename#*/}"_$i.sol -fixa $i -criterio 2 >> logs/log_"${filename#*/}" &
	   	# i=$((i+1))
	done
done