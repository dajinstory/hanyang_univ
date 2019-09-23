#include <stdio.h>
#include <string.h>
#include "functions.c"
#include "methods.c"
#define ABS(X) (X < 0 ? -X : X)
#define EPSILON 0.000001
#define NB 20

// original function and function to be used with determinating
typedef float(*funcPtr)(float);
typedef void(*funcdPtr)(float,float*,float*);

// two function
typedef struct func{
	funcPtr ptr0;
	funcdPtr ptr1;
	char* name;
}func;

// method pointer. for Linear Interpolation, secant, bisection
typedef float(*method_ptr)(funcPtr, float, float, float, int*);

// method struct
typedef struct method{
	method_ptr ptr;
	char* name;
	int use_d;
}method;

// function to be used with newton
void bisection_d(float x, float* x1, float* x2){
	*x1 = bessel(x);
	*x2 = bessel_d(x);
}

void func_custom_1(float x, float* x1, float* x2){
	*x1 = func1(x);
	*x2 = func1_d(x);
}
void func_custom_2(float x, float* x1, float* x2){
	*x1 = func2(x);
	*x2 = func2_d(x);
}
void func_custom_3(float x, float* x1, float* x2){
	*x1 = func3(x);
	*x2 = func3_d(x);
}


int main(){
	func functions[5] = {{bessel, bisection_d, "Bessel function"}};	
	method methods[3] = {{rtbis, "Bisection", 0}, {rtflsp, "Linear Interpolation", 0}, {rtsec, "Secant", 0}};
	
	int n=90, nb=NB;
	float xb1[NB], xb2[NB];
	float lx=1.0, rx=10.0, xacc=1e-6;
	float answer;
	int iter=-1;
	
	// function list
	for(int i=0;i<1;i++){
		funcPtr func0 = functions[i].ptr0;
		funcPtr func1 = functions[i].ptr1;
		char* func_name = functions[i].name;

		// set xb
		zbrak(func0, lx, rx, n, xb1, xb2, &nb);
		// select method
		printf("##%s\n", func_name);
		for(int j=0;j<3;j++){
			method_ptr method_function = methods[j].ptr;
			char* method_name = methods[j].name;
			int use_d = methods[j].use_d;
			
			printf("\n%s\n", method_name);
			// solve problem
			for(int k=1;k<=nb;k++){
				answer = method_function(func0, xb1[k], xb2[k], xacc, &iter);
				printf("- area #%d : %e %d time\n", k, answer, iter);
			}
		}

		printf("\n##Newton-Raphson\n");
		for(int k=1;k<=nb;k++){
			answer = rtnewt(func1, xb1[k], xb2[k], xacc, &iter);
			printf("- area #%d : %e %d time\n", k, answer, iter);
		}

		printf("\n##Newton with Bracketing\n");
		for(int k=1;k<=nb;k++){
			answer = rtsafe(func1, xb1[k], xb2[k], xacc, &iter);
			printf("- area #%d : %e %d time\n", k, answer, iter);
		}
		break;
	}


	// with other functions
	printf("##func_custom_1\n");
	zbrak(func1, 0.1, 1.0, n, xb1, xb2, &nb);
	for(int k=1;k<=nb;k++){
		answer = rtsafe(func_custom_1, xb1[k], xb2[k], xacc, &iter);
		printf("- area #%d : %e %d time\n", k, answer, iter);
	}
	
	printf("##func_custom_2\n");
	zbrak(func2, 0.0, 1.0, n, xb1, xb2, &nb);
	for(int k=1;k<=nb;k++){
		answer = rtsafe(func_custom_2, xb1[k], xb2[k], xacc, &iter);
		printf("- area #%d : %e %d time\n", k, answer, iter);
	}
	
	printf("##func_custom_3\n");
	zbrak(func3, -2.0, -1.0, n, xb1, xb2, &nb);
	for(int k=1;k<=nb;k++){
		answer = rtsafe(func_custom_3, xb1[k], xb2[k], xacc, &iter);
		printf("- area #%d : %e %d time\n", k, answer, iter);
	}
	
	return 0;
}
