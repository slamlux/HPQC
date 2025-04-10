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
## jsjsjjsjs
