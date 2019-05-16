#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define STACK_LEN 100
#define TRUE 1 
#define FALSE 0
#define INF 2147483648
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
#define MAX_STUDENT_NUM 20000
#define LCHILD(me)   (2*me +1)
#define RCHILD(me)  (LCHILD(me)+1)
#define PARENT(me)  ((me-1)/2)
//typedef, struct, Functions Declaration
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

//Functions for sorting.(used qsort() in standard C library for QuickSort)
void SelectionSort(Data arr[], int len);
void SelectionSort_str(Data arr[], int len);
void HeapSort(Data arr[], int len);
void HeapSort_str(Data arr[], int len);
int compare(const void *a, const void *b);
int compare_str(const void *a, const void *b);
//typedef, struct, Functions Declaration


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

void SelectionSort(Data arr[], int len) {
	int i, j, minIdx;
	Data tmpData;
	for (i = 0; i < len-1; i++) {
		minIdx = i;
		for (j = i + 1; j < len; j++) {
			if (arr[j].studentNum < arr[minIdx].studentNum) minIdx = j;
		}
		if (arr[i].studentNum != arr[minIdx].studentNum) { //나 자신이랑 교환하지 않기 위해 예외처리.
			memcpy(&tmpData, &arr[i], sizeof(Data));
			memcpy(&arr[i], &arr[minIdx], sizeof(Data));
			memcpy(&arr[minIdx], &tmpData, sizeof(Data));
		}
	}
}

void SelectionSort_str(Data arr[], int len) {
	int i, j, minIdx;
	Data tmpData;
	char tmpStr[11] = { NULL };
	for (i = 0; i < len - 1; i++) {
		minIdx = i;
		for (j = i + 1; j < len; j++) {
			if (strcmp(arr[minIdx].name,arr[j].name) >= 0) minIdx = j;
		}
		if (strcmp(arr[i].name, arr[minIdx].name) != 0) { //나 자신이랑 교환하지 않기 위해 예외처리.
			memcpy(&tmpData, &arr[i], sizeof(Data));
			memcpy(&arr[i], &arr[minIdx], sizeof(Data));
			memcpy(&arr[minIdx], &tmpData, sizeof(Data));
		}
	}
}

//qsort 함수포인터 인자에 필요한 compare함수
int compare(const void *a, const void *b)
{
	return	(*(Data*)a).studentNum - (*(Data*)b).studentNum;; //오름차순 정렬, 첫 파라미터가 더 크면 양수, 같으면 0, 두번째 파라미터가 더 크면 음수(strcmp와 동일한 구조)
}

int compare_str(const void *a, const void *b)
{
	return	strcmp((*(Data*)a).name,(*(Data*)b).name); //오름차순 정렬, 첫 파라미터가 더 크면 양수, 같으면 0, 두번째 파라미터가 더 크면 음수(strcmp와 동일한 구조)
}

