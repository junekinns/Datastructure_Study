#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#define TRUE 1 
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))

int main() {
	Node * pHead = NULL;
	add_first(&pHead, 10);
	add_first(&pHead, 20);
	add_first(&pHead, 30);
	add_first(&pHead, 40);
	add_first(&pHead, 50);
	add_first(&pHead, 60);
	add_first(&pHead, 70);
	display(pHead);
	clear(pHead);
	pHead = NULL;
	return 0;
}