# seed=42
# nIteracoes=1000
# criterioParada=1
# funcaoGulosa=1
make parse
for pasta in $(ls -d */) ; do
	for filename in $pasta/*.mtx ; do
		echo PASTA $pasta
		k=$(basename $filename .mtx)
		b=$(head -n 1 "$filename")
		./parse < "$filename" > ../parsed-datasets/$(basename $filename .mtx)_edges.txt
	done
done