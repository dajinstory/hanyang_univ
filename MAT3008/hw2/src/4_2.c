#include <stdio.h>
#include <math.h>

void solve_4_2(double x){
	double true_value, ans, plus;
	true_value = cos(x);
	ans = 1.0;
	plus = 1.0;
	for(int i=2;ans!=true_value;i+=2){
		plus *= -x*x/(double)(i*(i-1));
		ans += plus;
		printf("true value: %e\nvalue: %e\npercent relative errors: %e\n\n", true_value, ans, fabs(ans-true_value)*100/ans);
	}
}

int main(){
	solve_4_2(60.0*M_PI/180);
	return 0;
}
