#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib\gaussj.c"
#include "lib\lubksb.c"
#include "lib\ludcmp.c"
#include "lib\mprove.c"
#include "lib\nrutil.c"
#include "lib\svbksb.c"
#include "lib\svdcmp.c"
#include "lib\load_data.c"

#define ABS(X) (X < 0 ? -X : X)
#define EPSILON 0.000001

void check_arr(float** arr, int n){
	int i,j=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%f ", arr[i][j]);
		}
		printf("\n");
	}
}

int main(){


	FILE* file;
	float** mat2D, **mat2D2;
	float* mat1D, **mat1D2;
	int n,i,j;
	
	file = fopen("data\\lineq1.dat", "r");
	load_data(file, &mat2D, &mat1D, &n);
	fclose(file);
	file = fopen("data\\lineq1.dat", "r");
	load_data(file, &mat2D2, &mat1D2, &n);
	fclose(file);
	check_arr(mat2D, n);
	gaussj(mat2D,n,mat2D2,1);
	check_arr(mat2D, n);
	return 0;
}
