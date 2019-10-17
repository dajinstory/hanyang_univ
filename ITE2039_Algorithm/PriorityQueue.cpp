// 2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
int arr[100002];

void Insert(int i, int v);
void Pop(int r, int i, int n);
void Modify(int i, int v, int n);

int main() {
	int order, index, value, size = 0;
	while (true) {
		scanf("%d", &order);
		if (order == 1) {
			scanf("%d", &value);
			size++;
			Insert(size, value);
		}
		else if (order == 2) {
			printf("%d ", arr[1]);
			Pop(1, size, size);
			size--;
		}
		else if (order == 3) {
			scanf("%d%d", &index, &value);
			Modify(index, value, size);
		}
		else {
			break;
		}
	}
	printf("\n");
	for (int i = 1; i <= size; i++) {
		printf("%d ", arr[i]);
	}
	return 0;
}

void Insert(int i, int v) {
	while (i> 1 && arr[i / 2] <= v) {
		arr[i] = arr[i / 2];
		i /= 2;
	}
	arr[i] = v;
}

void Pop(int r, int i, int n) { // root, index, size
	int tmp = arr[i], index_now = r, index_next;
	while (index_now * 2 <= n) {
		index_next = index_now * 2;
		if (index_next < n && arr[index_next + 1] >= arr[index_next]) {
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
	if (v > arr[i]) {
		Insert(i, v);
	}
	else if (v < arr[i]) {
		arr[i] = v;
		Pop(i, i, n);
	}
}