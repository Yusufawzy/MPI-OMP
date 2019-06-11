#include <stdio.h>
#include <string.h>
#include "mpi.h"
int isprime(int n)
{
    int x = 0;
    for (x = 2; x < n; x++)
    {
        if (n % x == 0)
            return 0;
    }
    return 1;
}
int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int temp;
    int count = 0;
    int tag = 0;
    MPI_Status status;
    int x, y, i, portion,rem;
    int *localArr, *globalArr;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        printf("Enter the lower bound:");
        scanf("%d", &x);
        printf("Enter the upper bound:");
        scanf("%d", &y);
        y++;
        portion = (y - x) / p;
        rem = (y - x) % p;
        printf("portion is %d\n",portion);
        for (i = 1; i < p; i++)
        {
             MPI_Send(&portion, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
             MPI_Send(&x, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
             MPI_Send(&y, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        globalArr = malloc(sizeof(int) * (y - x));
    }
    else
    {
        MPI_Recv(&portion, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&y, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    }
    localArr = malloc(sizeof(int) * portion + 1);
    for (i = my_rank * portion + x; i < my_rank * portion + x + portion; i++)
    {
        if (isprime(i))
        {
        printf("Process %d found a prime number %d\n", my_rank, i);
            localArr[count++] = i;
        }
    }
    MPI_Gather(localArr, portion, MPI_INT, globalArr, portion, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        for(i=portion*p+x;i<portion*p+rem+x;i++){
        
        //Here we've to add remainder of the parts to the rest of the globalArr
        if(isprime(i)){
        printf("Process %d found a prime number %d\n", my_rank, i);
            globalArr[i]=i;
            count++;
        }
        }
        for (i = 1; i < p; i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            count += temp;
        }
        printf("Total number of prime numbers are %d\n", count);
        for (i = 0; i <= (y - x); i++)
        {
            if (globalArr[i] <= (y - x) && globalArr[i])
                printf("%d, ", globalArr[i]);
        }
        printf("\n");
    }
    else
    {
        MPI_Send(&count, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}