//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
using namespace std;

priority_queue<pair<int, int> > myQ;
char character[5];
int numOfCharacter;
int num, totalNum, bitNum;
long long int totalCost;

int max(int a, int b) {
	if (a < b)return b;
	return a;
}
void getData() {
	int tmp = 1;
	scanf("%d", &numOfCharacter);
	for (int i = 0; i < numOfCharacter; i++) {
		scanf("%s", character);
		scanf("%d", &num);
		myQ.push(make_pair(-num, num));
	}
	while (tmp<numOfCharacter) {
		bitNum++;
		tmp *= 2;
	}
	scanf("%d", &totalNum);
}
void solve() {
	pair<int, long long int> tmp1, tmp2;
	while (true) {
		tmp1 = myQ.top();
		myQ.pop();
		if (myQ.empty()) {
			printf("%d\n", bitNum*totalNum);
			printf("%lld", totalCost);
			return;
		}
		tmp2 = myQ.top();
		myQ.pop();
		totalCost += tmp1.second + tmp2.second;
		myQ.push(make_pair(tmp1.first + tmp2.first, tmp1.second + tmp2.second));
	}
}
int main() {
	getData();
	solve();
	return 0;
}