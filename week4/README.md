# Week4
## How to run the code
To run all the comm_test_mpi.c programs you first complite the program with the following prompt:

    mpicc HPQC/week4/comm_test_mpi.c -o bin/comm_test_mpi

to run code use the following command, no inputs are required:

    mpirun -np 2 bin/comm_test_mpi

To run ping_pong.c compile as comm_test_mpi.c and to run the code use the following command, the input is the number of pings:

    mpirun -np 2 bin/ping_pong 100

To run ping_array.c it's the same as ping_pong.c with a second input for the length of the array:

    mpirun -np 2 bin/ping_array 100 2000

To run all the vector sum using different types of MPI comunication you can complile the code as done for comm_test_mpi.c, and the code takes one input to run which corrisponds to the lenght og the array:

    mpirun -np 2 bin/vector_gather 100 
 
## Code and Benchmarking
### Part 1
The order of the messages is random each time. For a larger number of processors the send messages seem to print before the receive messages and with 10 or more processors the send messages are all printing before the receive messages but not consistently.
**MPI_Ssend**: the receive messages tend to print before the send messages, the order of the send messages is random.


**MPI_Bsend**:  the messages are in random order with sending messages usually printing first

**MPI_Rsend**: the messages are in random order

errors messages: MPI_Isend it has more arguments than the previous send messages

The internal timing for receive and send operations is very inconsistent the send operations vary a lot with timings usually being near 0.00003s varying from a 0.000016s to 0.000055s. The receive messages get increasingly faster with the last few messages taking 0.000001s or even 0.000000s with the first iteration taking as long as 0.000370s using 16 processors. The more processors are used the longer the first iteration of the receive message is, this is not necessarily the case if one or two more processors are used.
### Part 2
The average time converges on 0.000001s with the total elapsed time continuously increasing as the number of pings increases.

The ping pong program works as follows:

The counter starts at zero with the root processor sending the counter to a client directory, the client directory then adds one to the counter and sends it back to the root processor. The process is then repeated until the counter reaches the inputted number of pings.

To measure the bandwidth and latency of the program data was taken for how long it takes for the program to run for various array sizes varying from  8B and 2MiB the data was then plotting providing the following graph:

![](https://github.com/slamlux/HPQC/blob/main/week4/bandwidth.png)

The linear fit for the data was the used to determine the slope which corresponds to the bandwidth fitted to be 9.02*10<sup>7</sup> bytes/s and an intercept which is the latency of -5.92 * 10<sup>4</sup>s with a an error of 5.24 * 10<sup>4</sup>s the latency measurement isn’t very accurate and thus doesn’t tell much about the latency of the program.
### Part 3
MPI_broadcast was the fastest for an input of 100 having a real time 0.349s with mpi gather and send/recv operations having a similar real time of respectively 0.436s and 0.437. MPI_broadcast was faster but not by much.

Mpi send/recv, gather and reduce have very similar real time for an input of 100 with mpi_reduce having the longest real time of 0.457s while send/recv and gather had real times of respectively 0.437s and 0.439s. All the real times are really close so there is no clear faster function.

The MPI_op_create function performs very similrly in speed to all the previous mmp comunicatorshaving a real time of 0.440s for an input of 100. In terms of accuracy, it gives an accurate result if the number of input is divisible by the number of processors. 
