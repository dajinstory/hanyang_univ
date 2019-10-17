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

 

Position SingleRotateWithLeft(Position);

Position SingleRotateWithRight(Position);

Position DoubleRotateWithLeft(Position);

Position DoubleRotateWithRight(Position);

 

AVLTree Insert(ElementType X, AVLTree T);

int Height(Position P);

 

void PrintInorder(AVLTree T);

void DeleteTree(AVLTree T);

 

void main(int argc, char* argv[]) {

	FILE *fp = fopen(argv[1], "r");

	AVLTree myTree = NULL;

	int num;

	while (fscanf(fp, "%d", &num) != EOF) {

		myTree = Insert(num, myTree);

		PrintInorder(myTree);

		printf("\n");

	}

	DeleteTree(myTree);

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

		T = (AVLTree)malloc(sizeof(struct AVLNode));

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

		if (Height(T->Left) - Height(T->Right) == 2) {

			if (X < T->Left->Element) {

				T = SingleRotateWithLeft(T);

			}

			else {

				T = DoubleRotateWithLeft(T);

			}

		}

	}

	else if (X > T->Element) {

		T->Right = Insert(X, T->Right);

		if (Height(T->Right) - Height(T->Left) == 2) {

			if (X > T->Right->Element) {

				T = SingleRotateWithRight(T);

			}

			else {

				T = DoubleRotateWithRight(T);

			}

		}

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

Position SingleRotateWithLeft(Position K2) {

	Position K1;

	K1 = K2->Left;

	K2->Left = K1->Right;

	K1->Right = K2;

 

	K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;

	K1->Height = Max(Height(K1->Left), Height(K2)) + 1;

	return K1;

}

Position SingleRotateWithRight(Position K2) {

	Position K1;

	K1 = K2->Right;

	K2->Right = K1->Left;

	K1->Left = K2;

 

	K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;

	K1->Height = Max(Height(K1->Right), Height(K2)) + 1;

	return K1;

}

Position DoubleRotateWithLeft(Position K3) {

	K3->Left = SingleRotateWithRight(K3->Left);

	return SingleRotateWithLeft(K3);

}

Position DoubleRotateWithRight(Position K3) {

	K3->Right = SingleRotateWithLeft(K3->Right);

	return SingleRotateWithRight(K3);

}

void DeleteTree(AVLTree T) {

 

	if (T->Left != NULL)

		DeleteTree(T->Left);

	if (T->Right != NULL)

		DeleteTree(T->Right);

	free(T);

}

