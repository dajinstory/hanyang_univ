// 2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int arr[30001];

void InsertionSort(int n);

int main() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &arr[i]);
	}
	InsertionSort(N);
	for (int i = 0; i < N; i++) {
		printf("%d\n", arr[i]);
	}
	return 0;
}

void InsertionSort(int n) {
	int tmp_before, tmp_now, i, j;
	for (i = 1; i < n; i++) {
		for (j = 0; j < i; j++) {
			if (arr[i] > arr[j]) {
				tmp_now = arr[j];
				arr[j] = arr[i];
				break;
			}
		}
		for (j = j + 1; j <= i; j++) {
			tmp_before = tmp_now;
			tmp_now = arr[j];
			arr[j] = tmp_before;
		}
	}
}
