#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define STACK_LEN 100
#define TRUE 1 
#define FALSE 0
#define INF 2147483648
#define SWAP(a, b){a ^= b, b ^= a, a ^= b}
#define MAX(a, b)((a) > (b) ? (a) : (b))
#define MAX_STUDENT_NUM 1000
int phoneNumTable[100000000];
int studentNumTable[1000000];
typedef struct _student {
	int studentNum; //학번 : 13-19년도에 입학한 학생의 9자리 숫자
	int phoneNum; //전화번호 : 010으로 시작하는 11자리 숫자
	char name[11]; //이름 : 10자 크기의 영문자 임의의 문자
}STUDENT;
typedef STUDENT Data;
int phoneNumGenerator();
int studentNumGenerator(); 
void nameGenerator(char * nameArr, int len);

//Functions for sorting
void SelectionSort(int arr[], int len);
void QuickSort(int arr[], int low, int high, int len);
void HeapSort(int arr[], int len);

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

void SelectionSort(int arr[], int len) {
	int i, j, tmp;
}

void QuickSort(int arr[], int low, int high, int len) {
	int mid = low + high / 2;
}

void HeapSort(int arr[], int len) {

}
//Program for comparing 3 different sort. Selection, Quick, Heap
int main() {
	int i;
	srand(time(NULL));
	Data stuData[MAX_STUDENT_NUM];
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		//Generate 1000 students data
		stuData[i].studentNum = studentNumGenerator();
		stuData[i].phoneNum = phoneNumGenerator();
		nameGenerator(stuData[i].name, 10);
	}
	//Sorting (Students Number)
	//Print
	//Print time spent
	//3 Trials with every sorting funcs and compare.

	//Sorting (Name)
	//Print
	//Print time spent
	//3 Trials with every sorting funcs and compare.
	return 0;
}