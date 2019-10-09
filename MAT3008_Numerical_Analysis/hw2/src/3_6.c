#include <stdio.h>

double func_3_6_1(double x, int iter){
	double ans=1.0, plus=1.0;
	for(int i=1;i<=iter;i++){
		plus *= -((double)x/(double)i);
		ans += plus;
	}
	return ans;
}
double func_3_6_2(double x, int iter){
	double ans=1.0, plus=1.0;
	for(int i=1;i<=iter;i++){
		plus *= ((double)x/(double)i);
		ans += plus;
	}
	return 1.0/ans;
}

void solve_3_6(void){
	double true_value=6.737947e-3, value;
	printf("true value: %e\n", true_value);
	
	printf("\n# func 1\n");
	printf("value: %e\n", value = func_1(5.0, 20));
	printf("relative error: %e\n",(true_value-value)/true_value);
	printf("\n# func 2\n");
	printf("value: %e\n", value = func_2(5.0, 20));
	printf("relative error: %e\n",(true_value-value)/true_value);
}

int main(){
	solve_3_6();
	return 0;
}
