#ifndef __ArrayStack_H__
#define __ArrayStack_H__

#define TRUE 1
#define FALSE 0
#define STACK_LEN 100

typedef int Data;

typedef struct _arrayStack{
	Data stackArr[STACK_LEN];
	int topIdx;
}ArrayStack;

typedef ArrayStack Stack;

void StackInit(Stack *pStack);
int SIsEmpty(Stack *pStack);
void SPush(Stack * pStack, Data data);
Data SPop(Stack * pStack);
Data SPeek(Stack * pStack);
#endif // !__ArrayStack_H__
