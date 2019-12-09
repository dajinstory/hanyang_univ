#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VECTORS 77
#define MATSIZE 3

void load_data(file,x,y,_x,_y)
FILE* file;
float** x;
float** y;
float** _x;
float** _y;
{
    int idx;
    (*x)=(float*)malloc(sizeof(float)*(VECTORS));
    (*y)=(float*)malloc(sizeof(float)*(VECTORS));
    (*_x)=(float*)malloc(sizeof(float)*(VECTORS));
    (*_y)=(float*)malloc(sizeof(float)*(VECTORS));
    for(idx=0;idx<VECTORS;idx++){
        fscanf(file, "%f", &(*x)[idx]);
        fscanf(file, "%f", &(*y)[idx]);
        fscanf(file, "%f", &(*_x)[idx]);
        fscanf(file, "%f", &(*_y)[idx]);
    }
}

void make_matrix(x,y,_x,_y,A,b1,b2)
float** x;
float** y;
float** _x;
float** _y;
float*** A;
float** b1;
float** b2;
{
    int idx, tmp, size_n, size_m, i, j;
    float Xs,Ys,XXs,YYs,XYs,_Xs,_Ys,X_Xs,X_Ys,Y_Xs,Y_Ys;

    (*A)=(float**)malloc(sizeof(float*)*(MATSIZE+1));
    (*b1)=(float*)malloc(sizeof(float)*(MATSIZE+1));
    (*b2)=(float*)malloc(sizeof(float)*(MATSIZE+1));
    for(idx=0;idx<=MATSIZE;idx++){
        (*A)[idx]=(float*)malloc(sizeof(float)*(MATSIZE+1));
    }

    Xs=Ys=XXs=YYs=XYs=_Xs=_Ys=X_Xs=X_Ys=Y_Xs=Y_Ys=0;
    for(idx=0;idx<VECTORS;idx++){
        Xs+=(*x)[idx];
        Ys+=(*y)[idx];
        XXs+=(*x)[idx]*(*x)[idx];
        YYs+=(*y)[idx]*(*y)[idx];
        XYs+=(*x)[idx]*(*y)[idx];
        
        _Xs+=(*_x)[idx];
        _Ys+=(*_y)[idx];
        X_Xs+=(*x)[idx]*(*_x)[idx];
        X_Ys+=(*x)[idx]*(*_y)[idx];
        Y_Xs+=(*y)[idx]*(*_x)[idx];
        Y_Ys+=(*y)[idx]*(*_y)[idx];
    }
    (*A)[1][1]=XXs;
    (*A)[1][2]=(*A)[2][1]=XYs;
    (*A)[1][3]=(*A)[3][1]=Xs;
    (*A)[2][2]=YYs;
    (*A)[2][3]=(*A)[3][2]=Ys;
    (*A)[3][3]=(float)VECTORS;

    (*b1)[1]=X_Xs;
    (*b1)[2]=Y_Xs;
    (*b1)[3]=_Xs;

    (*b2)[1]=X_Ys;
    (*b2)[2]=Y_Ys;
    (*b2)[3]=_Ys;
}

void free_data(A,b1,b2,x,y,_x,_y)
float** A;
float* b1;
float* b2;
float* x;
float* y;
float* _x;
float* _y;
{
    int i;
    for(i=0;i<=MATSIZE;i++){
        free(A[i]);
    }
    free(A);
    free(b1);
    free(b2);
    free(x);
    free(y);
    free(_x);
    free(_y);
}

void check_arr2D(arr)
float** arr;
{
	int i,j;
	for(i=1;i<=MATSIZE;i++){
        printf("\t");
		for(j=1;j<=MATSIZE;j++){
			printf("%.2f\t", arr[i][j]);
		}
		printf("\n");
	}
}

void check_arr1D(arr)
float* arr;
{
	int i;
    printf("\t");
	for(i=1;i<=MATSIZE;i++){
		printf("%.2f\t", arr[i]);
	}	
    printf("\n");
}