void HeapSort(Data arr[], int len) { //Max Heap 힙의 최대값(루트)을 배열의 마지막부터 채워나가는 알고리즘
	Data tmpData;
	for (int i = 1; i < len; i++) {
		int curIdx = i;
		while (curIdx > 0) {
			int paIdx = PARENT(curIdx);
			if (arr[paIdx].studentNum < arr[curIdx].studentNum) {
				memcpy(&tmpData, &arr[curIdx], sizeof(Data));
				memcpy(&arr[curIdx], &arr[paIdx], sizeof(Data));
				memcpy(&arr[paIdx], &tmpData, sizeof(Data));
			}
			curIdx = paIdx;
		}
	}
	memcpy(&tmpData, &arr[0], sizeof(Data));
	memcpy(&arr[0], &arr[len - 1], sizeof(Data));
	memcpy(&arr[len - 1], &tmpData, sizeof(Data));
	len--; //힙 초기상태 구성 완료.
	int me;
	int lch, rch;
	while (len > 1) {
		me = 0;
		while (TRUE) {
			lch = LCHILD(me);
			rch = RCHILD(me);
			if (lch >= len) { //자식이 없는 상태(me가 마지막 데이터인 상태)
				break;
			}
			if (rch == len) { //자식이 왼쪽만 있는 상태(me의 leftChild가 마지막 데이터인 상태)
				if (arr[me].studentNum < arr[lch].studentNum) {
					memcpy(&tmpData, &arr[me], sizeof(Data));
					memcpy(&arr[me], &arr[lch], sizeof(Data));
					memcpy(&arr[lch], &tmpData, sizeof(Data));
				}
				break;
			}
			//여기서부턴 자식이 둘 다 있는 상태.
			int maxCh = arr[lch].studentNum < arr[rch].studentNum ? rch : lch; //좌 우 자식 중 큰 값의 인덱스를 maxCh에 저장
			if (arr[me].studentNum < arr[maxCh].studentNum) {
				memcpy(&tmpData, &arr[me], sizeof(Data));
				memcpy(&arr[me], &arr[maxCh], sizeof(Data));
				memcpy(&arr[maxCh], &tmpData, sizeof(Data));
				me = maxCh;
			}
			else {
				break;
			}
		}
		memcpy(&tmpData, &arr[0], sizeof(Data));
		memcpy(&arr[0], &arr[len - 1], sizeof(Data));
		memcpy(&arr[len - 1], &tmpData, sizeof(Data));
		len--;
	}
}

void HeapSort_str(Data arr[], int len) { //Max Heap,힙의 최대값(루트)을 배열의 마지막부터 채워나가는 알고리즘
	Data tmpData;
	char tmpStr[11] = { NULL }; //문자열 Swap을 위해 필요한 임시변수
	for (int i = 1; i < len; i++) {
		int curIdx = i;
		while (curIdx > 0) {
			int paIdx = PARENT(curIdx);
			if (strcmp(arr[curIdx].name,arr[paIdx].name) >= 0) {
				memcpy(&tmpData, &arr[curIdx], sizeof(Data));
				memcpy(&arr[curIdx], &arr[paIdx], sizeof(Data));
				memcpy(&arr[paIdx], &tmpData, sizeof(Data));
			}
			curIdx = paIdx;
		}
	}
	memcpy(&tmpData, &arr[0], sizeof(Data));
	memcpy(&arr[0], &arr[len - 1], sizeof(Data));
	memcpy(&arr[len - 1], &tmpData, sizeof(Data));
	len--; //힙 초기상태 구성 완료.
	int me;
	int lch, rch;
	while (len > 1) {
		me = 0;
		while (TRUE) {
			lch = LCHILD(me);
			rch = RCHILD(me);
			if (lch >= len) { //자식이 없는 상태(me가 마지막 데이터인 상태)
				break;
			}
			if (rch == len) { //자식이 왼쪽만 있는 상태(me의 leftChild가 마지막 데이터인 상태)
				if (strcmp(arr[lch].name, arr[me].name) >= 0) {
					memcpy(&tmpData, &arr[me], sizeof(Data));
					memcpy(&arr[me], &arr[lch], sizeof(Data));
					memcpy(&arr[lch], &tmpData, sizeof(Data));
				}
				break;
			}
			//여기서부턴 자식이 둘 다 있는 상태.
			int maxCh = strcmp(arr[rch].name, arr[lch].name) >= 0 ? rch : lch; //좌 우 자식 중 큰 값의 인덱스를 maxCh에 저장
			if (strcmp(arr[maxCh].name, arr[me].name) >= 0) {
				memcpy(&tmpData, &arr[me], sizeof(Data));
				memcpy(&arr[me], &arr[maxCh], sizeof(Data));
				memcpy(&arr[maxCh], &tmpData, sizeof(Data));
				me = maxCh;
			}
			else {
				break;
			}
		}
		memcpy(&tmpData, &arr[0], sizeof(Data));
		memcpy(&arr[0], &arr[len - 1], sizeof(Data));
		memcpy(&arr[len - 1], &tmpData, sizeof(Data));
		len--;
	}
}

