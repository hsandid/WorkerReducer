all: C_WorkerReducer worker CPP_WorkerReducer benchmark

cleanFolder:
	rm -f output*
	rm run worker temp.txt temp2.txt bench.txt benchmark bench.ps bench.pdf
C_WorkerReducer:
	gcc -o run c-version/C_WorkerReducer.c
worker:
	gcc -o worker c-version/worker.c

CPP_WorkerReducer:
	g++ cpp-version/CPP_WorkerReducer.cpp -o run_opt -std=c++11 -pthread

benchmark:
	g++ benchmark.cpp -o benchmark
run_benchmark:
	./benchmark
	gnuplot plot
	ps2pdf bench.ps

