//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int n, dp[104][104][2], arr[104];

int solve(int s, int e) {
	if (s == e) {
		dp[s][e][1] = s;
		return 0;
	}
	if (dp[s][e][0] != 0) {
		return dp[s][e][0];
	}
	else {
		int min = 987654321;
		for (int i = s; i < e; i++) {
			if (min >= solve(s, i) + solve(i + 1, e) + arr[s - 1] * arr[i] * arr[e]) {
				min = solve(s, i) + solve(i + 1, e) + arr[s - 1] * arr[i] * arr[e];
				dp[s][e][1] = i;
			}
		}
		return dp[s][e][0] = min;
	}
}
void printNum(int s, int e) {
	if (s == e) {
		printf("%d", s);
		return;
	}
	printf("(");
	printNum(s, dp[s][e][1]);
	printf(" ");
	printNum(dp[s][e][1] + 1, e);
	printf(")");
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i <= n; i++) {
		scanf("%d", &arr[i]);
	}
	printf("%d\n", solve(1,n));
	printNum(1, n);
	return 0;
}