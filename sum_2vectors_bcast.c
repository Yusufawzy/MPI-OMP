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
            Vec1[i] = i+1;
            Vec2[i] = i+1;
        }
        // printf("Enter the elements of the first vector\n");
        // for(i=0;i<n;i++){
        //     scanf("%f",&Vec1[i]);
        // }
        // printf("Enter the elements of the second vector\n");
        // for(i=0;i<n;i++){
        //     scanf("%f",&Vec2[i]);
        // }
        printf("Second vector\n");
        for (i = 0; i < n; i++)
        {
            printf("%f  ", Vec2[i]);
        }
        printf("\n");

        portion = n / p, rem = n % p;
    }
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
    //now all processes have vec1 initialized till its portion, except the root has the full arry
    //but we will just iterate in each one till the portionsize
    for (i = 0; i < portion; i++)
        Vec1[i] += Vec2[i];
    //Vec1 has the result, without using gather we want to combine the Vector again in one array
    if (my_rank==0){
        for (i = portion*p;i<portion*p+rem;i++){
            Vec1[i] += Vec2[i];
        }
        for(i = 1;i<p;i++){
            //recieve from specific one then add its corresponding part to the whole arr
            MPI_Recv(&Vec1[i * portion], portion,MPI_INT,i,0,MPI_COMM_WORLD,&status);
            printf("I received from process %d\n",i);
        }
        for (i = 0; i < n; i++)
        {
            printf("%f\n", Vec1[i]);
        }
    }
    else {
        //problem was that in sending I have only my arr so send it 3la b3do
        MPI_Send(Vec1, portion, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    printf("\n");
    MPI_Finalize();
}   