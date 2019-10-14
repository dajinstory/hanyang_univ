#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_n(file,n)
FILE* file;
int* n;
{
	int tmp;
    printf("load n...\n");
    fscanf(file, "%d", &tmp);    
    fscanf(file, "%d", &tmp);
    *n=tmp;
}

void load_matrix2D(file,mat,n)
FILE* file;
int n;
float** mat;
{
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            fscanf(file, "%f", &mat[i][j]);
        }
    }
}

void load_matrix1D(file,mat,n)
FILE* file;
int n;
float* mat;
{
	int i;
    for(i=0;i<n;i++){
        fscanf(file, "%f", &mat[i]);
    }
}

void load_data(file,mat2D,mat1D,n)
FILE* file;
float*** mat2D;
float** mat1D;
int* n;
{
    int idx,size;
    load_n(file,n);
    size=*n;
    (*mat1D)=(float*)malloc(sizeof(float)*size);
    (*mat2D)=(float**)malloc(sizeof(float*)*size);

    for(idx=0;idx<size;idx++){
        (*mat2D)[idx]=(float*)malloc(sizeof(float)*size);
    }

    load_matrix2D(file, *mat2D, *n);
    load_matrix1D(file, *mat1D, *n);

}
