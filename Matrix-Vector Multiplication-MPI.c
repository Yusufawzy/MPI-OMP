#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    float **mat1, *linear1, *vec1, *localRes, *globalRes;
    int row, col, i, j;
    float *localLinear1;
    int portion, rem;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (my_rank == 0)
    {
        printf("Enter the number of Rows: ");
        scanf("%d", &row);
        printf("Enter the number of Columns: ");
        scanf("%d", &col);
        mat1 = malloc(row * sizeof(float *));
        linear1 = malloc(row * col * sizeof(float));
        vec1 = malloc(col * sizeof(float));
        globalRes =  malloc(sizeof(float) * row);

        for (i = 0; i < row; i++)
        {
            mat1[i] = &linear1[col * i];
        }
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                mat1[i][j] = j+1;
                vec1[j] = j+1;
            }
        }
        //printing the matrix
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                printf("%f ", mat1[i][j]);
            }
            printf("\n");
        }
        //printing the vector
        printf("printing the vector\n");
        for (i = 0; i < col; i++)
        {
            printf("%f ", vec1[i]);
        }
        printf("\n");
        //portion is number of rows per slave
        portion = (row ) / p, rem = (row ) % p;
    }
    MPI_Bcast(&row, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&col, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);

    localLinear1 = malloc(sizeof(float) * portion*col);
    localRes = malloc(sizeof(float) * (portion));
    MPI_Scatter(linear1,  portion*col, MPI_FLOAT, localLinear1,  portion*col, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vec1, col, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    //localArr size is just portion that will be assigned to it
    for (i = 0; i < portion ; i++)
    {
        for (j = 0; j < col; j++)
        {
            localRes[i] += localLinear1[i * col + j] * vec1[j];
        }
    }
    MPI_Gather (localRes, portion,MPI_FLOAT,globalRes,portion,MPI_FLOAT,0,MPI_COMM_WORLD);
    if (my_rank ==0 ){
        //remainder part 
        for (i = portion*p; i < portion*p+rem ; i++)
            {
                for (j = 0; j < col; j++)
                {
                    globalRes[i] += linear1[i * col + j] * vec1[j];
                }
            }
        //printing final RESULT
        printf("Final results is \n");
        for (j = 0; j <  row; j++)
        {
            printf("%f\n",globalRes[j]);
        }
    }
    MPI_Finalize();
    return 0;
}
