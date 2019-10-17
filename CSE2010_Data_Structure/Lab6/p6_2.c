#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

 

typedef struct Tree {

	int value;

	struct Tree *left;

	struct Tree *right;

}Tree;

 

Tree* insertNode(Tree *root, int key);

Tree* deleteNode(Tree *root, int key, bool IsFirst);

Tree* findNode(Tree *root, int key);

Tree* findMin(Tree *root);

void deleteAll(Tree *root);

void printInorder(Tree* root);

void printPreorder(Tree* root);

void printPostorder(Tree* root);

void main(int argc, char* argv[]) {

	FILE *fi = fopen(argv[1], "r");

	char cv;

	int key;

	Tree* root = NULL;

	while (fscanf(fi, "%c", &cv) != EOF) {

		switch (cv) {

		case 'i':

			fscanf(fi, "%d", &key);

			root = insertNode(root, key);

			break;

		case 'd':

			fscanf(fi, "%d", &key);

			deleteNode(root, key, 1);

			break;

		case 'f':

			fscanf(fi, "%d", &key);

			findNode(root, key);

			break;

		case 'p':

			fscanf(fi, "%c", &cv);

			if (cv == 'i') {

				printInorder(root);

			}

			else if (cv == 'r') {

				printPreorder(root);

			}

			else if (cv == 'o') {

				printPostorder(root);

			}

			printf("\n");

			break;

		}

	}

	deleteAll(root);

	fclose(fi);

}

Tree* insertNode(Tree *root, int key) {

	if (root == NULL) {

		root = (Tree*)malloc(sizeof(Tree));

		if (root == NULL) {

			printf("FatalError! Out of space!!!\n");

		}

		else {

			printf("insert %d\n", key);

			root->value = key;

			root->left = NULL;

			root->right = NULL;

		}

	}

	else if (key < root->value) {

		root->left = insertNode(root->left, key);

	}

	else if (key > root->value) {

		root->right = insertNode(root->right, key);

	}

	else {

		printf("Insertion Error : There is already %d in the tree.\n", key);

	}

	return root;

}

Tree* deleteNode(Tree *root, int key, bool IsFirst) {

	Tree *TmpCell;

	if (root == NULL) {

		printf("Deletion error : %d is not in the tree.\n", key);

	}

	else if (key < root->value) {

		root->left = deleteNode(root->left, key, IsFirst);

	}

	else if (key > root->value) {

		root->right = deleteNode(root->right, key, IsFirst);

	}

	else if (root->left && root->right) {

		printf("Delete %d\n", key);

		TmpCell = findMin(root->right);

		root->value = TmpCell->value;

		root->right = deleteNode(root->right, root->value, 0);

	}

	else {

		if (IsFirst) {

			printf("Delete %d\n", key);

		}

		TmpCell = root;

		if (root->left == NULL) {

			root = root->right;

		}

		else if (root->right == NULL) {

			root = root->left;

		}

		free(TmpCell);

	}

	return root;

}

Tree* findNode(Tree *root, int key) {

	if (root == NULL) {

		printf("%d is not in the tree.\n", key);

		return NULL;

	}

	else if (key < root->value) {

		return findNode(root->left, key);

	}

	else if (key > root->value) {

		return findNode(root->right, key);

	}

	else {

		printf("%d is in the tree.\n", key);

		return NULL;

	}

}

Tree* findMin(Tree *root) {

	if (root->left == NULL) {

		return root;

	}

	else {

		return findMin(root->left);

	}

}

void deleteAll(Tree *root){

	if(root->left != NULL) {

		deleteAll(root->left);

	}

	else if(root->right != NULL){

		deleteAll(root->right);

	}

	free(root);

}

void printInorder(Tree* root) {

	if (root->left != NULL) {

		printInorder(root->left);

	}

	printf("%d ", root->value);

	if (root->right != NULL) {

		printInorder(root->right);

	}

}

void printPreorder(Tree* root) {

	printf("%d ", root->value);

	if (root->left != NULL) {

		printPreorder(root->left);

	}

	if (root->right != NULL) {

		printPreorder(root->right);

	}

}

void printPostorder(Tree* root) {

	if (root->left != NULL) {

		printPostorder(root->left);

	}

	if (root->right != NULL) {

		printPostorder(root->right);

	}

	printf("%d ", root->value);

}
