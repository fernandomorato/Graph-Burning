for filename in ../parsed-datasets/*edges.txt ; do
	a=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	echo $a $b
done
return
make grasp
for filename in ../parsed-datasets/*edges.txt ; do
	k=$(basename $filename .txt)
	echo Rodando $k
	./grasp $k < "$filename" > ../parsed-datasets/$k.out
done

make a
for filename in ../parsed-datasets/*.out ; do
	k=$(basename $filename .out)
	./a $k < "$filename"
done
