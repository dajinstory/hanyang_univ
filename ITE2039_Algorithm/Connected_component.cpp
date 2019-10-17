//2016026080_ÇÑ´ÙÁø_508
//2016026080_Handajin_508

#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, set_num=0;
int visit[1004];
vector<int> map[1004];
queue<int> myQ;
void getData() {
	int x, y;
	scanf("%d", &N);
	while ((scanf("%d%d", &x, &y)) != EOF) {
		map[x].push_back(y);
		map[y].push_back(x);
	}
}
void link(int n) {
	int now;
	myQ.push(n);
	visit[n] = set_num;
	while (!myQ.empty()) {
		now = myQ.front();
		myQ.pop();
		for (int i = 0; i < map[now].size(); i++) {
			if (visit[map[now][i]] == 0) {
				myQ.push(map[now][i]);
				visit[map[now][i]] = set_num;
			}
		}
	}
}
int main() {
	getData();
	for (int i = 1; i <= N; i++) {
		if (visit[i] == 0) {
			set_num++;
			link(i);
		}
	}
	printf("%d\n", set_num);
	for (int i = 1; i <= N; i++) {
		printf("%d\n", visit[i]);
	}
	return 0;
}