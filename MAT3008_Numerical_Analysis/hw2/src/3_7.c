#include <stdio.h>
#include <math.h>

double func(double x, int digits){
	double ans, x2;
	int mul=1;
	while(digits--) mul*=10;
	
	x2 = round(x*x*mul)/mul;
	ans = 1.0/(1.0-3.0*x2);
	return round(ans*mul)/mul;
}

double func_d(double x, int digits){
	double ans, x2, x4;
	int mul=1;
	while(digits--) mul*=10;
	
	x2 = round(x*x*mul)/mul;
	x4 = round(x*x*x*x*mul)/mul;
	ans = (6.0*x)/(1.0-6.0*x2+9.0*x4);
	return round(ans*mul)/mul;
}


int main(){
	printf("# digits 3\nfunc: %e\nfunc_derivate: %e\n\n", func(0.577, 3), func_d(0.577,3));
	printf("# digits 4\nfunc: %e\nfunc_derivate: %e\n\n", func(0.577, 4), func_d(0.577,4));
	return 0;
}
