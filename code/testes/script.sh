make f=gr-test
filename=instances/grid16.in
a=$(basename $filename .in)
# qtd_fixados=1
alpha=0.51

echo gr-test -alpha $alpha -tl 1 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_maluco -seed 1638723292556
./gr-test -alpha $alpha -tl 1 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_maluco -seed 1638723292556 >> logs/log_maluco

exit

for i in $(seq 0 7) ; do
	echo $i
	# echo Rodando com ALPHA = "$alpha" e fixando "$i" vertices inicialmente
	./gr-test-felipe -alpha $alpha -tl 1 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_1 -seed 1638723292556 -criterio 2 -centralidade 1 -fixa $i >> logs/log_"$a"_fixa_"$i"
done
# ./gr -alpha 0.7 -tl 1 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_1 -criterio 2 -centralidade 1 -fixa 0 >> logs/log_fixa_0
# ./gr -alpha 0.7 -tl 1 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_1 -criterio 2 -centralidade 1 -fixa 1 >> logs/log_fixa_1
# ./gr -alpha 0.51 -tl 60 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}"_2 -criterio 2 -centralidade 2 -fixa 0 >> logs/log_"$a"_2
exit
# make check
# ./check < instances/grid10.in > solutions/grid10_sol4_desc.txt
# exit
# mkdir logs
# mkdir output
# mkdir solutions
g++ gr-test-felipe.cpp -o gr
for filename in instances/*.in ; do
	n_processes=$(ps -U morato | grep -c gr)
	for i in $(seq 4 6) ; do
		while [ $n_processes -ge 3 ]
		do
			sleep 10
			n_processes=$(ps -U morato | grep -c gr)
		done
		echo "$filename"
		a=$(basename $filename .in)
	   	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/results.csv -sp solutions/sol_"${filename#*/}" -criterio 2 -fixa $i >> logs/log_$a_$i &
   	done
done
