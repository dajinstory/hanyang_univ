//2016026080_한다진_508
//2016026080_Handajin_508
#include<stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

int N, error=0;
stack<int>myS;
vector<int>map[1004];
int visit[1004];
int check[1004];
void getData() {
	int s, e;
	scanf("%d", &N);
	while ((scanf("%d%d", &s, &e)) != EOF) {
		map[s].push_back(e);
	}
	for (int i = 1; i <= N; i++) {
		sort(map[i].begin(), map[i].end());
	}
}
void solve(int now) {
	check[now] = 1;
	for (int i = 0; i < map[now].size(); i++) {
		if (check[map[now][i]]) {
			error=1;
			return;
		}
		if (!visit[map[now][i]]) {
			solve(map[now][i]);
		}
	}
	myS.push(now);
	visit[now] = 1;
	check[now] = 0;
}
int main() {
	getData();
	for (int i = 1; i <= N; i++) {
		if (!visit[i]) {
			solve(i);
		}
	}
	if(error){
		printf("0");
	}
	else{
		printf("1\n");
		while (!myS.empty()) {
			printf("%d ", myS.top());
			myS.pop();
		}
	}
	return 0;
}