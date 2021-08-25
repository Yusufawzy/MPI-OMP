#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char * argv[])
{
	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
	int source;		/* rank of sender	*/
	int dest;		/* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	char message[100];	/* storage for message	*/
	MPI_Status status;	/* return status for 	*/
				/* recieve		*/

	/* Start up MPI */
	MPI_Init(&argc, &argv);


	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (my_rank == 0)
	{
		/* create message */
		int dest = 1;
		int val = 100000;
		for (;dest<p;dest++){
		//	sprintf(message, "%d", dest);
			MPI_Send(&val,  1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		}
		/* use Strlen+1 to transmit /0  */
		
	}
	else
	{
			int rec;
			MPI_Recv(&rec, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			printf("%d, al7amdullah weslt\n", rec);
	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
