mkdir output
mkdir logs
g++ ../code/gr.cpp -o gr
for filename in *.in; do
	echo $filename
	a=$(basename $filename .in)
	./gr -alpha 0.51 -tl 600 -ip "$filename" -op output/all-info.csv -lp logs/$a.log &
done
for filename in *.in; do
	echo $filename
	a=$(basename $filename .in)
	./gr -alpha 0.25 -tl 600 -ip "$filename" -op output/all-info.csv -lp logs/$a.log &
done