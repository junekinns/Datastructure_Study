#include <stdio.h>
#define STACK_LEN 100
#define TRUE 1
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
#define TRAIN_NUM 9
//typedef, struct, Functions Declaration
typedef int Data;
typedef struct _stack {
	Data stackArr[STACK_LEN];
	int topIdx;
}Stack;

//Functions for Stack
void StackInit(Stack * pStack);
int SIsEmpty(Stack * pStack);
void push(Stack *pStack, Data data);
Data pop(Stack * pStack);
Data peek(Stack * pStack);
//typedef, struct, Functions Declaration

void StackInit(Stack * pStack) {
	pStack->topIdx = -1;
}

int SIsEmpty(Stack * pStack) {
	if (-1 == pStack->topIdx) return TRUE;
	else return FALSE;
}

void push(Stack *pStack, Data data) {
	if (STACK_LEN == pStack->topIdx) {
		printf("Stack Overflow!\n");
		return;
	}
	pStack->stackArr[++(pStack->topIdx)] = data;
}

Data pop(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack Underflow!\n");
		return 0;
	}
	return pStack->stackArr[(pStack->topIdx)--];
}

Data peek(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack is empty!\n");
		return 0;
	}
	return pStack->stackArr[(pStack->topIdx)];
}

int main() {
	int cnt, num;
	int train_num = 0;
	printf("열차번호 입력 : ");
	scanf("%d", &num);
	//편의를 위해 입력받은 num을 reverse 시켜 train_num에 대입.
	while (num > 0) {
		train_num = train_num * 10 + num % 10;
		num /= 10;
	}
	//printf("%d",train_num);

	Stack s1, s2;
	StackInit(&s1);
	StackInit(&s2);

	int f = 1;
	int outCnt = 0;
	cnt = 1;
	//인풋 다들어갈때까지는 일단 1번 열차를 제외하고 스택1에 모두 Push
	//k==f+1이면 스택2에 넣어둠
	int cascade = 0;
	for (int i = 0; i < 9; i++) {
		int k = train_num % 10;
		train_num /= 10;
		printf("%02d : IN(%d)\n", cnt++, k);
		if (k == f) {
			printf("%02d : OUT(%d)\n", cnt++, k);
			outCnt++;
			f++;
			if (cascade == 1) {
				//pop
				pop(&s2);
				printf("%02d : POP(%d)\n", cnt++, 2);
				printf("%02d : OUT(%d)\n", cnt++, k + 1);
				outCnt++;
				f++;
				cascade = 0;
			}
		}
		else if (k == f + 1) {
			push(&s2, k);
			cascade = 1;
			printf("%02d : PUSH(%d,%d)\n", cnt++, 2, k);
		}
		else {
			push(&s1, k);
			printf("%02d : PUSH(%d,%d)\n", cnt++, 1, k);
		}
	}
	//이제 스택1에서 스택2로 옮기면서 f를 찾고, 또 스택2에서 스택1로 옮기면서 f를 찾는다.
	//언제까지? outCnt==9까지
	while (outCnt < 9) {
		//스택1->스택2, f면 out, 아니면 s2로 push
		while (!SIsEmpty(&s1)) {
			int k = pop(&s1);
			printf("%02d : POP(%d)\n", cnt++, 1);
			if (k == f) {
				//out
				printf("%02d : OUT(%d)\n", cnt++, k);
				outCnt++;
				f++;
			}
			else {
				//push
				printf("%02d : PUSH(%d,%d)\n", cnt++, 2, k);
				push(&s2, k);
			}
		}
		//스택2->스택1
		while (!SIsEmpty(&s2)) {
			int k = pop(&s2);
			printf("%02d : POP(%d)\n", cnt++, 2);
			if (k == f) {
				//out
				printf("%02d : OUT(%d)\n", cnt++, k);
				outCnt++;
				f++;
			}
			else {
				//push
				printf("%02d : PUSH(%d,%d)\n", cnt++, 1, k);
				push(&s1, k);
			}
		}
	}
	printf("종료(총 %d회)\n", cnt - 1);
	return 0;
}