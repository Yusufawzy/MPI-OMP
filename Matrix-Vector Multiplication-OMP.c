#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
//Name: Yusuf Fawzy Elnady                  ID:20160299
int main(int argc, char *argv[])
{

    int row, col, vRow,i,j;
    while (1)
    {
        printf("Enter number of rows of the matrix\n");
        scanf("%d", &row);
        printf("Enter number of columns of the matrix\n");
        scanf("%d", &col);
        printf("Enter number of rows of the vector, No of Columns is 1\n");
        scanf("%d", &vRow);
        if (vRow != col)
            printf("Invalid indices numbers, please enter the numbers again\n");
        else break;
    }
    float *mat = malloc(sizeof(float) * row * col);
    float *vec = malloc(sizeof(float) * vRow);
    float *res = malloc(sizeof(float) * row);

    printf("Enter Matrix elements\n");
    for(i=0;i<row;i++)for(j=0;j<col;j++) scanf("%f",&mat[i*col+j]);
    printf("Enter Vector elements\n");
    for(j=0;j<vRow;j++) scanf("%f",&vec[j]);

    #pragma omp parallel private(i, j) shared(res,col,row,vec,mat)
    {
        #pragma omp for schedule(dynamic)
            for (i = 0; i < row ; i++)
            {
                for (j = 0; j < col; j++)
                {
                    res[i] += mat[i * col + j] * vec[j];
                }
            }
    }
    printf("Result of MUltiplication:\n");
    for(i=0;i<row;i++) printf("%f\n",res[i]);
}
