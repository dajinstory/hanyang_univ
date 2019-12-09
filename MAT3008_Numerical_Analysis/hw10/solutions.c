#include "lib\gaussj.c"
#include "lib\lubksb.c"
#include "lib\ludcmp.c"
#include "lib\mprove.c"
#include "lib\svbksb.c"
#include "lib\svdcmp.c"
#include "manage_data.c"

#define ABS(X) (X < 0 ? -X : X)
#define EPSILON 0.000001

void solve_inverse(row,col,A,b)
float** A;
float* b;
int row,col;
{
    // set temporary variables
    int i,j;
    float** mat_A = (float**)malloc(sizeof(float*)*(row+1));
    float** mat_B = (float**)malloc(sizeof(float*)*(row+1));
    for(i = 0; i <= row; i++){
        mat_A[i] = (float*)malloc(sizeof(float)*(col+1));
        mat_B[i] = (float*)malloc(sizeof(float)*(col+2));
        for(j=1;j<=col;j++){
            mat_B[i][j]=mat_A[i][j]=A[i][j];
        }
    }


    for(i = 1; i <= col; i++)
        mat_B[i][1] = b[i];

    // solve
    gaussj(mat_A, row, mat_B, col);

    // inverse
    check_arr2D(mat_A, row, col);

    // free variables
    for(i = 0; i <= row; i++){
        free(mat_A[i]);
        free(mat_B[i]);
    }
    free(mat_A);
    free(mat_B);
}

void solve_gaussj(row,col,A,b)
float **A, *b;
int row,col;
{
    // set temporary variables
    int i,j;
    float** mat_A = (float**)malloc(sizeof(float*)*(row+1));
    float** mat_B = (float**)malloc(sizeof(float*)*(row+1));
    for(i = 0; i <= row; i++){
        mat_A[i] = (float*)malloc(sizeof(float)*(col+1));
        mat_B[i] = (float*)malloc(sizeof(float)*(col+2));
        for(j=1;j<=col;j++){
            mat_B[i][j]=mat_A[i][j]=A[i][j];
        }
    }


    for(i = 1; i <= col; i++)
        mat_B[i][1] = b[i];

    // solve
    gaussj(mat_A, row, mat_B, col);

    // check results
    printf("\t");
    for(i=1;i<=col;i++) printf("%.2f ",mat_B[i][1]);
    printf("\n");

    // free variables
    for(i = 0; i <= row; i++){
        free(mat_A[i]);
        free(mat_B[i]);
    }
    free(mat_A);
    free(mat_B);
}

void solve_LU(row,col,A,b)
float **A, *b;
int row,col;
{
    // set variables
    int i,j;
    float d;
    float** mat_A = (float**)malloc(sizeof(float*)*(row+1));
    float* mat_b = (float*)malloc(sizeof(float)*(col+1));
    int* idx = (int*)malloc(sizeof(int)*(row+1));
    for(i = 0; i <= row; i++){
        mat_A[i] = (float*)malloc(sizeof(float)*(col+1));
        for(j=1;j<=col;j++){
            mat_A[i][j]=A[i][j];
        }
    }
    for(i = 1; i <= col; i++)
        mat_b[i] = b[i];

    // solve
    ludcmp(mat_A, row, idx, &d);
    lubksb(mat_A, row, idx, mat_b);

    // check results
    check_arr1D(mat_b,row);

    // free variables
    for(i = 0; i <= row; i++){
        free(mat_A[i]);
    }
    free(mat_A);
    free(mat_b);
    free(idx);
}
void solve_SVD(row,col,A,b)
float **A, *b;
int row,col;
{
    // set variables
    int i,j;
    float d;
    float** mat_A = (float**)malloc(sizeof(float*)*(row+1));
    float* mat_b = (float*)malloc(sizeof(float)*(col+1));
    float* w = (float*)malloc(sizeof(float)*(row+1));
    float** V = (float**)malloc(sizeof(float*)*(row+1));
    float* x = (float*)malloc(sizeof(float)*(row+1));
    for(i = 0; i <= row; i++){
        mat_A[i] = (float*)malloc(sizeof(float)*(col+1));
        for(j=1;j<=col;j++){
            mat_A[i][j]=A[i][j];
        }
    }
    for(i = 1; i <= col; i++)
        mat_b[i] = b[i];
    for(int i = 0; i <= row; i++)
        V[i] = (float*)malloc(sizeof(float)*(col+1));        

    // solve
    svdcmp(mat_A, row, col, w, V);
    svbksb(mat_A, w, V, row, col, mat_b, x);
    check_arr1D(x, row);

    // free variables
    for(i = 0; i <= row; i++){
        free(mat_A[i]);
        free(V[i]);
    }
    free(mat_A);
    free(V);
    free(mat_b);
    free(w);
    free(x);
}

void solve_mprove(row,col,A,b)
float **A, *b;
int row,col;
{
    int i,j;
    float d;
    float** mat_A = (float**)malloc(sizeof(float*)*(row+1));
    float* mat_b = (float*)malloc(sizeof(float)*(col+1));
    int* idx = (int*)malloc(sizeof(int)*(row+1));

    for(i = 0; i <= row; i++){
        mat_A[i] = (float*)malloc(sizeof(float)*(col+1));
        for(j=1;j<=col;j++){
            mat_A[i][j]=A[i][j];
        }
    }
    for(i = 1; i <= col; i++)
        mat_b[i] = b[i];

    ludcmp(mat_A, row, idx, &d);
    lubksb(mat_A, row, idx, mat_b);
    mprove(A, mat_A, row, idx, b, mat_b);
    check_arr1D(mat_b, row);
    
    // free variables
    for(i = 0; i <= row; i++){
        free(mat_A[i]);
    }
    free(mat_A);
    free(mat_b);
    free(idx);
}    