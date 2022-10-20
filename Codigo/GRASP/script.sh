make grasp
for filename in ../../Datasets/parsed-datasets/m*edges.txt ; do
	a=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	./grasp $filename 42 0.5 1000 0 0
	echo $a $b
done
rm grasp
exit
# make gen
# ./gen > random.in
# make grasp
# ./grasp $(basename random .in) $(head -n 1 random.in) < random.in > random.out
# return
# seed=42
# nIteracoes=1000
# criterioParada=1
# funcaoGulosa=1

make grasp
for filename in ../../Datasets/parsed-datasets/c-*edges.txt ; do
	k=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	echo Rodando $k
	./grasp $k $b < "$filename" > ../../Datasets/parsed-datasets/$k.out
done

make combine
for filename in ../../Datasets/parsed-datasets/c-*.out ; do
	k=$(basename $filename .out)
	./combine $k < "$filename"
done
