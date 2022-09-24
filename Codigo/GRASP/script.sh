# for filename in ../../Datasets/parsed-datasets/*edges.txt ; do
# 	a=$(basename $filename .txt)
# 	b=$(head -n 1 "$filename")
# 	echo $a $b
# done

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
