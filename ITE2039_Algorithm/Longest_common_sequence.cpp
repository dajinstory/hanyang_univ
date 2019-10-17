//2016026080_ÇÑ´ÙÁø_508
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

int matrix[504][504], al, bl;
string A, B;

int max(int a, int b) {
	if (a < b)return b;
	return a;
}

void printNum(int a, int b, int maxNum) {
	int newEnd = b;
	bool isPrint = false;
	if (maxNum == 0) {
		return;
	}

	for (int j = b; j > 0 && matrix[a][j] == maxNum; j--) {
		if (A[a - 1] == B[j - 1]) {
			newEnd = j - 1;
			isPrint = true;
			break;
		}
	}
	if (isPrint) {
		printNum(a - 1, newEnd, maxNum - 1);
		printf("%c", A[a - 1]);
	}
	else {
		printNum(a - 1, newEnd, maxNum);
	}
}

int main() {
	cin >> A >> B;
	al = A.length(), bl = B.length();

	for (int i = 1; i <= al; i++) {
		for (int j = 1; j <= bl; j++) {
			if (A[i - 1] == B[j - 1]) {
				matrix[i][j] = matrix[i - 1][j - 1] + 1;
			}
			else {
				matrix[i][j] = max(matrix[i][j - 1], matrix[i - 1][j]);
			}
		}
	}

	printNum(al, bl, matrix[al][bl]);
	return 0;
}