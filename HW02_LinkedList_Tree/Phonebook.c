#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Phonebook.h"
#define TRUE 1
#define FALSE 0

Node * findWithName(Node * list, const char * name)
{
	return NULL;
}

void add_first(Node ** list, Data item) {
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->data = item;
	if (NULL == *list) {
		*list = newNode;
		newNode->link = NULL;
	}
	else {
		newNode->link = *list;
		*list = newNode;
	}
}

Data deleteNode(Node * list, Node * pos) {
	Node * pNode = list;
	Data delData = pos->data;
	while (pos != pNode->link) {
		pNode = pNode->link;
	}
	pNode->link = pos->link;
	free(pos);
	return delData;
}

void sortList(Node ** list)
{
}

void display(Node * list) {
	Node * pNode = list;
	while (NULL != pNode) {
		//printf("%d -> ", pNode->data);
		pNode = pNode->link;
	}
	printf("NULL\n");
}

