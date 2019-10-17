//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>

int query[100004][2];
int count[100004];
int countSum[100004];

int main() {
	int N, M, K, tmp;
	scanf("%d%d%d", &N, &M, &K);
	for (int i = 0; i < K; i++) {
		scanf("%d%d", &query[i][0], &query[i][1]);
	}
	for (int i = 0; i < N; i++) {
		scanf("%d", &tmp);
		count[tmp]++;
	}
	for (int i = 1; i <= M; i++) {
		countSum[i] = countSum[i - 1] + count[i];
	}
	for (int i = 0; i < K; i++) {
		printf("%d\n", countSum[query[i][1]] - countSum[query[i][0] - 1]);
	}
	return 0;
}