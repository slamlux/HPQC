#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>


// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
//time the function
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
void check_args(int argc, char **argv, int *points, int *cycles, int *samples);
void initialise_vector(double vector[], int size, double initial);
void initialise_x_pos(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);
//mpi
void check_processors(int uni_size, int points, int my_rank, double positions[], double x_pos[], double vx[], double vy[],double time_stamps[], int time_steps, int argc, char **argv);
void run_tasks(int uni_size, int points, int my_rank, double positions[], double nx_pos[], double vx[], double vy[], double time_stamps[], int time_steps, int argc, char **argv);
void main_task(int uni_size, int points, int my_rank, double positions[], double x_pos[], double vx[], double vy[], double time_stamps[], int argc, char **argv, int time_steps);
void client_task(int uni_size, int my_rank, double time, int points);



int main(int argc, char **argv)
{
	// declare and initialise error handling variable
	int ierror = 0;
	FILE* out_file;
     	out_file = fopen(argv[1],"w");
	fclose(out_file);
	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size  = 0;
	
	// declare and initialise the numerical argument variable
	//int points = check_args(argc, argv);
	int points, cycles, samples;
	check_args(argc, argv, &points, &cycles, &samples);

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);


	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);


	int time_steps = cycles * samples + 1; // total timesteps
	double step_size = 1.0/samples;

	// creates a vector for the time stamps in the data
	double* time_stamps = (double*) malloc(time_steps * sizeof(double));
	initialise_vector(time_stamps, time_steps, 0.0);
	generate_timestamps(time_stamps, time_steps, step_size);

	// creates a vector variable for the current positions
	double* positions = (double*) malloc(points * sizeof(double));
	// creates a vector variable for the current positions
	double* vx= (double*) malloc(points * sizeof(double));
	// creates a vector variable for the current positions
	double* vy = (double*) malloc(points * sizeof(double));
	double* x_pos = (double*) malloc(points * sizeof(double));


	// and initialises every element to zero
	initialise_vector(positions, points, 0.0);
	initialise_vector(vx, points, 0.0);
	initialise_vector(vy, points, 0.0);
	initialise_x_pos(x_pos, points, 0.0);
	int size = 0;

	check_processors(uni_size, points, my_rank, positions, x_pos, vx, vy, time_stamps, time_steps, argc, argv);

	// if we use malloc, must free when done!
	free(time_stamps);
	free(positions);

	// closes the file
	//fclose(out_file);
	
	// finalise MPI
	ierror = MPI_Finalize();

	return 0;
}

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
	fprintf(*p_out_file, "#, time");
	for (int j = 0; j < points; j++)
	{
		fprintf(*p_out_file, ", y[%d]", j);
	}
	fprintf(*p_out_file, "\n");
}

// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
	double value = sin(time*2.0*M_PI);
	return(value);
}

// defines a function to update the positions
void update_positions(double* positions, int points, double time)
{
	// creates a temporary vector variable for the new positions
        double* new_positions = (double*) malloc(points * sizeof(double));

	// initialises the index
	int i = 0;
	new_positions[i] = driver(time);
	// creates new positions by setting value of previous element 
	for (i = 1; i < points; i++)
	{
		new_positions[i] = positions[i-1];
	}
	// propagates these new positions to the old ones
	for (i = 0; i < points; i++)
        {
                positions[i] = new_positions[i];
        }

	// frees the temporary vector
	free(new_positions);
}

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
	for (int i = 0; i < time_steps ; i++)
	{
		timestamps[i]=i*step_size;
	}	
	return time_steps;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
	// creates a variable to hold the sum
	int sum = 0;

	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		sum += vector[i];
	}

	// returns the sum
	return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// sets the elements of the vector to the initial value
		vector[i] = initial;
	}
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_x_pos(double vector[], int size, double initial)
{
	double j = 0.0;
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		
		// sets the elements of the vector to the initial value
		vector[i] = j;
		j++;
	}
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
	// iterates through the vector
	for (int i = 0; i < size; i++)
	{
		// prints the elements of the vector to the screen
		printf("%d, %lf\n", i, vector[i]);
	}
}

// defines a function that checks your arguments to make sure they'll do what you need
void check_args(int argc, char **argv, int *points, int *cycles, int *samples)
{
	// declare and initialise the numerical argument
	//int num_arg = 0;

	// check the number of arguments
	if (argc == 5) // program name and numerical argument
	{

		*points = atoi(argv[2]);
		*cycles = atoi(argv[3]);
		*samples = atoi(argv[4]);

	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: %s [outfile] [points] [cycles] [samples]\n", argv[0]);

		// and exit COMPLETELY
		exit (-1);
	}
	//return num_arg;
}

