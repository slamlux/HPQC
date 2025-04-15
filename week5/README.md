# Week 5
## How to run the code
Hard coding was removed so to compile the c code :

    mpicc HPQC/week5/string_wave_MPI.c -o bin/string_wave_MPI -lm

to run the code the iputs are path for output csv file, points, cycles, samples:

    mpirun -np 2 bin/string_wave_MPI data/wave2.csv 50 5 25

for the animate line python code the inputs are path to input csv file, and name of output file (the output file will be outputted to data/selected_name as a gif)

    python3 HPQC/week5/animate_line.py data/wave2.csv rope_animation

**string_wave_new.c** can be compiled and run as above with **string_wave_MPI.c** it takes the same inputs but it output an extra csv file for the x position
this file is hard coded into the program. the x position print out to data/xpos.csv.

to animate the data generated from **string_wave_new.c** you should use **new_animate_line.py** to run this python code use:

        python3 HPQC/week5/new_animate_line.py data/wave2.csv rope_animation

the inputs are the same as **animate_line.py** as the x position data is coded so that it uses the file data/xpos.csv the outputs from **string_wave_new.c**.
## Discussion
### Part 2
The strategy to parallelise the code is to use mpi_broadcast so that each processor has access to all the position array, with each processor only performing tasks on 
a specific chunk of the array with the root processor performing the driver operation. After the task was performed each subsection is then gathered at the root 
processor using MPI_gather. The root processor then proceeds to print the position array to the output file. Then the process is repeated with the updated positions.

The result for an input like: mpirun -np 2 bin/string_wave_MPI data/wave2.csv 50 5 25 is

![](https://github.com/slamlux/HPQC/blob/main/week5/sample_animation.gif)

add if the code is better then cereal

### Part 3
Springs between the points were added to make the simulation more realistic. The driver function is still implemented and travels through the rope, with the addition of small interaction between the neighbour points simulated through springs. 

Velocity vectors in the x and y direction as well as x position vector were added to be able to correctly implemnt the code.

The spring force F=-k*x makes the points accelerate following Newton's second law F=m*a the acceleration was assumed to be constant at each time step leading to the following equations for position an velocity at each time step:

x(t),y(t)=1/2*a<sub>x,y </sub>*t<sup>2 </sup> + v<sub>0 x,y </sub>*t+ x<sub>0 </sub>,y<sub>0 </sub>

V<sub>x,y </sub>(t)=a<sub>x,y </sub>*t++ v<sub>0 x,y </sub>

The result using the same inputs as part 2 with spring constant k =0.5 N/m and mass m=0.06 kg: mpirun -np 2 bin/string_wave_new data/wave2.csv 50 5 25

![](https://github.com/slamlux/HPQC/blob/main/week5/rope_with_springs.gif)


From the animations you can see the points are slightly oscillating due to the spring  model that was applied
