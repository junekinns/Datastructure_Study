#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRUE 1 
#define FALSE 0
#define INF 2147483648
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
#define MAX_STUDENT_NUM 20000
#define STACK_LEN ((MAX_STUDENT_NUM*2) + (2))
//typedef, struct, Functions Declaration
typedef int bool;
typedef struct _student {
	int studentNum; //학번 : 13-19년도에 입학한 학생의 9자리 숫자
	int phoneNum; //전화번호 : 010으로 시작하는 11자리 숫자
	char name[11]; //이름 : 10자 크기의 영문자 임의의 문자
}STUDENT;
typedef STUDENT Data;
//Tables for memoization to avoid overlap
int phoneNumTable[100000000];
int studentNumTable[1000000];
//Functions for generating random data(Phone Number, Student Number, Name)
int phoneNumGenerator();
int studentNumGenerator();
void nameGenerator(char * nameArr, int len);
int compare(const void *a, const void *b);
void QuickSort_NonRecur(Data arr[], int len);

//Functions for Stack
typedef struct _stack {
	int stackArr[STACK_LEN];
	int topIdx;
}Stack;

void StackInit(Stack * pStack);
bool SIsEmpty(Stack * pStack);
void push(Stack *pStack, int data);
int pop(Stack * pStack);
int peek(Stack * pStack);
//typedef, struct, Functions Declaration

void StackInit(Stack * pStack) {
	pStack->topIdx = -1;
}
bool SIsEmpty(Stack * pStack) {
	if (-1 == pStack->topIdx) return TRUE;
	else return FALSE;
}
void push(Stack *pStack, int data) {
	if (STACK_LEN == pStack->topIdx) {
		printf("Stack Overflow!\n");
		return;
	}
	pStack->stackArr[++(pStack->topIdx)] = data;
}
int pop(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack Underflow!\n");
		return;
	}
	return pStack->stackArr[(pStack->topIdx)--];
}
int peek(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack is empty!\n");
		return;
	}
	return pStack->stackArr[(pStack->topIdx)];
}

//전화번호 : 010으로 시작하는 11자리 숫자
int phoneNumGenerator() {
	int pNum = 10;
	while (TRUE) {
		for (int i = 0; i < 8; i++) {
			pNum *= 10;
			pNum += rand() % 10;
		}
		if (0 == phoneNumTable[pNum % 100000000]) { //중복방지
			phoneNumTable[pNum % 100000000]++;
			break;
		}
		else {
			pNum = 10;
			continue;
		}
	}
	return pNum;
}

//학번 : 13-19년도에 입학한 학생의 9자리 숫자
int studentNumGenerator() {
	int sNum = 2010;
	while (TRUE) {
		sNum += rand() % 7 + 3;
		for (int i = 0; i < 5; i++) {
			sNum *= 10;
			sNum += rand() % 10;
		}
		if (0 == studentNumTable[sNum % 100000]) { //중복방지
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

//이름 : 10자 크기의 영문자 임의의 문자
void nameGenerator(char * nameArr, int len) {
	for (int i = 0; i < len; i++) {
		nameArr[i] = (char)(rand() % 26) + 'A';
	}
	nameArr[10] = '\0';
}

//qsort 함수포인터 인자에 필요한 compare함수
int compare(const void *a, const void *b)
{
	return	(*(Data*)a).studentNum - (*(Data*)b).studentNum;; //오름차순 정렬, 첫 파라미터가 더 크면 양수, 같으면 0, 두번째 파라미터가 더 크면 음수(strcmp와 동일한 구조)
}

void QuickSort_NonRecur(Data arr[], int len) {
	Data tmpData;
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
		if (right - left + 1 > 1) { //현구간(left~right)의 배열의 길이가 1보다 크면, 즉 정렬할 데이터의 개수가 1개만 남을 때까지
			pivot = arr[right].studentNum;
			low = left - 1;
			high = right;
			
			//재귀로 구현한 퀵정렬과 같은 구조인 부분, low는 끝으로, high는 앞으로, 역전하면 탈출
			while (TRUE)
			{
				while (arr[++low].studentNum < pivot);
				while (arr[--high].studentNum > pivot);
				if (low >= high) break;
				memcpy(&tmpData, &arr[low], sizeof(Data));
				memcpy(&arr[low], &arr[high], sizeof(Data));
				memcpy(&arr[high], &tmpData, sizeof(Data));
			}
			memcpy(&tmpData, &arr[low], sizeof(Data));
			memcpy(&arr[low], &arr[right], sizeof(Data));
			memcpy(&arr[right], &tmpData, sizeof(Data));

			push(&callStack, right);
			push(&callStack, low + 1); //원래 퀵정렬에서 두번째 재귀호출(피봇의 뒷부분)
			push(&callStack, low - 1); //원래 퀵정렬에서 첫번째 재귀호출(피봇의 앞부분)
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
	double cpuTimes[2] = { 0 }; clock_t start_t, end_t; double duration_t;
	srand(time(NULL));
	Data stuData[MAX_STUDENT_NUM];
	Data sortStuData[MAX_STUDENT_NUM]; //DummyData for Sorting Test
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		//Generate MAX_STUDENT_NUM students data
		stuData[i].studentNum = studentNumGenerator();
		stuData[i].phoneNum = phoneNumGenerator();
		nameGenerator(stuData[i].name, 10);
	}
	//Sort by Studentnumber Comapare
	//Quick Sort(Recursive, qsort in C standard library)
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("QuickSort by StudentNumber Start!!\n");
	system("pause");
	start_t = clock();
	qsort(sortStuData, sizeof(sortStuData) / sizeof(Data), sizeof(Data), compare);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("학번:%d\t전화번호:%011d\t이름:%s\n", sortStuData[i].studentNum, sortStuData[i].phoneNum, sortStuData[i].name);
	}
	printf("Quicksort by StudentNumber time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[0] = duration_t;

	//Quick Sort(Non - Recursive)
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("QuickSort(Non-Recursive) by StudentNumber Start!!\n");
	system("pause");
	start_t = clock();
	QuickSort_NonRecur(sortStuData, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("학번:%d\t전화번호:%011d\t이름:%s\n", sortStuData[i].studentNum, sortStuData[i].phoneNum, sortStuData[i].name);
	}
	printf("Non-Recursive Quicksort time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[1] = duration_t;

	printf("\n==========================Total Result==========================\n\n");
	printf("Number of Data = %d\n", MAX_STUDENT_NUM);
	printf("Sort by StudentNumber using QuickSort(Recursive) and QuickSort(Non-recursive) and Compare these two sorting Algs\n\n");
	printf("QuickSort(Recursive) by StudentNumber\n");
	printf("%-50s = %fms\n", "Quicksort(Recursive) time taken by CPU", cpuTimes[0]);
	printf("\n");
	printf("QuickSort(Non-Recursive) by StudentNumber\n");
	printf("%-50s = %fms\n", "Quicksort(Non-Recursive) time taken by CPU", cpuTimes[1]);
	return 0;
}


