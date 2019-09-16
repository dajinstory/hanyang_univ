#include <stdio.h>
#include <math.h>

double func[5]={-88.0,7.0,-6.0,25.0,0.0};
void solve_4_5(void){
	const int degree = 4;
	const int step = 3;
	const int d1=degree+1;
	const int s1=step+1;

	const double base = 1.0;
	const double predict_x=3.0;
	double taylor[s1][d1];
	double ans=0.0;
	double true_value=0.0;
	
	double tx=1.0/predict_x;
	for(int i=0;i<degree+1;i++){
		taylor[0][i]=func[i];
		tx*=predict_x;
		true_value+=func[i]*tx;
	}
	for(int i=1;i<step+1;i++){
		double x = 1.0/base;
		for(int j=0;j<degree;j++){
			x*=base;
			taylor[i][j]=taylor[i-1][j+1]*(j+1)*x;
		}
	}
	double sum = 0.0;
	double xx = 1.0/(predict_x-base);
	for(int i=0;i<=step;i++){
		sum = 0.0;
		xx *= predict_x-base;
		for(int j=0;j<degree;j++){
			sum+=taylor[i][j];
		}
		ans+=sum*(predict_x-base);
		printf("%e\n", fabs(true_value-ans)*100-true_value);
	}
}

int main(){
	solve_4_5();
	return 0;
}
