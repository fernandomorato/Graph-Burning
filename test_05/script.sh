g++ ../code/gr.cpp -o gr
# ./gr -alpha 0.51 -tl 300 -ip grid10.in -op seila.csv -lp ans-grid10.log
# g++ cnt10.cpp -o cnt10
# ./cnt10 < cntvalues.out
# exit
# for i in $(seq 1 10); do
# 	./gr -alpha 0.51 -tl 10 -ip grid$i.in -op seila.csv -lp ans-grid$i.log
# done
# exit
for filename in *.in; do
	echo $filename
	a=$(basename $filename .in)
	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/grids.csv -lp logs/$a.log &
done
