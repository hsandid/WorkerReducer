# Worker-Reducer program

This project has been realized in the scope of the course CMPS 272 "Operating Systems", at the American University of Beirut. The Worker-Reducer program splits a text file into N fragments, and counts the occurrences of the strings "ECE", "CCE", and "CMPS" separately

Two versions of the program are available : 

- *C version* : Takes an integer N as an argument. This version spawns N worker processes (child and 3 reducer processes to handle file input. This solution is considered non-efficient.

- *C++ version* : Takes an integer N as an argument. This version spawns N threads (with *lseek* to read file properly in each thread) and 3 reducer processes to handle file input and do the job. This solution is considered efficient.

A benchmark process is also included in the source files. It runs both the optimized and unoptimized Worker-Reducer processes, and compares their runtime using *gnuplot* and *ps2pdf*.

### Makefile arguments :

- *all* : Compile the C and C++ versions of WorkerReducer, and the benchmark program.

- *cleanFolder* : Remove all generated files from previous compilations.

- *run_benchmark* : Execute the benchmark program and generate performance plot.