// 2016026080_한다진_508
#include <stdio.h>
int arr[800002];

void Insert(int index);
void Pop(int s, int n, int i);
void Modify(int i, int v, int n);

int main() {
	int N = 0, order, index, value;
	while (true) {
		scanf("%d", &order);
		if (order == 1) {
			N++;
			scanf("%d", &arr[N]);
			Insert(N);
		}
		else if (order == 2) {
			printf("%d ", arr[1]);
			Pop(1, N, N);
			N--;
		}
		else if (order == 3) {
			scanf("%d%d", &index, &value);
			Modify(index, value, N);
		}
		else {
			break;
		}
	}
	printf("\n");
	for (int i = 1; i <= N; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}

void Insert(int index) {//넣을 위치 = index
	int tmp = arr[index];
	while (index > 1 && arr[index / 2] < tmp) {
		arr[index] = arr[index / 2];
		index /= 2;
	}
	arr[index] = tmp;
}

void Pop(int s, int n, int i) {//팝 시작할 위치 = s, 전체크기 = n, 끌어올 index = i
	int tmp = arr[i], index_now = s, index_next;
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

void Modify(int i, int v, int n) {
	if (arr[i] <= v) {
		arr[i] = v;
		Insert(i);
	}
	else {
		arr[i] = v;
		Pop(i, n, i);
	}
}