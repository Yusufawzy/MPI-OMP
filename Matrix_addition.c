#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    int  i, j;
    float **mat1, **mat2;
    int row = 4, col = 4;
    float *linear2, *linear1;
    float *locallinear1, *locallinear2;
    int portion, rem;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        linear1 = malloc(row * col * sizeof(float));
        linear2 = malloc(row * col * sizeof(float));
        mat1 = malloc(row * sizeof(float *));
        mat2 = malloc(row * sizeof(float *));

        for (i = 0; i < row; i++)
        {
            mat1[i] = &linear1[col * i];
            mat2[i] = &linear2[col * i];
        }
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                mat1[i][j] = j;
                mat2[i][j] = j;
            }
        }
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                printf("%f ", mat1[i][j]);
            }
            printf("\n");
        }
        portion = (row*col) / p, rem =  (row*col) % p;
    }
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
    locallinear1 = malloc(sizeof(float) * portion);
    locallinear2 = malloc(sizeof(float) * portion);
    MPI_Scatter(linear1, portion, MPI_INT,
                locallinear1, portion, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(linear2, portion, MPI_INT,
                locallinear2, portion, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < portion; i++)
        locallinear1[i] += locallinear2[i];
    MPI_Gather(locallinear1, portion, MPI_INT, linear1, portion, MPI_INT, 0, MPI_COMM_WORLD);
    printf("\n");

    if (my_rank==0){
        for (i = portion * p; i < portion * p + rem; i++)
            linear1[i] += linear2[i];
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                printf("%f ", mat1[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
}