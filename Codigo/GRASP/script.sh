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
make grasp
k=$(basename "../../Datasets/parsed-datasets/ba_1k_2k_edges.txt" .txt)
b=$(head -n 1 "../../Datasets/parsed-datasets/ba_1k_2k_edges.txt")
echo Rodando $k
./grasp $k $b < "../../Datasets/parsed-datasets/ba_1k_2k_edges.txt" > ../../Datasets/parsed-datasets/$k.out

make combine
./combine $k < "../../Datasets/parsed-datasets/ba_1k_2k_edges.txt"

return

for filename in ../../Datasets/parsed-datasets/*edges.txt ; do
	k=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	echo Rodando $k
	./grasp $k $b < "$filename" > ../../Datasets/parsed-datasets/$k.out
done
make combine
for filename in ../../Datasets/parsed-datasets/*.out ; do
	k=$(basename $filename .out)
	./combine $k < "$filename"
done
