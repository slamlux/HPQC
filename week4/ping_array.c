#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void main_task(int uni_size, int source, int rank, int counter, int num_pings, long vector[], int size);
void client_task(int uni_size, int my_rank, int num_pings, int size);
void run_tasks(int uni_size, int my_rank, int num_pings, int counter, long vector[], int size);
void check_processors(int uni_size, int my_rank, int num_pings, int counter, long vector[], int size);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
void check_args(int argc, char **argv, int *num_pings, long *size);
void initialise_vector(long vector[], int size, int initial);
void print_vector(int vector[], int size);


int main(int argc, char **argv) 
{
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise rank and size varibles
	int my_rank, uni_size, counter;
	my_rank = uni_size = counter = 0;
	
	// number of pings from input
	int num_pings;
	long size;
	check_args(argc, argv, &num_pings, &size);
	
	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);


	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

	long* my_vector = malloc (size * sizeof(long));
	// and initialises every element to zero
	initialise_vector(my_vector, size, 1);
	my_vector[0]=0;
	double mem = sizeof(int);
	// checks if the input num of processors and if that is larger than 1 then runs the task
	check_processors(uni_size, my_rank, num_pings, counter, my_vector, size);
	//printf("memory for 1 int:%f\n", mem);
	// finalise MPI
	ierror = MPI_Finalize();
	free(my_vector);
	return 0;
}

void main_task(int uni_size, int source, int rank, int counter, int num_pings, long vector[], int size)
{


	// creates and initialies transmission variables
	int count, tag;
	tag =  0;
	count = 10;

	long* send_message = malloc (size * sizeof(long));
	initialise_vector(send_message, size, 0);

	long* recv_message = malloc (size * sizeof(long));
	initialise_vector(recv_message, size, 0);


	send_message = vector;
	MPI_Status status;
        
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);
	if (send_message[0] <= num_pings)
	{

		//send_message = counter;
		MPI_Send(send_message, size, MPI_LONG, 1, tag, MPI_COMM_WORLD);

		// receives the messages
		MPI_Recv(recv_message, size, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
	}
	timespec_get(&end_time, TIME_UTC);
	//print_vector(recv_message, size);
	//printf("\n");
	int counts = recv_message[0];
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);


	// prints the message from the sender
	//printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, counts, source);
	//printf("Rank %d took %f to receive\n",rank,runtime);

	//free(send_message);
	//free(recv_message);



 // end for (int their_rank = 1; their_rank < uni_size; their_rank++)

}

void client_task(int uni_size, int my_rank, int num_pings, int size)
{

	// creates and initialies transmission variables
	int dest, tag;
	dest = tag = 0;
	MPI_Status status;
	
	long* send_message1 = malloc (size * sizeof(long));
	initialise_vector(send_message1, size, 0);

	
	long* recv_message1 = malloc (size * sizeof(long));
	initialise_vector(recv_message1, size, 0);


	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);
	

	MPI_Recv(recv_message1, size, MPI_LONG, 0, tag, MPI_COMM_WORLD, &status);

	// sets the destination for the message
	dest = 0; // destination is root

	// creates the message
	long count = recv_message1[0];
	send_message1[0] = count + 1;
	
	//print_vector(send_message1, size);
	//printf("\n");
	// sends the message
	if (send_message1[0] <= num_pings)
	{
		MPI_Send(send_message1, size, MPI_LONG, dest, tag, MPI_COMM_WORLD);
	}

	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);
	
	int counts = send_message1[0];

	// prints the message from the sender
        //printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                             //my_rank, uni_size, counts, dest);
	//printf("Rank %d took %f to send\n",my_rank,runtime);

	//free(send_message1);
	//free(recv_message1);


}


void run_tasks(int uni_size, int my_rank, int num_pings, int counter, long vector[], int size)
{
	int counters = 0;
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);

	while (counter < num_pings)
	{
		if (0 == my_rank)
		{
			// iterates through all the other ranks
			for (int their_rank = 1; their_rank < uni_size; their_rank++)
			{
				// function that receives all the messages
				main_task(uni_size, their_rank, my_rank, counter, num_pings, vector, size);			
		
			} // end for (int their_rank = 1; their_rank < uni_size; their_rank++)
		} // end if (0 == my_rank)
		else // i.e. (0 != my_rank)
		{
			
			client_task(uni_size, my_rank, num_pings, size);

		} // end else // i.e. (0 != my_rank)
		counter++;
		vector[0]=counter;
		//printf("counter = %d\n", counter);
	}
	
	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);
	double avg_time = runtime/num_pings;
	if (0 == my_rank)
	{
		printf("The counter is at %d, with a total elapsed time of %lf and an average of %lf\n", counter, runtime, avg_time);
	}


}


void check_processors(int uni_size, int my_rank, int num_pings, int counter, long vector[], int size)
{
	//double count, runtime, avg_time;
	if (uni_size == 2)
	{

		run_tasks(uni_size, my_rank, num_pings, counter, vector, size);
		//printf("The counter is at %f, with a total elapsed time of %lf and an average of %lf\n", count, runtime, avg_time);

	} // end if (uni_size > 1)
	else // i.e. uni_size <=1
	{
		if (0 == my_rank)
		{

			// prints a warning
			printf("You can only use 2 processors for this program. MPI communicator size = %d\n", uni_size);
		}
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

// defines a function that checks your arguments to make sure they'll do what you need
void check_args(int argc, char **argv, int *num_pings, long *size)
{
	// declare and initialise the numerical argument
	//int num_arg = 0;

	// check the number of arguments
	if (argc == 3) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		*num_pings = atoi(argv[1]);
		*size = atoi(argv[2]);

	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	//return num_arg;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(long vector[], int size, int initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}


// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d, ", vector[i]);
	}
}

