#include <stdio.h>
void SWAP(int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
int main() {
	int A, B;
	printf("enter 2 integers\n");
	scanf("%d%d", &A, &B);
	printf("you entered\n%d, %d\n", A, B);
	SWAP(&A, &B);
	printf("after swapping\n%d, %d\n", A, B);
}