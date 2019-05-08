#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

void add_last(Node * list, Data item) {
	Node * p = list;
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->data = item;
	newNode->link = NULL;
	while (NULL != p->link)
		p = p->link;
	p->link = newNode;
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

void add(Node ** list, Node * pos, Data item) {
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->data = item;
	newNode->link = NULL;
	if (NULL == *list) {
		*list = newNode;
		newNode->link = NULL;
	}
	else {
		newNode->link = *list;
		*list = newNode;
	}
}

Data delete(Node * list, Node * pos) {
	Node * pNode = list;
	Data delData = pos->data;
	while (pos != pNode->link) {
		pNode = pNode->link;
	}
	pNode->link = pos->link;
	free(pos);
	return delData;
}

void clear(Node * list) {
	Node * pNode = list;
	while (NULL != pNode) {
		Node * delNode = pNode;
		pNode = pNode->link;
		free(delNode);
	}
}

void replace(Node ** list, Node * pos, Data item) {
	pos->data = item; //이게 끝 아닌가?
}

int is_in_list(Node ** list, Data item) {
	Node * pNode = *list;
	while (item != pNode->data) {
		if (NULL == pNode) {
			return FALSE;
		}
		pNode = pNode->link;
	}
	return TRUE;
}

Data get_entry(Node ** list, Node *pos) {

}

int get_length(Node * list) {
	Node * pNode = list;
	int cnt = 0;
	while (NULL != pNode) {
		cnt++;
		pNode = pNode->link;
	}
	return cnt;
}

int is_empty(Node * list) {
	if (NULL == list)
		return TRUE;
	return FALSE;
}

int is_full(Node * list) {
	//full???, list에 MAX_LENGTH와 같은 제한이 필요함
}

void display(Node * list) {
	Node * pNode = list;
	while (NULL != pNode) {
		printf("%d -> ", pNode->data);
		pNode = pNode->link;
	}
	printf("NULL\n");
}