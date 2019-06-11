#include <stdio.h>
#include "math.h"
#include "mpi.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int temp;
    int count = 0;
    int tag = 0;
    MPI_Status status;
    int x, n, y, i, portion, rem;
    float localSum = 0, totalSum = 0, *globalArr;
    float mean = 0, *localArr,localDiff=0,globalDiff,finalRes;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        printf("Enter n :");
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank != 0)
    {
        localArr = malloc(sizeof(float) * n);
        for (i = 0; i < n; i++)
        {
            localArr[i] = rand() % 100 + 1;
            localSum += localArr[i];
        }
    }
    MPI_Allreduce(&localSum, &totalSum, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    if (my_rank != 0)
    {
        mean = totalSum / (n * (p-1));

        for(i=0;i<n;i++){
            localDiff += pow(localArr[i]-mean,2);
        }
    }
    MPI_Reduce (&localDiff,&globalDiff,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
    if(my_rank==0){
        finalRes = globalDiff / (n*(p-1));
        finalRes= sqrt((double)finalRes);
        printf("Final Result is %f\n",finalRes);
    }
    MPI_Finalize();
}