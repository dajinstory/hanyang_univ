#include <stdio.h>

// power function for float data type. time is positive integer
float power_float(float num, int time){
	float re=1.0;
	while(time--) re *= num;
	return re;
}

// power function for double data type. time is positive integer
double power_double(double num, int time){
	double re=1.0;
	while(time--) re *= num;
	return re;
}

// function to get n for float epsilon
int get_epsilon_float_n(void){
	int n=0;
	float one=1.0;
	while(one+power_float(0.5, ++n)!=one);
	return n;
}

// function to get n for double epsilon
int get_epsilon_double_n(void){
	int n=0;
	double one=1.0;
	while(one+power_double(0.5, ++n)!=one);
	return n;
}

// function to get float epsilon
float get_epsilon_float(void){
	return power_float(0.5, get_epsilon_float_n()-1);
}

// function to get double epsilon
double get_epsilon_double(void){
	return power_double(0.5, get_epsilon_double_n()-1);
}