//Program for comparing 3 different sort. Selection, Quick, Heap
int main() {
	int i;
	double cpuTimes[6] = { 0 };
	clock_t start_t, end_t; double duration_t;
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
	//Selection Sort
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("SelectionSort by StudentNumber Start!!\n");
	start_t = clock();
	SelectionSort(sortStuData, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	system("pause");
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("학번:%d\t전화번호:%011d\t이름:%s\n", sortStuData[i].studentNum, sortStuData[i].phoneNum, sortStuData[i].name);
	}
	printf("SelctionSort by StudentNumber time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[0] = duration_t;


	//Quick Sort(qsort in C standard library)
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
	cpuTimes[1] = duration_t;


	//Heap Sort
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("HeapSort by StudentNumber Start!!\n");
	system("pause");
	start_t = clock();
	HeapSort(sortStuData, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("학번:%d\t전화번호:%011d\t이름:%s\n", sortStuData[i].studentNum, sortStuData[i].phoneNum, sortStuData[i].name);
	}
	printf("HeapSort by StudentNumber time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[2] = duration_t;

	//Sort by Name Comapare
	//Selection Sort
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("SelectionSort by Name Start!!\n");
	system("pause");
	start_t = clock();
	SelectionSort_str(sortStuData, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("이름:%s\t학번:%d\t전화번호:%011d\n", sortStuData[i].name, sortStuData[i].studentNum, sortStuData[i].phoneNum);
	}
	printf("SelectionSort by Name time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[3] = duration_t;
	

	//Quick Sort(qsort in C standard library)
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("QuickSort by Name Start!!\n");
	system("pause");
	start_t = clock();
	qsort(sortStuData, sizeof(sortStuData) / sizeof(Data), sizeof(Data), compare_str);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("이름:%s\t학번:%d\t전화번호:%011d\n", sortStuData[i].name, sortStuData[i].studentNum, sortStuData[i].phoneNum);
	}
	printf("Quicksort by Name time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[4] = duration_t;


	//Heap Sort
	memcpy(sortStuData, stuData, sizeof(Data) * MAX_STUDENT_NUM);
	printf("HeapSort by Name Start!!\n");
	system("pause");
	start_t = clock();
	HeapSort_str(sortStuData, MAX_STUDENT_NUM);
	end_t = clock();
	duration_t = (double)(end_t - start_t);
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		printf("이름:%s\t학번:%d\t전화번호:%011d\n", sortStuData[i].name, sortStuData[i].studentNum, sortStuData[i].phoneNum);
	}
	printf("HeapSort by Name time taken by CPU: %fms\n\n", duration_t);
	cpuTimes[5] = duration_t;

	printf("\n==========================Total Result==========================\n\n");
	printf("Number of Data = %d\n", MAX_STUDENT_NUM);
	printf("Sort by StudentNumber, Name each and Compare all the results\n\n");
	printf("Sort by StudentNumber\n");
	printf("%-50s = %fms\n", "SelectionSort by StudentNumber time taken by CPU", cpuTimes[0]);
	printf("%-50s = %fms\n", "Quicksort by StudentNumber time taken by CPU", cpuTimes[1]);
	printf("%-50s = %fms\n", "HeapSort by StudentNumber time taken by CPU", cpuTimes[2]);
	printf("\nSort by Name\n");
	printf("%-50s = %fms\n", "SelectionSort by Name time taken by CPU", cpuTimes[3]);
	printf("%-50s = %fms\n", "Quicksort by Name time taken by CPU", cpuTimes[4]);
	printf("%-50s = %fms\n", "HeapSort by Name time taken by CPU", cpuTimes[5]);
	return 0;
}