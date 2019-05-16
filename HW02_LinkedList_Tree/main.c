#include <stdio.h>
#include <stdlib.h>
#include "SinglyLinkedList.h"
#define TRUE 1 
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))

int main() {
	Node * pHead = NULL;
	add_first(&pHead, 10);
	add_first(&pHead, 60);
	add_first(&pHead, 30);
	add_first(&pHead, 40);
	add_first(&pHead, 560);
	add_first(&pHead, 60);
	add_first(&pHead, 70);
	add_last(pHead, 99);
	add_last(pHead, 98);
	add_last(pHead, 96);
	add_last(pHead, 92);
	add_last(pHead, 77);
	add_last(pHead, 66);
	display(pHead);
	clear(pHead);
	pHead = NULL;
	return 0;
}