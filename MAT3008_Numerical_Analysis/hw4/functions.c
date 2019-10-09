#include <math.h>
#define PI 3.1415926535897
#define E 2.7182818284590

float bessj0(float);
float bessy0(float);
float bessj1(float);
float bessy1(float);

void bessel(float, float*, float*);
void myfunc_1(float, float*, float*);
void myfunc_2(float, float*, float*);
void myfunc_3(float, float*, float*);
void myfunc_4(float, float*, float*);
void myfunc_5(float, float*, float*);
void example_8_32(float, float*, float*);
void example_8_36(float, float*, float*);

//hw3
void bessel(float x, float* y, float* dy){
	*y = bessj0(x);
	*dy = -bessj1(x);
}
void myfunc_1(float x, float* y, float* dy){
	*y = 10*pow(E,-x)*sin(2*PI*x)-2;
	*dy = 10*(pow(E,-x)*2*PI*cos(2*PI*x)-pow(E,-x)*sin(2*PI*x));
}
void myfunc_2(float x, float* y, float* dy){
	*y = x*x-2*x*pow(E,-x)+pow(E,-2*x);
	*dy = 2*x+2*x*pow(E,-x)-2*pow(E,-2*x);
}
void myfunc_3(float x, float* y, float* dy){
	//*y = cos(x+sqrt(2.0))+x*x/2+x*sqrt(2.0));
	*y = cos(x+sqrt(2.0))+x*(x/2+sqrt(2.0));
	*dy = -sin(x+sqrt(2.0))+x+sqrt(2.0);
}
void myfunc_4(float x, float* y, float* dy){
	*y = x*(x*x-0.5);
	*dy = 3*x*x-0.5;
}

// hw4
void myfunc_5(float x, float* y, float* dy){
	//*y = pow(E,-0.005*x) * cos(floor(sqrt(2000-0.01*x*x)*0.05)) - 1;
	//*dy = -0.005*x*pow(E,-0.005*x)*cos(floor(sqrt(2000-0.01*x*x)*0.05));
	*y = pow(E,-0.005*x) * cos(sqrt(2000-0.01*x*x)*0.05) - 0.01;
	*dy = -0.005*x*pow(E,-0.005*x)*cos(sqrt(2000-0.01*x*x)*0.05) + pow(E, -0.005*x)*sin(sqrt(2000-0.1*x*x)*0.05)*0.025/sqrt(2000-0.01*x*x)*0.02*x;
}
void example_8_32(float x, float* y, float* dy){
	// e0 = 8.85 * 10^-12
	// q, Q = 2*10^-5
	// cal1 = q*Q/e0 = (4*10^-10)/(8.85*10^-12) = 400 / 8.85
	double cal1 = 400.0 / 8.85;
	
	//double ans = 4*pow(10,-10)*x / (4*PI*e0*pow((x*x+0.81), 3/2))
	*y = cal1 / (4*PI) * x / pow((x*x+0.81), 3.0/2.0) - 1.0;
	*dy = cal1 / 4*PI * (pow(x*x+0.81, 3.0/2.0) - 3*x*x*pow(x*x+0.81, 1.0/2.0)) / pow(x*x + 0.81, 3.0);
}
void example_8_36(float x, float* y, float* dy){
	*y = 0.99403 + pow(10,-4)*x*(1.671 + pow(10,-4)*x*(9.7215+pow(10,-3)*x*(-9.5838 + pow(10,-3)*x*1.9520))) - 1.2;
	*dy = pow(10,-4)*(1.671 + pow(10,-4)*x*(2.0*9.7215+pow(10,-3)*x*(3.0*(-9.5838) + pow(10,-3)*x*4.0*1.9520)));
}


