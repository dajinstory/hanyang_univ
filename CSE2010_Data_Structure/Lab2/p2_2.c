#include <stdio.h>
int main() {
	char userName[31];
	printf("enter your name:\n");
	fgets(userName, 30, stdin);
	printf("your name is %s", userName);
	return 0;
}