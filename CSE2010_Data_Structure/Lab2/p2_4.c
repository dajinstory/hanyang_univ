#include <stdio.h>

int Add(int a, int b){
	return a+b;
}
int Sub(int a, int b){
	return a-b;
}
int Mul(int a, int b){
	return a*b;
}
int Div(int a, int b){
	return a/b;
}
int (*functionPointer[4])(int, int)={Add,Sub,Mul,Div};

int main(){
	int calculateType, tmp1, tmp2;
	while(1){
		printf("=====================\n");
		printf("0. add\n");
		printf("1. sub\n");
		printf("2. mul\n");
		printf("3. div\n");
		printf("4. exit\n");
		printf("=====================\n");
		printf("Select Operation :");
		scanf("%d", &calculateType);
		if(calculateType==4){
			break;
		}
		printf("Input 2 operand :");
		scanf("%d%d", &tmp1, &tmp2);
		printf("Result = %d\n", functionPointer[calculateType](tmp1, tmp2));
	}
	return 0;
} 

