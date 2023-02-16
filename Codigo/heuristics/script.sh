seed=42
alpha=0.5
max_iter=1000
criterio_parada=0
funcao_gulosa=0
make greedy
for filename in ../../Datasets/input/*.in ; do
	a=$(basename $filename .wtf)
	echo Rodando $a
	# ./greedy $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/eigenvector_reactive/"${a#*/}".out
	./greedy < $filename > ../../Datasets/output/"${a#*/}".out
done
rm greedy