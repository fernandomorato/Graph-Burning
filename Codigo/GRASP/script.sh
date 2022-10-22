# filename="../../Datasets/input/c-fat500-10_edges.txt"
seed=42
# alpha=0.7
max_iter=1000
criterio_parada=0
funcao_gulosa=0

make grasp
for filename in ../../Datasets/input/*.txt ; do
	a=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	echo Rodando $a
	./grasp $filename $seed 0.3 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_03/"${a#*/}".log
	./grasp $filename $seed 0.5 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_05/"${a#*/}".log
	./grasp $filename $seed 0.7 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_07/"${a#*/}".log
done
rm grasp
# exit