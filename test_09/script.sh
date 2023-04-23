mkdir logs
mkdir output
mkdir solutions
g++ ../code/testes/gr-test.cpp -o gr
for filename in instances/*.in ; do
	n_processes=$(ps -U morato | grep -c gr)
	while [ $n_processes -ge 30 ]
	do
		sleep 10
		n_processes=$(ps -U morato | grep -c gr)
	done
	echo "$filename"
	for i in $(seq 1 3) ; do
   		./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results_0$i.csv -sp solutions/sol_"${filename#*/}" -criterio $i >> logs/log_"${filename#*/}" &
   	done
done
g++ ../code/ajeita.cpp -o ajeita
for i in $(seq 1 3) ; do
	./ajeita < output/results_0$i.csv > results_0$i_aux.csv
	cp results_0$i_aux.csv results_0$i.csv
	rm results_0$i_aux.csv
done
rm ajeita
# for filename in instances/* ; do
# 	n_processes=$(ps -U fcpereira | grep -c gr)
# 	while [ $n_processes -ge 30 ]
# 	do
# 		sleep 10
# 		n_processes=$(ps -U fcpereira | grep -c gr)
# 	done
# 	echo "$filename"
#    	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results.csv -criterio 2 > logs/log_"${filename#*/}" &
# done