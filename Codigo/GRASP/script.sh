filename="../../Datasets/input/c-fat500-10_edges.txt"
seed=42
alpha=0.7
max_iter=1000
criterio_parada=0
funcao_gulosa=0

make grasp
# for filename in ../../Datasets/input/ree*edges.txt ; do
	a=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	./grasp $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa
	echo $a $b
# done
rm grasp
# exit