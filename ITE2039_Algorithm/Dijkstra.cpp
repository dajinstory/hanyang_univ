//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

int N, V, E;
vector<pair<int, int> >map[5004];
priority_queue<pair<int, int> >myQ;
int visit[5004];
void getData() {
	int dest, cost;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d%d", &V, &E);
		for (int j = 0; j < E; j++) {
			scanf("%d%d", &dest, &cost);
			map[V].push_back(make_pair(dest, cost));
		}
	}
}
void solve() {
	int v, c, n = 0;;
	myQ.push(make_pair(0, 1));
	while (!myQ.empty()) {
		v = myQ.top().second;
		c = -myQ.top().first;
		myQ.pop();
		if (visit[v]) {
			continue;
		}
		visit[v] = 1;
		n++;
		if (n == N) {
			printf("%d", c);
			return;
		}
		for (int i = 0; i < map[v].size(); i++) {
			if (visit[map[v][i].first]) {
				continue;
			}
			myQ.push(make_pair(-(c + map[v][i].second), map[v][i].first));
		}
	}
}
int main() {
	getData();
	solve();
	return 0;
}