float bessj0(float x){

	float ax,z;
	double xx,y,ans,ans1,ans2;

	if ((ax=fabs(x)) < 8.0) {

		y=x*x; 
		ans1=57568490574.0+y*(-13362590354.0+y*(651619640.7 +y*(-11214424.18+y*(77392.33017+y*(-184.9052456))))); 
		ans2=57568490411.0+y*(1029532985.0+y*(9494680.718 +y*(59272.64853+y*(267.8532712+y*1.0)))); 
		ans=ans1/ans2; 
	} else { 

		z=8.0/ax; 
		y=z*z; 
		xx=ax-0.785398164; 
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4 +y*(-0.2073370639e-5+y*0.2093887211e-6))); 
		ans2 = -0.1562499995e-1+y*(0.1430488765e-3 +y*(-0.6911147651e-5+y*(0.7621095161e-6 -y*0.934945152e-7))); 
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
	} 
	return ans;
}


float bessy0(float x){

	float bessj0(float x);
	float z; 
	double xx,y,ans,ans1,ans2;
	
	if (x < 8.0) {
		y=x*x; 
		ans1 = -2957821389.0+y*(7062834065.0+y*(-512359803.6 +y*(10879881.29+y*(-86327.92757+y*228.4622733)))); 
		ans2=40076544269.0+y*(745249964.8+y*(7189466.438 +y*(47447.26470+y*(226.1030244+y*1.0)))); 
		ans=(ans1/ans2)+0.636619772*bessj0(x)*log(x); 
	} else { 
		z=8.0/x; y=z*z; xx=x-0.785398164; 
		ans1=1.0+y*(-0.1098628627e-2+y*(0.2734510407e-4+y*(-0.2073370639e-5+y*0.2093887211e-6))); 
		ans2 = -0.1562499995e-1+y*(0.1430488765e-3+y*(-0.6911147651e-5+y*(0.7621095161e-6+y*(-0.934945152e-7)))); 
		ans=sqrt(0.636619772/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
	}
	return ans;

}

float bessj1(float x){
	float ax,z; 
	double xx,y,ans,ans1,ans2;
	
	if ((ax=fabs(x)) < 8.0) {

		y=x*x; 
		ans1=x*(72362614232.0+y*(-7895059235.0+y*(242396853.1 +y*(-2972611.439+y*(15704.48260+y*(-30.16036606)))))); 
		ans2=144725228442.0+y*(2300535178.0+y*(18583304.74 +y*(99447.43394+y*(376.9991397+y*1.0)))); 
		ans=ans1/ans2; 
	} else {
		z=8.0/ax; 
		y=z*z; 
		xx=ax-2.356194491; 
		ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4 +y*(0.2457520174e-5+y*(-0.240337019e-6)))); 
		ans2=0.04687499995+y*(-0.2002690873e-3 +y*(0.8449199096e-5+y*(-0.88228987e-6 +y*0.105787412e-6))); 
		ans=sqrt(0.636619772/ax)*(cos(xx)*ans1-z*sin(xx)*ans2); 
		if (x < 0.0) ans = -ans;
	} 
	return ans;

}


float bessy1(float x){

	float bessj1(float x); 
	float z; 
	double xx,y,ans,ans1,ans2; 

	if (x < 8.0) {
		y=x*x; 
		ans1=x*(-0.4900604943e13+y*(0.1275274390e13 +y*(-0.5153438139e11+y*(0.7349264551e9 +y*(-0.4237922726e7+y*0.8511937935e4))))); 
		ans2=0.2499580570e14+y*(0.4244419664e12 +y*(0.3733650367e10+y*(0.2245904002e8 +y*(0.1020426050e6+y*(0.3549632885e3+y)))));
		ans=(ans1/ans2)+0.636619772*(bessj1(x)*log(x)-1.0/x); 
	} else {
		z=8.0/x; 
		y=z*z; 
		xx=x-2.356194491; 
		ans1=1.0+y*(0.183105e-2+y*(-0.3516396496e-4 +y*(0.2457520174e-5+y*(-0.240337019e-6)))); 
		ans2=0.04687499995+y*(-0.2002690873e-3 +y*(0.8449199096e-5+y*(-0.88228987e-6 +y*0.105787412e-6))); 
		ans=sqrt(0.636619772/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
	} 
	return ans;
}




