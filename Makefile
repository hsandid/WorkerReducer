all: WorkerReducer worker WorkerReducer_Optimized benchmark

cleanFolder:
	rm -f output*
	rm run worker temp.txt temp2.txt bench.txt benchmark bench.ps bench.pdf
WorkerReducer:
	gcc -o run WorkerReducer.c
worker:
	gcc -o worker worker.c

WorkerReducer_optimized:
	g++ WorkerReducer_Optimized.cpp -o run_opt -std=c++11 -pthread

benchmark:
	g++ benchmark.cpp -o benchmark
run_benchmark:
	./benchmark
	gnuplot plot
	ps2pdf bench.ps

