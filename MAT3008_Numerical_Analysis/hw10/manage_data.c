#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_data(file,n,m,A,b)
FILE* file;
float*** A;
float** b;
int* n;
int* m;
{
    int idx, tmp, size_n, size_m, i, j;

    fscanf(file, "%d", n);    
    fscanf(file, "%d", m);    
    size_n=(*n);
    size_m=(*m);

    (*A)=(float**)malloc(sizeof(float*)*(size_n+1));
    (*b)=(float*)malloc(sizeof(float)*(size_n+1));
    for(idx=0;idx<=size_n;idx++){
        (*A)[idx]=(float*)malloc(sizeof(float)*(size_m+1));
    }

    for(i=1;i<=size_n;i++){
        for(j=1;j<=size_m;j++){
            fscanf(file, "%f", &(*A)[i][j]);
        }
    }
    for(i=1;i<=size_n;i++){
        fscanf(file, "%f", &(*b)[i]);
    }
}

void free_data(n,m,A,b)
float** A;
float* b;
int n,m;
{
    int i;
    for(i=0;i<=n;i++){
        free(A[i]);
    }
    free(A);
    free(b);
}

void check_arr2D(arr,n,m)
float** arr;
int n,m;
{
	int i,j;
	for(i=1;i<=n;i++){
        printf("\t");
		for(j=1;j<=m;j++){
			printf("%.2f\t", arr[i][j]);
		}
		printf("\n");
	}
}

void check_arr1D(arr,n)
float* arr;
int n;
{
	int i;
    printf("\t");
	for(i=1;i<=n;i++){
		printf("%.2f\t", arr[i]);
	}	
    printf("\n");
}