//start of MPI

void main_task(int uni_size, int points, int my_rank, double positions[], double x_pos[], double vx[], double vy[], double time_stamps[], int argc, char **argv, int time_steps)
{


	// creates and initialies transmission variables
	int count, tag;
	tag =  0;
	count = 10;

	int chunk = points/(uni_size);
	//int start = chunk * (my_rank-1);

	// creates a vector for the time stamps in the data
	double* sub_positions_y = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_positions_y, chunk, 2.0);

	double* sub_positions_x = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_positions_x, chunk, 2.0);

	double* sub_velocity_y = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_velocity_y, chunk, 2.0);

	double* sub_velocity_x = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_velocity_y, chunk, 2.0);
        double t = time_stamps[1];
	double k=0.1;
	double m=0.06;	
	double l = 1.0;
	MPI_Status status;

	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);



	MPI_Bcast(positions, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(x_pos, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(vy, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(vx, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);


	sub_positions_y[0] = driver(time_stamps[time_steps]);
	// creates new positions by setting value of previous element
	for (int i = 1; i < chunk; i++)
	{
		
		double dy1 = positions[i] - positions[i-1];
		double dy2 = positions[i+1] - positions[i];
		double dx1 = x_pos[i] - x_pos[i-1];
		double dx2 = x_pos[i+1] - x_pos[i];
		double dl1 = sqrt(dx1*dx1 + dy1*dy1) - l;
		double dl2 = sqrt(dx2*dx2 + dy2*dy2) - l;
	
		double angle1 = atan(abs(dy1/dx1));
		double angle2 = atan(abs(dy2/dx2));
		double ax = (cos(angle1)*(k*dl1) + cos(angle2)*(-k*dl2))/m;
		double ay = (sin(angle1)*(k*dl1) + sin(angle2)*(-k*dl2))/m;
		sub_velocity_x[i] = ax*t+vx[i];
		sub_velocity_y[i] = ay*t+vy[i];
		sub_positions_y[i] = 0.5* ay*t*t + vx[i]*t + positions[i-1];
		sub_positions_x[i] = 0.5* ax*t*t + vy[i]*t + x_pos[i];

		//printf("Received st %f \n", try);
	}

	//gather
	MPI_Gather(sub_positions_y, chunk, MPI_DOUBLE, positions, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_positions_x, chunk, MPI_DOUBLE, x_pos, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_velocity_y, chunk, MPI_DOUBLE, vy, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_velocity_x, chunk, MPI_DOUBLE, vx, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// iterates through each time step in the collection	
	// creates a file
	FILE* out_file;
	FILE* out_filex;
     	out_filex = fopen("data/xpos.csv","a");
     	out_file = fopen(argv[1],"a");

	// prints an index and time stamp
	fprintf(out_file, "%d, %lf", time_steps, time_stamps[time_steps]);
	fprintf(out_filex, "%d, %lf", time_steps, time_stamps[time_steps]);
	// iterates over all of the points on the line
	for (int j = 0; j < points; j++)
	{
		// prints each y-position to a file
		double print = positions[j];
		double p = x_pos[j];
		//printf("Gathering %f \n", print);
		fprintf(out_file, ", %lf", print);
		fprintf(out_filex, ", %lf", p);
	}
	// prints a new line
	fprintf(out_file, "\n");
	fprintf(out_filex, "\n");
	// closes the file
		
	
	// if we use malloc, must free when done!
	//free(time_stamps);
	//free(positions);

	// closes the file
	fclose(out_file);
	fclose(out_filex);



	timespec_get(&end_time, TIME_UTC);
	//print_vector(recv_message, size);
	//printf("\n");
	//int counts = recv_message[0];
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);


	// prints the message from the sender
	//printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, counts, source);
	//printf("Rank %d took %f to receive\n",rank,runtime);
	//free(sub_positions);
	//free(recv_message);



 // end for (int their_rank = 1; their_rank < uni_size; their_rank++)

}

void client_task(int uni_size, int my_rank, double time, int points)
{

	// creates and initialies transmission variables
	int dest, tag, source;
	dest = tag = source = 0;
	MPI_Status status;
	int chunk = points/(uni_size);
	int start = chunk * (my_rank);
	double t = 0.04;
	double k=0.1;
	double m=0.06;	
	double l = 1.0;


	// creates a vector for the time stamps in the data
	double* sub_positions_y = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_positions_y, chunk, 0.0);

	double* sub_positions_x = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_positions_x, chunk, 0.0);

	double* sub_velocity_y = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_velocity_y, chunk, 0.0);

	double* sub_velocity_x = (double*) malloc(chunk* sizeof(double));
	initialise_vector(sub_velocity_y, chunk, 0.0);

	// creates a vector variable for the current positions
	double* positions = (double*) malloc(points * sizeof(double));
	// creates a vector variable for the current positions
	double* vx= (double*) malloc(points * sizeof(double));
	// creates a vector variable for the current positions
	double* vy = (double*) malloc(points * sizeof(double));
	double* x_pos = (double*) malloc(points * sizeof(double));


	// and initialises every element to zero
	initialise_vector(positions, points, 0.0);
	initialise_vector(vx, points, 0.0);
	initialise_vector(vy, points, 0.0);
	initialise_x_pos(x_pos, points, 0.0);


	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);


	//broadcast
	MPI_Bcast(positions, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(x_pos, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(vy, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(vx, points, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// creates new positions by setting value of previous element 
	for (int i = 0; i < chunk; i++)
	{
		double dy1 = positions[start+i] - positions[start+i-1];
		double dy2 = positions[start + i+1] - positions[start+i];
		double dx1 = x_pos[start+i] - x_pos[start+i-1];
		double dx2 = x_pos[start+i+1] - x_pos[start+i];
		double dl1 = sqrt(dx1*dx1 + dy1*dy1) - l;
		double dl2 = sqrt(dx2*dx2 + dy2*dy2) - l;
	
		double angle1 = atan(abs(dy1/dx1));
		double angle2 = atan(abs(dy2/dx2));
		double ax = (cos(angle1)*(k*dl1) + cos(angle2)*(-k*dl2))/m;
		double ay = (sin(angle1)*(k*dl1) + sin(angle2)*(-k*dl2))/m;
		sub_velocity_x[i] = ax*t+vx[i];
		sub_velocity_y[i] = ay*t+vy[i];
		sub_positions_y[i] = 0.5* ay*t*t+ vx[start+i]*t + positions[start+i-1];
		sub_positions_x[i] = 0.5* ax*t*t + vy[start+i]*t + x_pos[start+i];

		//printf("Received st %f \n", try);
	}

	//printf("%f at time %f \n", some, time);
	// sets the destination for the message
	dest = 0; // destination is root
	
	//gather
	MPI_Gather(sub_positions_y, chunk, MPI_DOUBLE, positions, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_positions_x, chunk, MPI_DOUBLE, x_pos, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_velocity_y, chunk, MPI_DOUBLE, vy, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(sub_velocity_x, chunk, MPI_DOUBLE, vx, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);
	
	//int counts = send_message1[0];

	// prints the message from the sender
        //printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                             //my_rank, uni_size, counts, dest);
	//printf("Rank %d took %f to send\n",my_rank,runtime);

	free(sub_positions_y);
	free(sub_positions_x);
	free(sub_velocity_y);
	free(sub_velocity_x);
	free(positions);
	free(x_pos);
	free(vx);
	free(vy);


}


void run_tasks(int uni_size, int points, int my_rank, double positions[], double x_pos[], double vx[], double vy[], double time_stamps[], int time_steps, int argc, char **argv)
{
	int counters = 0;
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	// get time before the sum 
	timespec_get(&start_time, TIME_UTC);
	if (0 == my_rank)
	{
		FILE* out_file;
		FILE* out_filex;
     		out_file = fopen(argv[1],"a");
		out_filex = fopen("data/xpos.csv","a");
		print_header(&out_file, points);
		print_header(&out_filex, points);
		fclose(out_file);
		fclose(out_filex);
	}

	for (int i = 0; i < time_steps; i++)
	{
		double time = time_stamps[i];
		if (0 == my_rank)
		{
			// function that receives all the messages
			main_task(uni_size, points, my_rank, positions, x_pos, vx, vy, time_stamps, argc, argv, i);

		} // end if (0 == my_rank)
		else // i.e. (0 != my_rank)
		{
			
			client_task(uni_size, my_rank, time, points);

		} // end else // i.e. (0 != my_rank)
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("counter = %d\n", counter);
	}
	
	timespec_get(&end_time, TIME_UTC);
	
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);
	//double avg_time = runtime/num_pings;


}


void check_processors(int uni_size, int points, int my_rank, double positions[], double x_pos[], double vx[], double vy[], double time_stamps[], int time_steps, int argc, char **argv)
{
	//double count, runtime, avg_time;
	if (uni_size > 1)
	{
		run_tasks(uni_size, points, my_rank, positions, x_pos, vx, vy, time_stamps, time_steps, argc, argv);
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


