#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
//time the function
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
//for vectors
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size, int start);
void non_trivial_vector(int vector[], int size);
void subdivision_of_vector(int vector[], int vector2[], int start, int stop);
// for mpi
int root_task(int uni_size, int num_arg, int my_vector[]);
void client_task(int my_rank, int num_arg, int uni_size, int my_vector[]);
void check_uni_size(int uni_size);
void check_task(int uni_size, int my_rank, int num_arg, int vector[]);



int main(int argc, char **argv)
{
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise the numerical argument variable
	int num_arg = check_args(argc, argv);
	
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);
	
	// checks the universe size is correct
	check_uni_size(uni_size);

	// creates a vector variable
	// int my_vector[num_arg]; // suffers issues for large vectors
	int* my_vector = malloc (num_arg * sizeof(int));
	// and initialises every element to zero
	initialise_vector(my_vector, num_arg, 0);


	// TODO: put some code here that makes a more meaningful vector
	non_trivial_vector(my_vector, num_arg);


	// checks what task to do and does it
	check_task(uni_size, my_rank, num_arg, my_vector);

	// get time at the end of the sum
        //timespec_get(&end_time, TIME_UTC);

	// calculates the runtime
	//time_diff = calculate_runtime(start_time, end_time);
	//runtime = to_second_float(time_diff);


	// outputs the runtime
	//printf("\n\nRuntime for core loop: %lf seconds.\n\n", runtime);

	//print_vector(my_vector, num_arg);
	// if we use malloc, must free when done!


	// finalise MPI
	ierror = MPI_Finalize();
	free(my_vector);
	return 0;

}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size, int start)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		int init = start + i;
		// sets the elements of the vector to the initial value
		sum += *(vector + init);
	}

	// returns the sum
	return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(int vector[], int size, int initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}

// defines a function to initialise all values in a vector to a given inital value
void non_trivial_vector(int vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = i+2;
	}
}

void subdivision_of_vector(int vector[], int vector2[], int start, int stop)
{
	int* var;
	// iterates through the vector
	for (int i = 0; i < stop; i++)
	{
		var = &vector[start + i];
		// sets the elements of the vector to the initial value
		vector2[i] = *var;
	}
}



// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d\n", vector[i]);
	}
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
	// declare and initialise the numerical argument
	int num_arg = 0;

	// check the number of arguments
	if (argc == 2) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		num_arg = atoi(argv[1]);
	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	return num_arg;
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

//start mpi
int root_task(int uni_size, int num_arg, int my_vector[])
{

	// creates and initialies transmission variables
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;

	// sets the destination for the message
	int dest = 0; // destination is root
	int chunk, start, stop;
	chunk = start = stop = 0;
	chunk = num_arg/(uni_size);

	int* sub_vector = malloc (chunk * sizeof(int));
	initialise_vector(sub_vector, chunk, 0);
	//scatter
	MPI_Scatter(my_vector, chunk, MPI_INT, sub_vector, chunk, MPI_INT, 0, MPI_COMM_WORLD);

	// creates and intiialises the variable for the final output
	int output_sum = sum_vector(sub_vector, chunk, start);

	
	// iterates through all the other ranks
	for (source = 1; source < uni_size; source++)
	{
		// receives the messages
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		//printf('&d/n', source);
		// adds the values to a running tally
		output_sum += recv_message;
	} // end for (source = 1; source < uni_size; source++)

	// outputs and returns the result
	printf("The combined result is %d\n", output_sum);
	return output_sum;
}

void client_task(int my_rank, int num_arg, int uni_size, int my_vector[])
{
	// creates and initialies transmission variables
	int send_message, count, dest, tag, source;
	send_message = dest = tag = source = 0;
	count = 1;
	MPI_Status status;
	int chunk = num_arg/(uni_size);
	int start = 0;
	//int* recv_message = malloc (chunk * sizeof(int));
	// sets the destination for the message
	dest = 0; // destination is root
	
	int* sub_vector = malloc (chunk * sizeof(int));
	initialise_vector(sub_vector, chunk, 0);
	//scatter
	MPI_Scatter(my_vector, chunk, MPI_INT, sub_vector, chunk, MPI_INT, 0, MPI_COMM_WORLD);


	//MPI_Recv(recv_message, num_arg, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

	// sums the vector
	int my_sum = sum_vector(sub_vector, chunk, start);
	//printf("%d\n", my_sum);
	
	// creates the message
	send_message = my_sum;
	//free(recv);

	// sends the message
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	
	return;
}


void check_uni_size(int uni_size)
{
	// sets the minimum universe size
	int min_uni_size = 1;
	// checks there are sufficient tasks to communicate with
	if (uni_size >= min_uni_size)
	{
		return;
	} // end if (uni_size >= min_uni_size)
	else // i.e. uni_size < min_uni_size
	{
		// Raise an error
		fprintf(stderr, "Unable to communicate with fewer than %d processes.", min_uni_size);
		fprintf(stderr, "MPI communicator size = %d\n", uni_size);

		// and exit COMPLETELY
		exit(-1);
	}
}

void check_task(int uni_size, int my_rank, int num_arg, int vector[])
{
	// checks which process is running and calls the appropriate task
	if (0 == my_rank)
	{
		root_task(uni_size, num_arg, vector);
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
		client_task(my_rank, num_arg, uni_size, vector);
	} // end else // i.e. (0 != my_rank)
}

