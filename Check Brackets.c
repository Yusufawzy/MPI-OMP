#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int my_rank;
    int p;
    int tag = 0;
    MPI_Status status;
    int i, portion;
    char *s;
    char *sLocal;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int *localArr, *globalArr;
    if (my_rank == 0)
    {
        //Try to uncomment one of them
        s = "{{[[{}])((}}[][]][)]";
        // s = "{{[[{}})((}}[][]][)]";
        int len = strlen(s);
        //As it will be divisible so no need to handle the remainder part
        portion = len / p;
        printf("char length is %d\n", strlen(s));
        //kol wa7ed at2fal laz ytft7 so I have ( { [ ] } ) so the array will be gathered will be
        globalArr = malloc(sizeof(int) * 6);
    }
    localArr = malloc(sizeof(int) * 6); //as 6 is [ -> open , ] -> closed and so on
    MPI_Bcast(&portion, 1, MPI_INT, 0, MPI_COMM_WORLD);
    sLocal = malloc(sizeof(char) * portion);
    MPI_Scatter(s, portion, MPI_CHAR, sLocal, portion, MPI_CHAR, 0, MPI_COMM_WORLD);
    for(i = 0 ; i < 6 ; i++){
        localArr[i] = 0;
    }
    for (i = 0; i < portion; i++)
    {
        if (sLocal[i] == '[')
        {
            localArr[0]++;
        }
        else if (sLocal[i] == ']')
        {
            localArr[1]++;
        }
        else if (sLocal[i] == '{')
        {
            localArr[2]++;
        }
        else if (sLocal[i] == '}')
        {
            localArr[3]++;
        }
        else if (sLocal[i] == '(')
        {
            localArr[4]++;
        }
        else if (sLocal[i] == ')')
        {
            localArr[5]++;
        }
    }
    MPI_Reduce(localArr, globalArr, 6, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        for(i = 0 ; i < 6 ; i++){
            printf("%d \n", globalArr[i]);
        }
        if (globalArr[0] == globalArr[1] && globalArr[2] == globalArr[3] && globalArr[4] == globalArr[5])
        {
            printf("The string is balanced\n");
        }
        else
        {
            printf("The string isn't balanced\n");
        }
    }
    MPI_Finalize();
}
