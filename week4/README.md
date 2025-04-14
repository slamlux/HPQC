# Week4
## Code and Benchmarking
### Part 1
The order of the messages is random each time. For a larger number of processors the send messages seem to print before the receive messages and with 10 or more processors the send messages are all printing before the receive messages but not consistently.
**MPI_Ssend**: the receive messages tend to print before the send messages, the order of the send messages is random.


**MPI_Bsend**:  the messages are in random order with sending messages usually printing first

**MPI_Rsend**: 

**MPI_Isend**: it has more arguments than the previous send messages

The internal timing for receive and send operations is very inconsistent the send operations vary a lot with timings usually being near 0.00003s varying from a 0.000016s to 0.000055s. The receive messages get increasingly faster with the last few messages taking 0.000001s or even 0.000000s with the first iteration taking as long as 0.000370s using 16 processors. The more processors are used the longer the first iteration of the receive message is, this is not necessarily the case if one or two more processors are used.
### Part 2
The average time converges on 0.000001s with the total elapsed time continuously increasing as the number of pings increases.

The ping pong program works as follows:

The counter starts at zero with the root processor sending the counter to a client directory, the client directory then adds one to the counter and sends it back to the root processor. The process is then repeated until the counter reaches the inputted number of pings.
