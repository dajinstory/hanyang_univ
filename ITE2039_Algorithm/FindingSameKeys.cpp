//2016026080_วัดูม๘_508
#include <stdio.h>
bool isInA[100004];
int main() {
	int N, M, ansSize = 0, tmp;
	scanf("%d%d", &N, &M);
	for (int i = 0; i < N; i++) {
		scanf("%d", &tmp);
		isInA[tmp] = true;
	}
	for (int i = 0; i < M; i++) {
		scanf("%d", &tmp);
		if (isInA[tmp]) {
			ansSize++;
		}
	}
	printf("%d", ansSize);
	return 0;
}