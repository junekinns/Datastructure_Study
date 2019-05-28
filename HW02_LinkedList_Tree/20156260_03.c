#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRUE 1 
#define FALSE 0
#define MAX_STUDENT_NUM 50000
#define LAST_DATA_NUM 1000
#define RANDOM_DATA_NUM 1000
#define OFFSET 1000
#define NAME_LEN 10
int index = 0; //출력 건너뛰기용 전역변수
//typedef, struct, Functions Declaration
typedef struct _student {
	int studentNum; //학번 : 13-19년도에 입학한 학생의 9자리 숫자
	int phoneNum; //전화번호 : 010으로 시작하는 11자리 숫자
	char name[NAME_LEN + 1]; //이름 : 10자 크기의 영문자 임의의 문자
}STUDENT;
typedef STUDENT Data;

typedef struct bstNode {
	Data data;
	struct bstNode * pRight;
	struct bstNode * pLeft;
}BstNode;
//Tables for memoization to avoid overlap
int phoneNumTable[100000000];
int studentNumTable[1000000];

//Functions for generating random data(Phone Number, Student Number, Name)
int phoneNumGenerator();
int studentNumGenerator();
void nameGenerator(char * nameArr, int len);

//Functions for BST
void insertBNode(BstNode ** root,  Data inputData);
void inorderBST(BstNode * pNode);
BstNode * searchByKey(BstNode * root, int key);

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
	int i;
	for (i = 0; i < len; i++) {
		nameArr[i] = (char)(rand() % 26) + 'A';
	}
	nameArr[i] = '\0';
}

void insertBNode(BstNode ** root, Data inputData) {
	BstNode * newBt = (BstNode*)malloc(sizeof(BstNode));
	newBt->data = inputData;
	newBt->pLeft = NULL;
	newBt->pRight = NULL;
	BstNode * pRoot = *root;

	if (NULL == pRoot) {
		*root = newBt;
	}
	else {
		BstNode * pNode = pRoot;
		while (TRUE) {
			if (inputData.studentNum < pNode->data.studentNum) {
				if (NULL == pNode->pLeft) {
					pNode->pLeft = newBt;
					break;
				}
				else {
					pNode = pNode->pLeft;
				}
			}
			else if (inputData.studentNum > pNode->data.studentNum) {
				if (NULL == pNode->pRight) {
					pNode->pRight = newBt;
					break;
				}
				else {
					pNode = pNode->pRight;
				}
			}
		}
	}
}

void inorderBST(BstNode * pNode) {
	if (NULL == pNode) return;
	inorderBST(pNode->pLeft);
	index++;
	if(index % OFFSET == 0)
		printf("%d\n", pNode->data.studentNum);
	inorderBST(pNode->pRight);
}

BstNode * searchByKey(BstNode * root, int key) {
	BstNode * pNode = root;
	while (TRUE) {
		if (NULL == pNode)
			return NULL;
		if (key == pNode->data.studentNum) {
			break;
		}
		else {
			if (key < pNode->data.studentNum)
				pNode = pNode->pLeft;
			else
				pNode = pNode->pRight;
		}
	}
	return pNode;
}

Data stuData[MAX_STUDENT_NUM];
int main() {
	int i;
	int foundCount = 0;
	srand(time(NULL));
	BstNode * pRoot = NULL;
	int lastData[LAST_DATA_NUM];
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		//Generate MAX_STUDENT_NUM students data
		stuData[i].studentNum = studentNumGenerator();
		stuData[i].phoneNum = phoneNumGenerator();
		nameGenerator(stuData[i].name, NAME_LEN);
		insertBNode(&pRoot, stuData[i]);
		if (i >= (MAX_STUDENT_NUM - LAST_DATA_NUM)) {
			//마지막 1000개 따로 저장
			lastData[i - (MAX_STUDENT_NUM - LAST_DATA_NUM)] = stuData[i].studentNum;
		}
	}
	printf("Step 1. 중위 순회 하기\n");
	/*(1) 구성된 이진 탐색 트리를 중위 순회한다. 
	순회할 때 학번을 화면에 표시한다. 
	이 때 나중에 방문한 노드는 먼저 방문한 노드에 비해 뒤쪽 학번이어야 한다. 
	화면에 표시한 학번이 번호순으로 되어 있는지 확인한다. (이진 탐색 트리가 잘 구성되었는지를 확인하는 목적)*/
	inorderBST(pRoot);

	printf("Step 2. 데이터 탐색하기\n");
	/*(2) 마지막에 생성된 1000개의 학번을 이용하여 각각 탐색을 한다. 탐색 후 해당 학번과 이름을 화면에 표시한다.*/
	for (i = 0; i < LAST_DATA_NUM; i++) {
		BstNode * searchNode = searchByKey(pRoot, lastData[i]);
		if (NULL == searchNode) {
			printf("Error\n");
			continue;
		}
		printf("%04d. %d %s\n", i + 1, searchNode->data.studentNum, searchNode->data.name);
	}

	printf("Step 3. 랜덤한 학번으로 검색하기\n");
	memset(studentNumTable, 0, 1000000);
	/*(3) 랜덤하게 학번 1000개를 생성한 후,
	탐색 트리에 존재하는지 확인한다.
	존재하는 학번이 있으면 그 학번을 화면에 표시한다. 없으면 없다고 표시한다.*/
	for (i = 0; i < RANDOM_DATA_NUM; i++) {
		//Generate RANDOM_DATA_NUM student number
		int searchKey = studentNumGenerator();
		BstNode * searchNode = searchByKey(pRoot, searchKey);
		if (NULL == searchNode) {
			printf("%04d. %d (없음)\n", i + 1, searchKey);
		}
		else {
			foundCount++;
			printf("%04d. %d %s\n", i + 1, searchNode->data.studentNum, searchNode->data.name);
		}
	}
	printf("총 %d개 발견\n", foundCount);
	return 0;
}