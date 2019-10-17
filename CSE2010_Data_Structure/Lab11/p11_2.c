#define INF 9999

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

 

typedef struct Node {

	int vertex;

	int dist; //distance

	int prev;

}Node;

 

typedef struct Graph {

	int size; // Vertex 개수

	int** vertices; // size+1 * size+1

	Node* nodes; // size+1

}Graph;

 

typedef struct Heap {

	int Capacity;

	int Size;

	Node* Element; // Capacity

}Heap;

 

Graph CreateGraph(int size);

void printShortestPath(Graph g);

void printEachRoute(Graph g, int index);

Heap* createMinHeap(int heapSize);

void insertToMinHeap(Heap* minHeap, int vertex, int distance);

Node deleteMin(Heap* minHeap);

bool isEmpty(Heap* minHeap);

bool isFull(Heap* minHeap);

 

void main(int argc, char* argv[]) {

	FILE* fi = fopen(argv[1], "r");

	Graph g;

	int size, index;

	fscanf(fi, "%d\n", &size);

	g = CreateGraph(size + 1);

	char temp = 0;

	while (temp != '\n') {

		int node1, node2, weight;

		fscanf(fi, "%d-%d-%d", &node1, &node2, &weight);

		g.vertices[node1][node2] = weight;

		temp = fgetc(fi);

	}

	printShortestPath(g);

	free(g.nodes);

	for(index = 0; index <= g.size; index++){
	
		free(g.vertices[index]);
	}
	
	free(g.vertices);
	
	fclose(fi);
}

 

Graph CreateGraph(int size) {

	int index_i, index_j;

	Graph tmpGraph;

	tmpGraph.size = size - 1;

	tmpGraph.vertices = (int**)malloc(sizeof(int*)*(size));

	for (index_i = 0; index_i < size; index_i++) {

		tmpGraph.vertices[index_i] = (int*)malloc(sizeof(int)*(size));

		for (index_j = 0; index_j < size; index_j++) {

			tmpGraph.vertices[index_i][index_j] = 0;

		}

	}

	tmpGraph.nodes = (Node*)malloc(sizeof(struct Node)*(size));

	return tmpGraph;

}

 

void printShortestPath(Graph g) {

	int index, vertex, dist, prev;

	Node tmpNode;

	Heap* minHeap = createMinHeap((g.size)*(g.size));

	for (index = 1; index <= g.size; index++) {

		tmpNode.vertex = index;

		tmpNode.dist = INF;

		tmpNode.prev = -1;

		g.nodes[index] = tmpNode;

	}

	insertToMinHeap(minHeap, 1, 0);

	g.nodes[1].dist = 0;

	while (!isEmpty(minHeap)) {

		tmpNode = deleteMin(minHeap);

		vertex = tmpNode.vertex;

		dist = tmpNode.dist;

		if (dist <= g.nodes[vertex].dist) {

			g.nodes[vertex].dist = dist;

			for (index = 1; index <= g.size; index++) {

				if (g.vertices[vertex][index] != 0) {

					if (dist + g.vertices[vertex][index] < g.nodes[index].dist) {

						g.nodes[index].dist = dist + g.vertices[vertex][index];

						g.nodes[index].prev = vertex;

						insertToMinHeap(minHeap, index, dist + g.vertices[vertex][index]);

					}

				}

			}

		}

	}

	for (index = 2; index <= g.size; index++) {

		printEachRoute(g, index);

		printf("%d (cost : %d)\n", g.nodes[index].vertex, g.nodes[index].dist);

	}

	free(minHeap->Element);

}

 

Heap* createMinHeap(int heapSize) {

	Heap* tmpHeap;

	tmpHeap = (Heap*)malloc(sizeof(struct Heap));

	tmpHeap->Capacity = heapSize;

	tmpHeap->Size = 0;

	tmpHeap->Element = (Node*)malloc(sizeof(struct Node) * heapSize);

	return tmpHeap;

}

 

void insertToMinHeap(Heap* minHeap, int vertex, int distance) {

	int index;

	if (isFull(minHeap)) {

		printf("Insertion Error : Min Heap is full\n");

		return;

	}

	minHeap->Size++;

	for (index = minHeap->Size; index > 1 && minHeap->Element[index / 2].dist > distance; index /= 2) {

		minHeap->Element[index] = minHeap->Element[index / 2];

	}

	minHeap->Element[index].vertex = vertex;

	minHeap->Element[index].dist = distance;

}

 

Node deleteMin(Heap* minHeap) {

	int index, Child;

	Node MinElement, LastElement;

	MinElement = minHeap->Element[1];

	LastElement = minHeap->Element[minHeap->Size--];

 

	for (index = 1; index * 2 <= minHeap->Size; index = Child) {

		Child = index * 2;

		if (Child != minHeap->Size && minHeap->Element[Child + 1].dist < minHeap->Element[Child].dist) {

			Child++;

		}

		if (LastElement.dist > minHeap->Element[Child].dist) {

			minHeap->Element[index] = minHeap->Element[Child];

		}

		else {

			break;

		}

	}

	minHeap->Element[index].dist = LastElement.dist;

	minHeap->Element[index].vertex = LastElement.vertex;

	return MinElement;

}

 

bool isEmpty(Heap* minHeap) {

	if (minHeap->Size == 0) {

		return true;

	}

	else {

		return false;

	}

}

 

bool isFull(Heap* minHeap) {

	if (minHeap->Size == minHeap->Capacity) {

		return true;

	}

	else {

		return false;

	}

}

void printEachRoute(Graph g, int index) {

	if (g.nodes[index].prev != -1) {

		printEachRoute(g, g.nodes[index].prev);

		printf("%d->", g.nodes[index].prev);

	}

}
