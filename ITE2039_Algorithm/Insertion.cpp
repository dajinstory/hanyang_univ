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
	int tmp, i, j;
	for (i = 1; i < n; i++) {
		tmp = arr[i];
		j = i;
		while (j > 0 && tmp > arr[j - 1]) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = tmp;
	}
}
