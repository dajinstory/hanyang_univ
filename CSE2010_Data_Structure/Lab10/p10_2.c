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

int Dequeue(Queue Q);

void Enqueue(Queue Q, int X);

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

 

	// txt 파일의 첫번째 줄을 fgets 함수로 한번에 char형 배열인 read 에 저장하고, 숫자단위로 끊어서 int형 배열인 numbers에 저장

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

 

	//numbers배열의 크기가 numbers_size에 저장되어 있는데 numbers_size+1만큼의 길이의 포인터 배열을 만든다

	//포인터 배열 nodes의 0번째 값에는 numbers_size 값이 들어가고, 1번째부터 numbers_size번째 까지는 numbers 배열안의 숫자들이 전부 차례대로 들어간다.

	nodes = (int*)malloc(sizeof(int)*(numbers_size + 1));

	nodes[0] = numbers_size;

	for (index = 0; index < numbers_size; index++) {

		nodes[index + 1] = numbers[index];

	}

 

	//해당 포인터배열을 가지고 topGraph를 만들어준다.

	topGraph = CreateGraph(nodes);

 

	//두번째 줄의 값들을 가져온다. 그 후 edge를 추가해준다.

	while (fscanf(fi, "%d-%d ", &a, &b) != EOF) {

		topGraph = InsertEdge(topGraph, a, b);

	}

 

	//lab 에서 출력하기 위한 함수
	/*
	for (i = 0; i <= topGraph->size+1; i++) {

		for (j = 0; j <= topGraph->size; j++) {

			if (i == 0 && j == 0) {

				printf("  ");

				continue;

			}

			printf("%d ", topGraph->matrix[i][j]);

		}

		printf("\n");

	}
	*/
	

	//Topological Sort 실행한다.

	Topsort(topGraph);



	//free

	free(nodes);

	DeleteGraph(topGraph);

	fclose(fi);

	return 0;

}

 

//size = 개수

// node 0부터 size-1 까지

// matrix size+2 * size+1

Graph CreateGraph(int* nodes) {

	Graph tmpGraph;

	tmpGraph = (Graph)malloc(sizeof(struct _Graph));

	int i, j;

	tmpGraph->size = nodes[0];

	tmpGraph->node = (int*)malloc(sizeof(int)*(tmpGraph->size));

 

	for (i = 0; i < tmpGraph->size; i++) {

		tmpGraph->node[i] = nodes[i + 1];

	}

 

	tmpGraph->matrix = (int**)malloc(sizeof(int*)*(tmpGraph->size + 2));

	for (i = 0; i <= tmpGraph->size+1; i++) {

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

	G->matrix[G->size + 1][j + 1]++;

	return G;

}

 

void DeleteGraph(Graph G) {

	int i;

	free(G->node);

	for (i = 0; i <= G->size+1; i++) {

		free(G->matrix[i]);

	}

	free(G->matrix);

	free(G);

}

 

void Topsort(Graph G) {

	//Queue를 Max_ARR_SIZE 크기로 만들어 준다.

	Queue topQueue = MakeNewQueue(MAX_ARR_SIZE);

	int V, W, i, j;
		
	

	//처음에 indegree값이 0인 노드를 넣기(정확히는 노드들의 index값 넣기)

	for (i = 1; i <= G->size; i++) {

		if (G->matrix[G->size + 1][i] == 0) {

			Enqueue(topQueue, i);

			//넣고 indegree값 -1 해준다. (indegree값이 -1이면 이미 큐에 넣었다고 확인하기 위해)
			
			G->matrix[G->size + 1][i]--;

		}

	}

 

	while (!IsEmpty(topQueue)) {

		V = Dequeue(topQueue);
		
		//큐에서 삭제한 노드의 index값(indegree값 확인하기 위한 배열에서의). 을 V에 저장한다.
		
		//V를 출발점으로 하고 j를 도착점으로 하는 간선들을 지운다(여기서 V와 j는 모두 index값).

		//j의 indegree를 1 줄여준다.
		for (j = 1; j <= G->size; j++) {

			if (G->matrix[V][j] == 1) {

				G->matrix[V][j] = 0;

				G->matrix[G->size + 1][j]--;

			}

		}

 

		/*

		for (i = 0; i <= G->size + 1; i++) {

			for (j = 0; j <= G->size; j++) {

				if (i == 0 && j == 0) {

					printf("\n  ");

					continue;

				}

				printf("%d ", G->matrix[i][j]);

			}

			printf("\n");

		}*/

 

		printf("%d ", G->node[V-1]);

		for (i = 1; i <= G->size; i++) {

			if (G->matrix[G->size + 1][i] == 0) {

				Enqueue(topQueue, i);

				G->matrix[G->size + 1][i]--;

			}

		}

	}

	DeleteQueue(topQueue);
	
	printf("\n");

}

 

Queue MakeNewQueue(int X) {

	Queue L;

	L = (Queue)malloc(sizeof(struct _Queue));

	L->key = (int*)malloc(sizeof(int)*X);

	L->max_queue_size = X;

	L->qsize = 0;

	L->first = 1;

	L->rear = 0;

	return L;

}

 

int IsEmpty(Queue Q) {

	if (Q->qsize == 0) {

		return 1;

	}

	else {

		return 0;

	}

}

 

int IsFull(Queue Q) {

	if (Q->max_queue_size == Q->qsize) {

		return 1;

	}

	else {

		return 0;

	}

}

 

int Dequeue(Queue Q) {

	int DeletedNode;

	if (IsEmpty(Q)) {

		printf("Dequeue Failed : Queue is empty!\n");

		return -1;

	}

	DeletedNode = Q->key[Q->first];

	Q->first = (Q->first + 1) % (Q->max_queue_size);

	Q->qsize--;

	return DeletedNode;

}

 

void Enqueue(Queue Q, int X) {

	if (IsFull(Q)) {

		printf("Full queue\n");

	}

	else {

		Q->qsize++;

		Q->rear = (Q->rear + 1) % (Q->max_queue_size);

		Q->key[Q->rear] = X;

	}

}

 

void DeleteQueue(Queue Q) {

	free(Q->key);

	free(Q);

}

 

void MakeEmpty(Queue Q) {

 

}

