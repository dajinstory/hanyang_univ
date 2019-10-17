#define MAX_STR_SIZE 100

#define MAX_ARR_SIZE 100

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

 

typedef struct _Queue* Queue;

typedef struct _Graph* Graph;

 

struct _Queue {

	int* key;

	int first;

	int rear;

	int qsize;

	int max_queue_size;

};

 

struct _Graph {

	int size;

	int* node;

	int** matrix;

};

 

Graph CreateGraph(int* nodes);//int ok

Graph InsertEdge(Graph G, int a, int b);

void DeleteGraph(Graph G);

void Topsort(Graph G);

Queue MakeNewQueue(int X);

int IsEmpty(Queue Q);

int IsFull(Queue Q);

int Dequeue(Queue* Q);

void Enqueue(Queue* Q, int X);

void DeleteQueue(Queue Q);//Queue* ok

void MakeEmpty(Queue Q);//Queue* ok

 

int main(int argc, char* argv[]) {

	FILE *fi = fopen(argv[1], "r");

	char read[MAX_STR_SIZE];

	int* nodes; // 0부터 size 까지. 0번쨰 값에는 숫자 개수 넣기

	int numbers[MAX_ARR_SIZE];

	int index, numbers_size, num = 0;

	int i, j, a, b;

	Graph topGraph;

 

	fgets(read, MAX_STR_SIZE, fi);

	for (index = 0, numbers_size = 0; index < MAX_STR_SIZE; index++) {

		if (read[index] == '\n' || read[index] == ' ') {

			numbers[numbers_size] = num;

			num = 0;

			numbers_size++;

		}

		else {

			num = num * 10 + read[index] - '0';

		}

	}

	nodes = (int*)malloc(sizeof(int)*(numbers_size + 1));

	nodes[0] = numbers_size;

	for (index = 0; index < numbers_size; index++) {

		nodes[index + 1] = numbers[index];

	}

	topGraph = CreateGraph(nodes);

 

	while (fscanf(fi, "%d-%d ", &a, &b) != EOF) {

		topGraph = InsertEdge(topGraph, a, b);

	}

	for (i = 0; i <= topGraph->size; i++) {

		for (j = 0; j <= topGraph->size; j++) {

			if (i == 0 && j == 0) {

				printf("  ");

				continue;

			}

			printf("%d ", topGraph->matrix[i][j]);

		}

		printf("\n");

	}

 

	//free

	free(nodes);

	DeleteGraph(topGraph);

	fclose(fi);

	return 0;

}

 

//size = 개수

// node 0부터 size-1 까지

// matrix 0부터 size 까지

Graph CreateGraph(int* nodes) {

	Graph tmpGraph;

	tmpGraph = (Graph)malloc(sizeof(struct _Graph));

	int i, j;

	tmpGraph->size = nodes[0];

	tmpGraph->node = (int*)malloc(sizeof(int)*(tmpGraph->size));

 

	for (i = 0; i < tmpGraph->size; i++) {

		tmpGraph->node[i] = nodes[i + 1];

	}

 

	tmpGraph->matrix = (int**)malloc(sizeof(int*)*(tmpGraph->size + 1));

	for (i = 0; i <= tmpGraph->size; i++) {

		tmpGraph->matrix[i] = (int*)malloc(sizeof(int)*(tmpGraph->size + 1));

		for (j = 0; j <= tmpGraph->size; j++) {

			tmpGraph->matrix[i][j] = 0;

		}

	}

	for (i = 1; i <= tmpGraph->size; i++) {

		tmpGraph->matrix[i][0] = nodes[i];

		tmpGraph->matrix[0][i] = nodes[i];

	}

	return tmpGraph;

}

Graph InsertEdge(Graph G, int a, int b) {

	int i, j;

	for (i = 0; i < G->size; i++) {

		if (a == G->node[i]) {

			break;

		}

	}

	for (j = 0; j < G->size; j++) {

		if (b == G->node[j]) {

			break;

		}

	}

	G->matrix[i + 1][j + 1]++;

	return G;

}

 

void DeleteGraph(Graph G) {

	int i;

	free(G->node);

	for (i = 0; i <= G->size; i++) {

		free(G->matrix[i]);

	}

	free(G->matrix);

	free(G);

}

