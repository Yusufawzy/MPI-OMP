#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    int globalArr[] = {1, 4, 5, 7, 0, -1, 1000, 34, 56, 0, 777777};
    int portion, rem, *localArr, localMax, finalRes;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (my_rank == 0)
    {
        int length = sizeof(globalArr) / sizeof(int);
        portion = length / p;
        
        rem = length % p;
        printf("remainder is %d\n",rem);
    }
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Scatter (arr ,portionSize,MPI_INT,localArr,portionSize,MPI_INT,0,MPI_COMM_WORLD );
    //MPI_Reduce (&localMax , &globalMax,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);

    localArr = malloc(portion * sizeof(int));
    MPI_Scatter(globalArr, portion, MPI_INT, localArr, portion, MPI_INT, 0, MPI_COMM_WORLD);
    int i = 0;
    localMax = 1e-9;
    for (; i < portion; i++)
    {
        if (localArr[i] > localMax)
        {
            localMax = localArr[i];
        }
    }
    //problem was that in the remainder I have to work on the global array
    if (my_rank == 0)
    {
        for (i = portion * p; i < portion * p + rem; i++)
        {
            if (globalArr[i] > localMax)
            {
                localMax = globalArr[i];
            }
        }
    }
    printf("local maximum from process %d is %d\n", my_rank, localMax);
    MPI_Reduce(&localMax, &finalRes, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        printf("global maximum  %d\n", finalRes);
    }
    MPI_Finalize();
    return 0;
}
