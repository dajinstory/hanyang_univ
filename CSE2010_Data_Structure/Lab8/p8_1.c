#include <stdio.h>

#include <stdlib.h>

struct AVLNode;

typedef struct AVLNode *Position;

typedef struct AVLNode *AVLTree;

typedef int ElementType;

 

struct AVLNode {

	ElementType Element;

	AVLTree Left;

	AVLTree Right;

	int Height;

};

 

Position SingleRotateWithLeft(Position node);

Position SingleRotateWithRight(Position node);

Position DoubleRotateWithLeft(Position node);

Position DoubleRotateWithRight(Position node);

AVLTree Insert(ElementType X, AVLTree T);

void PrintInorder(AVLTree T);

int Height(Position P);

void main(int argc, char* argv[]) {

	FILE *fp = fopen(argv[1], "r");

	AVLTree myTree = NULL;

	int element, num;

	while (fscanf(fp, "%d", &num) != EOF) {

		myTree = Insert(num, myTree);

		PrintInorder(myTree);

		printf("\n");

	}

	//DeleteTree;

	fclose(fp);

}

int Height(Position P) {

	if (P == NULL) {

		return -1;

	}

	else {

		return P->Height;

	}

}

int Max(int a, int b) {

	if (a >= b) {

		return a;

	}

	else {

		return b;

	}

}

AVLTree Insert(ElementType X, AVLTree T) {

	if (T == NULL) {

		T = (struct AVLNode*)malloc(sizeof(struct AVLNode));

		if (T == NULL) {

			printf("FatalError_Out of space!!!\n");

			return T;

		}

		else {

			T->Element = X;

			T->Height = 0;

			T->Left = T->Right = NULL;

		}

	}

	else if (X < T->Element) {

		T->Left = Insert(X, T->Left);

	

	}

	else if (X > T->Element) {

		T->Right = Insert(X, T->Right);

 

	}

	T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

	return T;

}

void PrintInorder(AVLTree T) {

	if (T->Left != NULL) {

		PrintInorder(T->Left);

	}

	printf("%d(%d) ", T->Element, Height(T));

	if (T->Right != NULL) {

		PrintInorder(T->Right);

	}

}
