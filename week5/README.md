## Part 1
Hard coding was removed so to compile the c code :

    mpicc HPQC/week5/string_wave_new.c -o bin/string_wave_new -lm

to run the code the iputs are path for output csv file, points, cycles, samples:

    mpirun -np 2 bin/string_wave_new data/wave2.csv 50 5 25

for the animate line python code the inputs are path to input csv file, and name of output file (the output file will be outputted to data/selected_name as a gif)

    python3 HPQC/week5/animate_line.py data/wave2.csv rope_animation

## Part 2
The strategy to parallelise the code is to use mpi_broadcast so that each processor has access to all the position array, with each processor only performing tasks on 
a specific chunk of the array with the root processor performing the driver operation. After the task was performed each subsection is then gathered at the root 
processor using MPI_gather. The root processor then proceeds to print the position array to the output file. Then the process is repeated with the updated positions.

The result for an input like: mpirun -np 2 bin/string_wave_new data/wave2.csv 50 5 25 is

![](https://github.com/slamlux/HPQC/new/main/week5/sample_animation.gif)
