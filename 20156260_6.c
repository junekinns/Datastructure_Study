#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRUE 1 
#define FALSE 0
#define INF 2147483648
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
#define MAX_STUDENT_NUM 1000
#define STACK_LEN ((MAX_STUDENT_NUM*2) + (2))
//typedef, struct, Functions Declaration
typedef int bool;
typedef int Data;
typedef struct _stack {
	Data stackArr[STACK_LEN];
	int topIdx;
}Stack;

//Functions for Stack
void StackInit(Stack * pStack);
bool SIsEmpty(Stack * pStack);
void push(Stack *pStack, Data data);
Data pop(Stack * pStack);
Data peek(Stack * pStack);
//Functions for generating random data(Student Number)
int studentNumGenerator();
//Table for memoization to avoid overlap
int studentNumTable[1000000];
int compare(const void *a, const void *b);
void QuickSort_Recur(int arr[], int left, int right);
void QuickSort_NonRecur(int arr[], int len);
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
		return;
	}
	return pStack->stackArr[(pStack->topIdx)--];
}
Data peek(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack is empty!\n");
		return;
	}
	return pStack->stackArr[(pStack->topIdx)];
}

//��ȭ��ȣ : 010���� �����ϴ� 11�ڸ� ����
//�й� : 13-19�⵵�� ������ �л��� 9�ڸ� ����
int studentNumGenerator() {
	int sNum = 2010;
	while (TRUE) {
		sNum += rand() % 7 + 3;
		for (int i = 0; i < 5; i++) {
			sNum *= 10;
			sNum += rand() % 10;
		}
		if (0 == studentNumTable[sNum % 100000]) { //�ߺ�����
			studentNumTable[sNum % 100000]++;
			break;
		}
		else {
			sNum = 2010;
			continue;
		}
	}
	return sNum;
}

//qsort �Լ������� ���ڿ� �ʿ��� compare�Լ�
int compare(const void *a, const void *b)
{
	return *(int *)a - *(int *)b; //�������� ����, ù �Ķ���Ͱ� �� ũ�� ���, ������ 0, �ι�° �Ķ���Ͱ� �� ũ�� ����(strcmp�� ������ ����)
}

int Partition(int arr[], int left, int right) {
	int pivot = arr[left];
	int low = left + 1;
	int high = right;

	while (low <= high) {
		while (pivot >= arr[low] && low <= right) {
			low++;
		}
		while (pivot <= arr[high] && high >= left + 1) {
			high--;
		}
		if (low <= high)
			SWAP(arr[low], arr[high]);
	}
	SWAP(arr[left], arr[high]);
	return high;
}

void QuickSort_Recur(int arr[], int left, int right) {
	if (left <= right) {
		int pivot = Partition(arr, left, right);
		QuickSort_Recur(arr, left, pivot - 1);
		QuickSort_Recur(arr, pivot+1, right);
	}
}

void QuickSort_NonRecur(int arr[], int len) {
	Stack callStack;
	StackInit(&callStack);
	int stackCount = 0, maxStackCount = 0;
	int pivot, left, right, low, high;
	right = len - 1;
	left = 0;
	push(&callStack, right);
	push(&callStack, left);
	while (!SIsEmpty(&callStack)) {
		left = pop(&callStack);
		right = pop(&callStack);
		if (right - left + 1 > 1) { //������(left~right)�� �迭�� ���̰� 1���� ũ��, �� ������ �������� ������ 1���� ���� ������
			pivot = arr[right];
			low = left - 1;
			high = right;
			
			//��ͷ� ������ �����İ� ���� ������ �κ�, low�� ������, high�� ������, �����ϸ� Ż��
			while (TRUE)
			{
				while (arr[++low] < pivot);
				while (arr[--high] > pivot);
				if (low >= high) break;
				SWAP(arr[low], arr[high]);
			}
			SWAP(arr[low], arr[right]); //ó���Ǻ�(right) �� low�� ��ȯ

			push(&callStack, right);
			push(&callStack, low + 1); //���� �����Ŀ��� �ι�° ���ȣ��(�Ǻ��� �޺κ�)
			push(&callStack, low - 1); //���� �����Ŀ��� ù��° ���ȣ��(�Ǻ��� �պκ�)
			push(&callStack, left);
			stackCount = callStack.topIdx;
			maxStackCount = maxStackCount < stackCount ? stackCount : maxStackCount;
		}
	}
	printf("Stack Count : %d\n", maxStackCount);
}
//Program for comparing 2 different Quicksort. QuickSort(recursive) and QuickSort(non-recursive)
int main() {
	int i;
	int unsortedStuNum[MAX_STUDENT_NUM] = { 0 };
	int sortStuNum[MAX_STUDENT_NUM] = { 0 };
	double cpuTimes[2] = { 0 };
	clock_t start_t, end_t; double duration_t;

	srand(time(NULL));
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		//Generate 1000 studentnumber data
		unsortedStuNum[i] = studentNumGenerator();
	}
	//Sort by Studentnumber Comapare
	//Quick Sort(Recursive)
	memcpy(sortStuNum, unsortedStuNum, sizeof(unsortedStuNum));
	printf("QuickSort(Recursive) by StudentNumber Start!!\n");
	system("pause");
	start_t = clock();
	QuickSort_Recur(sortStuNum, 0, MAX_STUDENT_NUM - 1);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("%d\n", sortStuNum[i]);
	}
	printf("Recursive Quicksort time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[0] = duration_t;

	//Quick Sort(Non - Recursive)
	memcpy(sortStuNum, unsortedStuNum, sizeof(unsortedStuNum));
	printf("QuickSort(Non-Recursive) by StudentNumber Start!!\n");
	system("pause");
	start_t = clock();
	QuickSort_NonRecur(sortStuNum, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("%d\n", sortStuNum[i]);
	}
	printf("Non-Recursive Quicksort time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[1] = duration_t;

	return 0;
}