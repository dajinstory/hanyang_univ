#include <stdio.h>
#include <string.h>
#include "matrix_methods.c"
#define ABS(X) (X < 0 ? -X : X)
#define EPSILON 0.000001
#define NB 200
#define N 10000


// original function and function to be used with determinating
typedef void(*funcdPtr)(float,float*,float*);

// two function
typedef struct funcd{
	funcdPtr ptr;
	char* name;
	float lx;
	float rx;
	float xacc;
}funcd;

// method pointer. for Linear Interpolation, secant, bisection
typedef float(*method_ptr)(funcdPtr, float, float, float, int*);

// method struct
typedef struct method{
	method_ptr ptr;
	char* name;
}method;


int main(){
	
	int n=N, nb=NB;
	float xb1[NB], xb2[NB];
	
	float answer;
	int iter=-1;
	

	return 0;
}
