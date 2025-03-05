#include <stdio.h>
#include <mpi.h>
#include <time.h>

void main_task(int uni_size, int source, int rank);
void client_task(int uni_size, int my_rank);
void run_tasks(int uni_size, int my_rank);
void check_processors(int uni_size, int my_rank);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);


int main(int argc, char **argv) 
{
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

	
	// checks if the input num of processors and if that is larger than 1 then runs the task
	check_processors(uni_size, my_rank);
	
	// finalise MPI
	ierror = MPI_Finalize();
	return 0;
}

void main_task(int uni_size, int source, int rank)
{


	// creates and initialies transmission variables
	int recv_message, count, tag;
	recv_message = tag = 0;
	count = 1;
	MPI_Status status;
        
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);

	// receives the messages
	MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	// prints the message from the sender
	printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, recv_message, source);
	printf("Rank %d took %f to receive\n",rank,runtime);

	//return 0;

 // end for (int their_rank = 1; their_rank < uni_size; their_rank++)

}

void client_task(int uni_size, int my_rank)
{

	// creates and initialies transmission variables
	int send_message, count, dest, tag;
	send_message = dest = tag = 0;
	count = 1;
	MPI_Status status;

	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);


	// sets the destination for the message
	dest = 0; // destination is root

	// creates the message
	send_message = my_rank * 10;

	// sends the message
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);

	// prints the message from the sender
        printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                             my_rank, uni_size, send_message, dest);
	printf("Rank %d took %f to send\n",my_rank,runtime);


}


void run_tasks(int uni_size, int my_rank)
{

	if (0 == my_rank)
	{
		// iterates through all the other ranks
		for (int their_rank = 1; their_rank < uni_size; their_rank++)
		{
			// function that receives all the messages
			main_task(uni_size, their_rank, my_rank);			
		
		} // end for (int their_rank = 1; their_rank < uni_size; their_rank++)
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
			
		client_task(uni_size, my_rank);

	} // end else // i.e. (0 != my_rank)

}


void check_processors(int uni_size, int my_rank)
{

	if (uni_size > 1)
	{

		run_tasks(uni_size, my_rank);

	} // end if (uni_size > 1)
	else // i.e. uni_size <=1
	{
		// prints a warning
		printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
	}


}

double to_second_float(struct timespec in_time)
{
	// creates and initialises the variables
	float out_time = 0.0;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;

	// extracts the elements from in_time
	seconds = in_time.tv_sec;
	nanoseconds = in_time.tv_nsec;

	// calculates the time in seconds by adding the seconds and the nanoseconds divided by 1e9
	out_time = seconds + nanoseconds/1e9;

	// returns the time as a double
	return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
	// creates and initialises the variables
	struct timespec time_diff;
	long int seconds, nanoseconds;                                                                                                       seconds = nanoseconds = 0;
	double runtime = 0.0;

	// extracts the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// if the ns part is negative
	if (nanoseconds < 0)
	{
		// "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// creates the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}
