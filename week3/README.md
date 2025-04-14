# Week 3

## How to run the code

compile hello_mpi.c :
 
	mpicc hello_mpi.c -o bin/hello_mpi

to run from the home directory: 
	
	mpirun -np (number of processors e.g. 4) bin/hello_mpi

compile hello_mpi_serial.c :

	gcc hello_mpi_serial.c -o bin/hello_mpi_serial

to run from the home directory:

	bin/hello_mpi_serial

compile vector_serial.c :

	gcc vector_serial.c -o bin/vector_serial

to run from the home directory:#

	bin/vector_serial

compile vector_serial_mpi.c :

	mpicc vector_serial_mpi.c -o bin/vector_serial_mpi

to run from the home directory:

	mpirun -np (number of processors e.g. 4) bin/vector_serial_mpi
## Benchmarking and tasks
### Part 1
The real time remains around 0.4 seconds with user and sys time steadily increasing while adding processors, the sum of the user and sys become more after than the real time after 7 processors.
The runtimes are much lower in the serial version with real times of 0.002 seconds.
### Part 2
Proof.c operation is sum over number of processors *input.
### Part 3
Serial code breakdown:

**Main():** initialises  a vector and subsequently makes the vector non-trivial, sums over all the elements of the vector and finally prints the sum. It has functions that measure the time at the start and end of the code to internally bench mark the code, the time that the loop takes is also printed.

**Sum_vector():** sums over all the elements of a vector.

**Initialise_vector():** fill all the elements of the vector with the same number.

**Non_trivial_vector():** substitutes all the elements of the vector following the formula i+2 where i is the position of the element starting at 0.

**Print_vector():** prints each element of the vector to the terminal.

**Check_args():** check input arguments from the terminal and stops the code if the right number of arguments aren’t called.

**Calculate_runtime():** calculates the runtime for an inputted start time and end time used for internal benchmarking.

**To_second_float():** returns the output of calculate_runtime() as a float.


Serial breaks after the input 1000362 and has a real time of 0.028s which much lower than the real time for 10 inputs of the parallel version with six processors which had  a real time of 0.719s so for this task the parallel version is always going to be slower. In this scenario 
