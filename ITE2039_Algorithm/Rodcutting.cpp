//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
#include <algorithm>
using namespace std;

int N, arr[104], root[104];
int members[104], memSize = 0;

void solve();
void printMem();
void getMem(int n);

int main() {
	solve();
	return 0;
}

void solve() {
	scanf("%d", &N);
	for (int i = 1; i <= N; i++) {
		scanf("%d", &arr[i]);
		root[i] = i;
		for (int j = 1; j < i; j++) {
			if (arr[j] + arr[i - j]>arr[i]) {
				arr[i] = arr[j] + arr[i - j];
				root[i] = i - j;
			}
		}
	}
	printf("%d\n", arr[N]);
	printMem();
}

void printMem() {
	getMem(N);
	for (int i = 1; i <= N; i++) {
		for (int k = 0; k < members[i]; k++) {
			printf("%d ", i);
		}
	}
}

void getMem(int n) {
	if (root[n] == n) {
		members[n]++;
		return;
	}
	getMem(root[n]);
	getMem(n - root[n]);
}