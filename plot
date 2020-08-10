set terminal postscript 14 dashed color
set output "bench.ps"
set xlabel "Number of Splits"
set ylabel "Execution Times"
plot "bench.txt" using 1:2 lt rgb "black"  with linespoint title "Not optimized","bench.txt"using 1:3 lt rgb "red" with linespoint title "Optimized"
