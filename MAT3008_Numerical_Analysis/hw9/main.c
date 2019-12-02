#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solutions.c"


int main(){
	int tc;
	for(tc=1;tc<=3;tc++){
		FILE* file;
		float* x;
		float* y;
		float* _x;
		float* _y;
		
		float** A;
		float* b1;
		float* b2;
		int i,j;		
		char* problem;
		printf("\n\n## Problem number %d\n",tc);
		
		// Load matrix data
		sprintf(problem, "data\\\\fitdata%d.dat",tc);
		file = fopen(problem, "r");
		load_data(file, &x, &y, &_x, &_y);
		fclose(file);
		make_matrix(&x,&y,&_x,&_y,&A,&b1,&b2);
		// Get Inverse matrix
		solve_inverse(A,b1);
		solve_inverse(A,b2);

		free_data(A,b1,b2,x,y,_x,_y);
	}
	return 0;
}
