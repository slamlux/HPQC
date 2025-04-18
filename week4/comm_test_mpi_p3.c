#include <stdio.h>
#include <mpi.h>

void main_task(int uni_size, int source, int rank);
void client_task(int uni_size, int my_rank);
void run_tasks(int uni_size, int my_rank);
void check_processors(int uni_size, int my_rank);

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
        
	// receives the messages
	MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

	// prints the message from the sender
	printf("Hello, I am %d of %d. Received %d from Rank %d\n", rank, uni_size, recv_message, source);
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
	

	// sets the destination for the message
	dest = 0; // destination is root

	// creates the message
	send_message = my_rank * 10;

	// sends the message
	MPI_Ssend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// prints the message from the sender
        printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                             my_rank, uni_size, send_message, dest);


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
