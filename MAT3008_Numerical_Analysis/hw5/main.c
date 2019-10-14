#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solutions.c"


int main(){
	int tc;
	for(tc=1;tc<=3;tc++){
		FILE* file;
		float** A;
		float* b;
		int n,m,i,j;		
		char* problem;
		printf("\n\n## Problem number %d\n",tc);
		
		// Load matrix data
		sprintf(problem, "data\\\\lineq%d.dat",tc);
		file = fopen(problem, "r");
		load_data(file, &n, &m, &A, &b);
		fclose(file);
		
		// Solve with gaussj
		printf("* Gauss-Jordan Elimination\n");
		solve_gaussj(n,m,A,b);

		// Solve with LU-Decomposition
		printf("* LU-Decomposition\n");
		solve_LU(n,m,A,b);

		// Solve with Singular Value Decomposition
		printf("* Singular Value Decomposition\n");
		solve_SVD(n,m,A,b);

		// Solve with mprove
		printf("* mprove with LU Decomposition\n");
		solve_mprove(n,m,A,b);
		
		// Get Inverse matrix
		printf("* Inverse Matrix\n");
		solve_inverse(n,m,A,b);

		free_data(n,m,A,b);
	}
	return 0;
}
