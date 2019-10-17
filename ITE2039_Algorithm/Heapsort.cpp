// 2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int arr[100002];

void Insert(int index);
void Pop(int n);

int main() {
	int N, K;
	scanf("%d%d", &N, &K);
	for (int i = 1; i <= N; i++) {
		scanf("%d", &arr[i]);
		Insert(i);
	}
	while(K--){
		printf("%d ", arr[1]);
		Pop(N);
		N--;
	}
	printf("\n");
	for (int i = 1; i <= N; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}

void Insert(int index) {
	int tmp = arr[index];
	while (index > 1 && arr[index / 2] < tmp) {
		arr[index] = arr[index / 2];
		index /= 2;
	}
	arr[index] = tmp;
}

void Pop(int n) {
	int tmp = arr[n], index_now = 1, index_next;
	while (index_now * 2 <= n) {
		index_next = index_now * 2;
		if (index_next < n && arr[index_next + 1] > arr[index_next]) {
			index_next++;
		}
		if (tmp < arr[index_next]) {
			arr[index_now] = arr[index_next];
		}
		else {
			break;
		}
		index_now = index_next;
	}
	arr[index_now] = tmp;
}
