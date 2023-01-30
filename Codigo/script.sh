make gera_eigenvalues
for filename in ../Datasets/input/*.txt ; do
	a=$(basename $filename .txt)
	echo Rodando $a
	./gera_eigenvalues < $filename > ../Datasets/input/"${a#*/}".wtf
done
rm gera_eigenvalues