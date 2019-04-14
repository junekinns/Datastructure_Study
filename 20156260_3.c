#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STACK_LEN 100
#define TRUE 1
#define FALSE 0
#define INF 2147483648
#define SWAP(a, b){int t = a; a = b; b = t;}
#define MAX(a, b)((a) > (b) ? (a) : (b))
//typedef, struct, Functions Declaration
typedef int bool;
typedef int Data;
typedef struct _stack {
	Data stackArr[STACK_LEN];
	int topIdx;
}Stack;
//stack 1 is full
int flag = 0;
int arr[10] = { 1 };

//Functions for Stack
void StackInit(Stack * pStack);
bool SIsEmpty(Stack * pStack);
void push(Stack *pStack, Data data);
Data pop(Stack * pStack);
Data peek(Stack * pStack);
//typedef, struct, Functions Declaration

void StackInit(Stack * pStack) {
	pStack->topIdx = -1;
}

bool SIsEmpty(Stack * pStack) {
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
	srand(time(NULL));
	int cnt = 0;
	int num = 0;
	while (cnt < 9) {
		int random = rand() % 10;
		if (arr[random] == 0) {
			arr[random] = 1;
			cnt++;
			num = num * 10 + random;
		}
	}
	printf("������ȣ : %d\n", num);
	//������ȣ ������
	int num2 = 0;
	while (num > 0) {
		num2 = num2 * 10 + num % 10;
		num /= 10;
	}
	//printf("%d",num2);

	Stack s1, s2;
	StackInit(&s1);
	StackInit(&s2);

	int f = 1;
	int outCnt = 0;
	cnt = 1;
	//��ǲ �ٵ��������� �ϴ� 1����� ����1�� �ٳ�������
	//k==f+1�̸� ����2�� �־��
	int cascade = 0;
	for (int i = 0; i < 9; i++) {
		int k = num2 % 10;
		num2 /= 10;
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
	//���� ����1���� ����2�� �ű�鼭 f�� ã��, �� ����2���� ����1�� �ű�鼭 f�� ã�´�.
	//��������? outCnt==9����
	while (outCnt < 9) {
		//����1->����2, f�� out, �ƴϸ� s2�� push
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
		//����2->����1
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
	printf("����(�� %dȸ)\n", cnt - 1);
	return 0;
}