#include <stdio.h>
#include <string.h>

int main(int argc , char * argv[])
{
    
    int *linear ; 
    int **mat;
    int n , m ;
    printf("Rows:");scanf("%d",&n);
    printf("Columns:");scanf("%d",&m);
    printf("Number of rows is %d and cols is %d\n",n,m);
    linear = malloc(n*m*sizeof(int));
    //We also need to initialize mat, thu we initialize it below directly 
    mat = malloc(n*sizeof(int));
    memset(linear,-1,sizeof(int)*n*m);
    int i =0,j ;
    //Here we go from 0 to number of rows, but each time we skip by step of cols (m)
    
    for (i=0;i<n;i++){
        mat[i]=&linear[i*m];
    }
    for (i=0;i<n;i++){
        for (j=0;j<m;j++){
            printf("%d ",mat[i][j]);
        }
    printf("\n");  
    }
    free(mat);
    free(mat[0]);
    return 0;

}

