#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

 

typedef struct Graph {

	int num;//size

	int** weight;//size+1 * size+1

	int* check_visit;//100

}graph;

 

typedef struct Stack {

	int* content;

	int top;

	int max_stack_size;

}stack;

 

typedef struct CircularQueue {

	int* content;

	int first;

	int rear;

	int qsize;

	int max_queue_size;

}queue;

 

graph makeGraph(FILE* fi);

 

void DFS_recursive(graph g, int start, int end);

void DFS_iterative(graph g, int start, int end);

void BFS_search(graph g, int start, int end);

 

 

stack* create_stack(int num);

void push(stack* s, int value);

int pop(stack* s);

void close_stack(stack* s);

bool sIsFull(stack* s);

bool sIsEmpty(stack* s);

 

 

queue* create_queue(int size);

void enqueue(queue* q, int value);

int dequeue(queue* q);

void close_queue(queue* q);

bool qIsFull(queue* q);

bool qIsEmpty(queue* q);

 

void main(int argc, char* argv[]) {

	FILE* fi = fopen(argv[1], "r");

	graph g = makeGraph(fi);

	

	int start, end, index;

	fscanf(fi, "%d-%d", &start, &end);

 

	

	printf("DFS recursive :");

	DFS_recursive(g, start, end);

	if (g.check_visit[end-1] == 0)

		printf(" cannot find");

	//reset g.check_visit

	for (index = 0; index <= g.num; index++) {

		g.check_visit[index] = 0;

	}

	

	printf("\nDFS iterative :");

	DFS_iterative(g, start, end);

	//reset g.check_visit

	for (index = 0; index <= g.num; index++) {

		g.check_visit[index] = 0;

	}

 

	printf("\nBFS:");

	BFS_search(g, start, end);

	printf("\n");

 

	//free all the memory

	for (index = 0; index < g.num + 1; index++) {

		free(g.weight[index]);

	}

	free(g.weight);

	free(g.check_visit);

	fclose(fi);

}

 

//그래프 만드는 함수---------------------------------------

graph makeGraph(FILE* fi) {

	graph tmpGraph;

	int size, index;

	fscanf(fi, "%d\n", &size);

 

	//tmpGraph 동적할당

	tmpGraph.num = size;

	tmpGraph.check_visit = (int*)malloc(sizeof(int)*(100));

	tmpGraph.weight = (int**)malloc(sizeof(int*)*(size + 1));

	for (index = 0; index < size + 1; index++) {

		tmpGraph.weight[index] = (int*)malloc(sizeof(int)*(size + 1));

	}

 

	//tmpGraph.check_visit 초기화

	for (index = 0; index < 100; index++) {

		tmpGraph.check_visit[index] = 0;

	}

 

	//tmpGraph.weight 값 초기화

	int index_i, index_j;

	for (index_i = 0; index_i < size + 1; index_i++) {

		for (index_j = 0; index_j < size + 1; index_j++) {

			tmpGraph.weight[index_i][index_j] = 0;

		}

	}

 

	//tmpGraph.weight 값 설정

	char temp = 0;

	while (temp != '\n') {

		int node1, node2;

		fscanf(fi, "%d-%d", &node1, &node2);

		tmpGraph.weight[node1][node2] = 1;

		temp = fgetc(fi);

	}

 

	//return

	return tmpGraph;

}

 

// 순회해서 출력하는 함수들---------------------------------

 

void DFS_recursive(graph g, int start, int end) {

	if (g.check_visit[end - 1] == 1) {

		return;

	}

	g.check_visit[start-1] = 1;

	printf(" %d", start);

	int next;

	for (next = 1; next <= g.num; next++) {

		if (g.weight[start][next]==1 && g.check_visit[next-1] == 0) {

			DFS_recursive(g, next, end);

		}

	}

}

void DFS_iterative(graph g, int start, int end) {

	stack* myS = create_stack(100);

	int now, next;

 

	push(myS, start);

	g.check_visit[start] = 1;

	while (!sIsEmpty(myS)) {

		now = pop(myS);

		printf(" %d", now);

		if (now == end) {

			close_stack(myS);

			return;

		}

		for (next = 1; next <= g.num; next++) {

			if (g.weight[now][next] == 1 && g.check_visit[next] == 0) {

				push(myS, next);

				g.check_visit[next] = 1;

			}

		}

	}

	printf(" cannot find");

	close_stack(myS);

}

 

void BFS_search(graph g, int start, int end) {

	queue* myQ = create_queue(100);

	int now, next;

 

	enqueue(myQ, start);

	g.check_visit[start] = 1;

	while (!qIsEmpty(myQ)) {

		now = dequeue(myQ);

		printf(" %d", now);

		if (now == end) {

			close_queue(myQ);

			return;

		}

		for (next = 1; next <= g.num; next++) {

			if (g.weight[now][next] == 1 && g.check_visit[next] == 0) {

				enqueue(myQ, next);

				g.check_visit[next] = 1;

			}

		}

	}

	printf(" cannot find");

	close_queue(myQ);

}

 

//stack 관련 함수들----------------------------------------

 

stack* create_stack(int num) {

	stack* S = NULL;

	S = (stack*)malloc(sizeof(stack));

	S->content = (int*)malloc(sizeof(int)*num);

	S->max_stack_size = num;

	S->top = -1;

	return S;

}

void push(stack* s, int value) {

	if (sIsFull(s)) {

		printf("Stack is already full!\n");

		return;

	}

	s->top++;

	s->content[s->top] = value;

}

int pop(stack* s) {

	if (sIsEmpty(s)) {

		printf("Error");

		return -1;

	}

	int topValue = s->content[s->top];

	s->top--;

	return topValue;

}

void close_stack(stack* s) {

	free(s->content);

	free(s);

}

bool sIsFull(stack* s) {

	if (s->top == s->max_stack_size - 1) {

		return true;

	}

	else {

		return false;

	}

}

bool sIsEmpty(stack* s) {

	if (s->top == -1) {

		return true;

	}

	else {

		return false;

	}

}

 

 

 

//queue 관련 함수들----------------------------------------

queue* create_queue(int size) {

	queue* L;

	L = (queue*)malloc(sizeof(queue));

	L->content = (int*)malloc(sizeof(int)*size);

	L->max_queue_size = size;

	L->qsize = 0;

	L->first = 1;

	L->rear = 0;

	return L;

}

void enqueue(queue* q, int value) {

	if (qIsFull(q)) {

		printf("Full queue\n");

		return;

	}

	q->qsize++;

	q->rear = (q->rear + 1) % (q->max_queue_size);

	q->content[q->rear] = value;

}

int dequeue(queue* q) {

	if (qIsEmpty(q)) {

		printf("Dequeue Failed : Queue is empty!\n");

		return -1;

	}

	int firstNum = q->content[q->first];

	q->first = (q->first + 1) % (q->max_queue_size);

	q->qsize--;

	return firstNum;

}

void close_queue(queue* q) {

	free(q->content);

	free(q);

}

bool qIsFull(queue* q) {

	if (q->qsize == q->max_queue_size) {

		return true;

	}

	else {

		return false;

	}

}

bool qIsEmpty(queue* q) {

	if (q->qsize == 0) {

		return true;

	}

	else {

		return false;

	}

}

