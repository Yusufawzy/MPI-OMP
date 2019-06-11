#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    int n, i;
    float *Vec1, *Vec2;
    float *localVec1, *localVec2;
    int portion, rem;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        printf("Enter the length of the vector\n");

        scanf("%d", &n);
        Vec1 = malloc(n * sizeof(float));
        Vec2 = malloc(n * sizeof(float));
        for (i = 0; i < n; i++)
        {
            Vec1[i] = i + 1;
            Vec2[i] = i + 1;
        }
        portion = n / p, rem = n % p;
    }
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
    localVec1 = malloc(sizeof(float)*portion);
    localVec2 = malloc(sizeof(float)*portion);
    MPI_Scatter(Vec1, portion, MPI_INT,
                localVec1, portion, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(Vec2, portion, MPI_INT,
                localVec2, portion, MPI_INT, 0, MPI_COMM_WORLD);

    //now all processes have vec1 initialized till its portion, except the root has the full arry
    //but we will just iterate in each one till the portionsize
    for (i = 0; i < portion; i++)
        localVec1[i] += localVec2[i];
    //Vec1 has the result, without using gather we want to combine the Vector again in one array
    if (my_rank == 0)
    {
        for (i = portion * p; i < portion * p + rem; i++)
        {
            Vec1[i] += Vec2[i];
        }
    }

    MPI_Gather(localVec1, portion, MPI_INT, Vec1, portion , MPI_INT, 0, MPI_COMM_WORLD);
    for (i = 0; i < n; i++)
    {
        printf("%f\n", Vec1[i]);
    }
    printf("\n");
    MPI_Finalize();
}