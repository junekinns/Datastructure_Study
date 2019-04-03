#include <stdio.h>
#include "ArrayStack.h"

void StackInit(Stack *pStack) {
	pStack->topIdx = -1;
}

int SIsEmpty(Stack *pStack) {
	if (-1 == pStack->topIdx) return TRUE;
	else return FALSE;
}

void SPush(Stack * pStack, Data data) {
	pStack->stackArr[++(pStack->topIdx)] = data;
}

Data SPop(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack Underflow!");
		exit(-1);
	}
	return pStack->stackArr[(pStack->topIdx)--];
}

Data SPeek(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack Underflow!");
		exit(-1);
	}
	return pStack->stackArr[(pStack->topIdx)];
}