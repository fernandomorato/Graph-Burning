seed=42
alpha=0.5
max_iter=1000
criterio_parada=0
funcao_gulosa=0
# make degree
# make degree_bias
# make degree_reactive
# make eigenvector
# make eigenvector_bias
make eigenvector_reactive
for filename in ../../Datasets/input/*.wtf ; do
	a=$(basename $filename .wtf)
	echo Rodando $a
	# ./degree $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/degree/"${a#*/}".out
	# ./degree_bias $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/degree_bias/"${a#*/}".out
	# ./degree_reactive $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/degree_reactive/"${a#*/}".out
	# ./eigenvector $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/eigenvector/"${a#*/}".out
	# ./eigenvector_bias $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/eigenvector_bias/"${a#*/}".out
	./eigenvector_reactive $filename $seed $alpha $max_iter $criterio_parada $funcao_gulosa > ../../Datasets/output/eigenvector_reactive/"${a#*/}".out
done
# rm degree
# rm degree_bias
# rm degree_reactive
# rm eigenvector
# rm eigenvector_bias
rm eigenvector_reactive