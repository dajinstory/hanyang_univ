//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int N, D[2][104], T[2][104], S[2][104], E[2][2];
bool root[2][104];

void getData();
void solve();
void printRoot(int n, int r);
int min(int r, int a, int b, int index);

int main() {
	getData();
	solve();
	return 0;
}

int min(int r, int a, int b, int index) {
	if (a <= b) {
		root[r][index] = r;
		return a;
	}
	root[r][index] = -r + 1;
	return b;
}

void getData() {
	scanf("%d", &N);
	scanf("%d%d", &E[0][0], &E[1][0]);
	scanf("%d%d", &E[0][1], &E[1][1]);
	for (int i = 1; i <= N; i++) {
		scanf("%d", &S[0][i]);
	}
	for (int i = 1; i <= N; i++) {
		scanf("%d", &S[1][i]);
	}
	for (int i = 1; i <= N-1; i++) {
		scanf("%d", &T[0][i]);
	}
	for (int i = 1; i <= N-1; i++) {
		scanf("%d", &T[1][i]);
	}
	D[0][1] = E[0][0] + S[0][1];
	D[1][1] = E[1][0] + S[1][1];
}

void solve() {
	for (int i = 2; i <= N; i++) {
		D[0][i] = min(0, D[0][i - 1], D[1][i - 1] + T[1][i - 1], i) + S[0][i];
		D[1][i] = min(1, D[1][i - 1], D[0][i - 1] + T[0][i - 1], i) + S[1][i];
	}
	printf("%d\n", min(0, D[0][N] + E[0][1], D[1][N] + E[1][1], N+1));
	printRoot(N + 1, 0);
}

void printRoot(int n, int r) {
	if (n > 2) {
		printRoot(n - 1, root[r][n]);
	}
	printf("%d %d\n", root[r][n] + 1, n - 1);
}