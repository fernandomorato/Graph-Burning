for filename in ../../Datasets/output/*.in.out ; do
	rm $filename
done
exit

make greedy
for filename in ../../Datasets/input/*in ; do
	a=$(basename $filename .in)
	echo Rodando $a
	./greedy < $filename > ../../Datasets/output/"${a#*/}".out
done
rm greedy
exit

seed=42
alpha=0.5
max_iter=1000
criterio_parada=0
funcao_gulosa=0
make reactive
for filename in ../../Datasets/input/*.in ; do
	a=$(basename $filename .in)
	echo Rodando $a
	# ./greedy $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/eigenvector_reactive/"${a#*/}".out

	# ./reactive < $filename > ../../Datasets/output/"${a#*/}".out
done
rm reactive