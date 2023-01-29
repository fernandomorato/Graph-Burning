make teste
for filename in ../../Datasets/input/*.txt ; do
	echo Rodando $(basename $filename .txt)
	./teste < $filename
done
rm teste
exit
make get_answers
for filename in ../../Datasets/logs/eigenvector_centrality/logs_03/* ; do
	a=$(basename $filename .log)
	echo Rodando $a
	./get_answers $a < $filename
done
for filename in ../../Datasets/logs/eigenvector_centrality/logs_05/* ; do
	a=$(basename $filename .log)
	echo Rodando $a
	./get_answers $a < $filename
done
for filename in ../../Datasets/logs/eigenvector_centrality/logs_07/* ; do
	a=$(basename $filename .out)
	echo Rodando $a
	./get_answers $a < $filename
done
rm get_answers
gjhghjghjgj

exit
# filename="../../Datasets/input/c-fat500-10_edges.txt"
seed=42
alpha=0.7
max_iter=1000
criterio_parada=0
funcao_gulosa=0

make grasp
mkdir ../../Datasets/logs/eigenvector_centrality/logs_03
mkdir ../../Datasets/logs/eigenvector_centrality/logs_05
for filename in ../../Datasets/input/*.txt ; do
	a=$(basename $filename .txt)
	b=$(head -n 1 "$filename")
	echo Rodando $a
	./grasp $filename $seed 0.3 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/eigenvector_centrality/logs_03/"${a#*/}".log
	./grasp $filename $seed 0.5 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/eigenvector_centrality/logs_05/"${a#*/}".log
	# ./grasp $filename $seed 0.3 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_03/"${a#*/}".log
	# ./grasp $filename $seed 0.5 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_05/"${a#*/}".log
	# ./grasp $filename $seed 0.7 $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/logs/logs_07/"${a#*/}".log
done
rm grasp
# exit