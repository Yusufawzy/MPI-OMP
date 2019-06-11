#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char * argv[])
{
	int my_rank;		
	int p;			
	int source;		
	int dest;		
	int tag = 0;
    int x ;		
    int final = 1;
	char message[100];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (my_rank==0){
        x =1;
    }
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    x=my_rank+1;
    MPI_Reduce(&x,&final,1,MPI_INT,MPI_PROD,0,MPI_COMM_WORLD);
    if (my_rank==0){
        printf("factorial of %d is %d\n",p,final);
    }
	MPI_Finalize();
	return 0;
}
