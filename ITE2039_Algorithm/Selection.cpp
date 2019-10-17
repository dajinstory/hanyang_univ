// 2016026080_วัดูม๘_508
#include <stdio.h>
int arr[30001];

void SelectionSort(int n, int m);

int main() {
	int N, M;
	scanf("%d%d", &N, &M);
	for (int i = 0; i < N; i++) {
		scanf("%d", &arr[i]);
	}
	SelectionSort(N, M);
	for (int i = 0; i < N; i++) {
		printf("%d\n", arr[i]);
	}
	return 0;
}

void SelectionSort(int n, int m) {
	int tmp_index, tmp_value;
	for (int i = 0; i < m; i++) {
		tmp_index = i;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < arr[tmp_index]) {
				tmp_index = j;
			}
		}
		tmp_value = arr[i];
		arr[i] = arr[tmp_index];
		arr[tmp_index] = tmp_value;
	}
}
