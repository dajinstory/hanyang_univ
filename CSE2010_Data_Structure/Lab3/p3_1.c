#include <stdio.h>
#include <stdlib.h>
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
struct Node{
	ElementType element;
	Position next;
};
List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
Position Find(ElementType P, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
int main(int argc, char* argv[]){
	FILE* fp;
	List LinkedList = NULL;
	char operationType;
	ElementType tmp, place;
	fp=fopen("lab3_input.txt","r");
	LinkedList = MakeEmpty(LinkedList);
	while(fscanf(fp,"%c",&operationType)!=EOF){
		if(operationType == 'i'){
			fscanf(fp,"%d%d",&tmp,&place);
			if(place==-1){
				Insert(tmp,LinkedList,LinkedList);
			}
			else{
				Insert(tmp,LinkedList,Find(place,LinkedList));
			}
		}
		else if(operationType == 'p'){
			PrintList(LinkedList);
		}
	}
	fclose(fp);
	return 0;
}
List MakeEmpty(List L){
	L=(List)malloc(sizeof(struct Node));
	L->next=(List)malloc(sizeof(struct Node));
	L->element=0;
	L->next = NULL;
	return L;
}
int IsEmpty(List L){
	return L->next == NULL;
}
int IsLast(Position P, List L){
	return P->next == NULL;
}
Position Find(ElementType X, List L){
	Position P;
	P=L->next;
	while(P!=NULL && P->element !=X){
		P=P->next;
	}
	return P;
}
Position FindPrevious(ElementType X, List L){
	Position P;
	P=L;
	while(P->next != NULL && P->next->element != X){
		P=P->next;
	}
	return P;
}
void Insert(ElementType X, List L, Position P){
	Position TmpCell;
	TmpCell = (Position)malloc(sizeof(struct Node));
	if(TmpCell==NULL){
		printf("Out of space");
		return;
	}
	TmpCell->element = X;
	TmpCell->next = P->next;
	P->next=TmpCell;
}
void PrintList(List L){
	L=L->next;
	while(L!=NULL){
		printf("%d ", L->element);
		L=L->next;
	}
	printf("\n");
}

