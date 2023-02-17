filename=instances/deezer_hr.in
g++ -o executables/gr code/gr.cpp && ./executables/gr -seed 42 -alpha 0.5 -tl 5 -ip "$filename" -op output/output_gr.csv -lp logs/log_gr.log