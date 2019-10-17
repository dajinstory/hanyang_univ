// 2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int arr[100001];
int tmp[100001];

void MergeSort(int start, int end);
void Merge(int first_begin, int first_end, int second_begin, int second_end);

int main() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &arr[i]);
	}
	MergeSort(0, N - 1);
	for (int i = 0; i < N; i++) {
		printf("%d\n", arr[i]);
	}
	return 0;
}

void MergeSort(int start, int end) {
	if (end - start < 1) {
		return;
	}
	int middle = (start + end) / 2;
	MergeSort(start, middle);
	MergeSort(middle + 1, end);
	Merge(start, middle, middle + 1, end);
}

void Merge(int first_index, int first_end, int second_index, int second_end){
	int start = first_index;
	for (int i = first_index; i <= second_end; i++) {
		if (first_index > first_end) {
			tmp[i] = arr[second_index];
			second_index++;
		}
		else if (second_index > second_end) {
			tmp[i] = arr[first_index];
			first_index++;
		}
		else {
			if (arr[first_index] > arr[second_index]) {
				tmp[i] = arr[first_index];
				first_index++;
			}
			else {
				tmp[i] = arr[second_index];
				second_index++;
			}
		}
	}
	for (int i = start; i <= second_end; i++) {
		arr[i] = tmp[i];
	}
}