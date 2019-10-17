#include <stdio.h>

#include <stdlib.h>



typedef struct CircularQueue *Cqueue;

struct CircularQueue {

	int *key;

	int front;

	int rear;

	int qsize;

	int max_queue_size;

};

Cqueue MakeNewQueue(int max);

int IsEmpty(Cqueue Q);

int IsFull(Cqueue Q);

void Enqueue(Cqueue Q, int X);

void Dequeue(Cqueue Q);

void PrintFirst(Cqueue Q);

void PrintRear(Cqueue Q);

void DeleteQueue(Cqueue Q);

 

int main(int argc, char* argv[]) {

	FILE* fp;

	fp = fopen("lab4_input1.txt", "r");

	Cqueue myQ = NULL;

	char operationType;

	int tmp;

	while (fscanf(fp, " %c", &operationType) != EOF) {

		if (operationType == 'n') {

			fscanf(fp, "%d", &tmp);

			myQ = MakeNewQueue(tmp);

		}

		else if (operationType == 'e') {

			fscanf(fp, "%d", &tmp);

			Enqueue(myQ, tmp);

		}

		else if (operationType == 'd') {

 

		}

		else if (operationType == 'f') {

			PrintFirst(myQ);

		}

		else if (operationType == 'r') {

			PrintRear(myQ);

		}

 

	}

	return 0;

}

Cqueue MakeNewQueue(int max) {

	Cqueue L;

	L = (Cqueue)malloc(sizeof(struct CircularQueue));

	L->key = (int*)malloc(sizeof(int)*max);

	L->max_queue_size = max;

	L->qsize = 0;

	L->front = 1;

	L->rear = 0;

	return L;

}

int IsEmpty(Cqueue Q) {

	if (Q->qsize == 0) {

		return 1;

	}

	else {

		return 0;

	}

}

int IsFull(Cqueue Q) {

	if (Q->max_queue_size == Q->qsize) {

		return 1;

	}

	else {

		return 0;

	}

}

void Enqueue(Cqueue Q, int X) {

	if (IsFull(Q)) {

		printf("Full queue\n");

	}

	else {

		Q->qsize++;

		Q->rear = (Q->rear + 1) % (Q->max_queue_size);

		Q->key[Q->rear] = X;

	}

}

void Dequeue(Cqueue Q) {

 

}

void PrintFirst(Cqueue Q) {

	printf("%d\n", Q->key[Q->front]);

}

void PrintRear(Cqueue Q) {

	printf("%d\n", Q->key[Q->rear]);

}

void DeleteQueue(Cqueue Q) {

 

}

