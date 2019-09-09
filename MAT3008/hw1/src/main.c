#include <stdio.h>
#include <float.h>


void machar(int *ibeta, int *it, int *irnd, int *ngrd, int *machep, int *negep,
            int *iexp, int *minexp, int *maxexp, float *eps, float *epsneg,
            float *xmin, float *xmax);

void machar_double(int *ibeta, int *it, int *irnd, int *ngrd, int *machep, int *negep,
            int *iexp, int *minexp, int *maxexp, double *eps, double *epsneg,
            double *xmin, double *xmax);

// power function for float data type. time is positive integer
float power_float(float num, int time);

// power function for double data type. time is positive integer
double power_double(double num, int time);

// function to get n for float epsilon
int get_epsilon_float_n(void);

// function to get n for double epsilon
int get_epsilon_double_n(void);

// function to get float epsilon
float get_epsilon_float(void);

// function to get double epsilon
double get_epsilon_double(void);

int main(){

	// method 1
	printf("METHOD #1\n");
	int ibeta, it, irnd, ngrd, machep, negep, iexp, minexp, maxexp;
	float eps, epsneg, xmin, xmax;
	double eps_d, epsneg_d, xmin_d, xmax_d;
	
	machar(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp, &eps, &epsneg, &xmin, &xmax);
	printf("machar float epsilon: %e, %.23f\n",eps, eps);
	machar_double(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp, &eps_d, &epsneg_d, &xmin_d, &xmax_d);
	printf("machar double epsilon: %e, %.52f\n\n",eps_d, eps_d);
	
	// method 2
	printf("METHOD #2\n");
	float epsilon_float = get_epsilon_float();
	double epsilon_double = get_epsilon_double();
	
	printf("my float epsilon n: %d\n", get_epsilon_float_n());
	printf("my float epsilon: %e, %.23f\n", epsilon_float, epsilon_float);
	printf("MACHINE FLTOAT_EPSILON: %e\n\n", FLT_EPSILON);

	printf("my double epsilon n: %d\n", get_epsilon_double_n());
	printf("my double epsilon: %e, %.52f\n", epsilon_double, epsilon_double);
	printf("MACHINE DOUBLE_EPSILON: %e\n\n", DBL_EPSILON);
	return 0;
}
