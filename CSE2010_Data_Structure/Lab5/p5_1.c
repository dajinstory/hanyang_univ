#include <stdio.h>

#include <string.h>

#include <stdlib.h>

 

typedef struct Stack {

	int *key;

	int top;

	int max_stack_size;

}Stack;

 

Stack* CreateStack(int max);

void Push(Stack* S, int X);

void Pop(Stack* S);

void DeleteStack(Stack* S);

int IsEmpty(Stack* S);

int IsFull(Stack *S);

 

int main(int argc, char* argv[]) {

	FILE* fi;

	fi = fopen(argv[1], "r");

	char input_str[101];

	int tmp, num;

	Stack* stack;

	fgets(input_str, 101, fi);

	printf("Pushed numbers :");

	stack = CreateStack(101);

	for (num = 0; num < 101; num++) {

		if (input_str[num] == '+') {

 

		}

		else if (input_str[num] == '#') {

			break;

		}

		else {

			Push(stack, input_str[num] - '0');

		}

	}

	printf("\nEvaluation result : %d\n", stack->key[stack->top]);

	fclose(fi);

	//return 0;

	DeleteStack(stack);

	return 0;

}

 

Stack* CreateStack(int max) {

	Stack* S = NULL;

	S = (Stack*)malloc(sizeof(max));

	S->key = (int*)malloc(sizeof(int)*max);

	S->max_stack_size = max;

	S->top = -1;

	return S;

}

void Push(Stack* S, int X) {

	if (IsFull(S)) {

		printf("Stack is already full!\n");

		return;

	}

	S->top++;

	S->key[S->top] = X;

	printf(" %d", X);

}

void Pop(Stack* S) {

	if (IsEmpty(S)) {

		printf("Error");

		return;

	}

	S->top--;

}

void DeleteStack(Stack* S) {

	free(S->key);

	free(S);

}

int IsEmpty(Stack* S) {

	if (S->top == -1) {

		return 1;

	}

	else {

		return 0;

	}

}

int IsFull(Stack *S) {

	if (S->top == S->max_stack_size - 1) {

		return 1;

	}

	else {

		return 0;

	}

}

