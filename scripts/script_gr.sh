g++ -o executables/gr code/gr.cpp
for filename in instances/*; do
	# echo $filename
	./executables/gr -alpha 0.51 -tl 1800 -ip "$filename" -op output/important-info.csv -lp logs/important-log.log &
done