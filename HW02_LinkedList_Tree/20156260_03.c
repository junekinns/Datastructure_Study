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
int index = 0; //��� �ǳʶٱ�� ��������
//typedef, struct, Functions Declaration
typedef struct _student {
	int studentNum; //�й� : 13-19�⵵�� ������ �л��� 9�ڸ� ����
	int phoneNum; //��ȭ��ȣ : 010���� �����ϴ� 11�ڸ� ����
	char name[NAME_LEN + 1]; //�̸� : 10�� ũ���� ������ ������ ����
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

//��ȭ��ȣ : 010���� �����ϴ� 11�ڸ� ����
int phoneNumGenerator() {
	int pNum = 10;
	while (TRUE) {
		for (int i = 0; i < 8; i++) {
			pNum *= 10;
			pNum += rand() % 10;
		}
		if (0 == phoneNumTable[pNum % 100000000]) { //�ߺ�����
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
//�̸� : 10�� ũ���� ������ ������ ����
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
			//������ 1000�� ���� ����
			lastData[i - (MAX_STUDENT_NUM - LAST_DATA_NUM)] = stuData[i].studentNum;
		}
	}
	printf("Step 1. ���� ��ȸ �ϱ�\n");
	/*(1) ������ ���� Ž�� Ʈ���� ���� ��ȸ�Ѵ�. 
	��ȸ�� �� �й��� ȭ�鿡 ǥ���Ѵ�. 
	�� �� ���߿� �湮�� ���� ���� �湮�� ��忡 ���� ���� �й��̾�� �Ѵ�. 
	ȭ�鿡 ǥ���� �й��� ��ȣ������ �Ǿ� �ִ��� Ȯ���Ѵ�. (���� Ž�� Ʈ���� �� �����Ǿ������� Ȯ���ϴ� ����)*/
	inorderBST(pRoot);

	printf("Step 2. ������ Ž���ϱ�\n");
	/*(2) �������� ������ 1000���� �й��� �̿��Ͽ� ���� Ž���� �Ѵ�. Ž�� �� �ش� �й��� �̸��� ȭ�鿡 ǥ���Ѵ�.*/
	for (i = 0; i < LAST_DATA_NUM; i++) {
		BstNode * searchNode = searchByKey(pRoot, lastData[i]);
		if (NULL == searchNode) {
			printf("Error\n");
			continue;
		}
		printf("%04d. %d %s\n", i + 1, searchNode->data.studentNum, searchNode->data.name);
	}

	printf("Step 3. ������ �й����� �˻��ϱ�\n");
	memset(studentNumTable, 0, 1000000);
	/*(3) �����ϰ� �й� 1000���� ������ ��,
	Ž�� Ʈ���� �����ϴ��� Ȯ���Ѵ�.
	�����ϴ� �й��� ������ �� �й��� ȭ�鿡 ǥ���Ѵ�. ������ ���ٰ� ǥ���Ѵ�.*/
	for (i = 0; i < RANDOM_DATA_NUM; i++) {
		//Generate RANDOM_DATA_NUM student number
		int searchKey = studentNumGenerator();
		BstNode * searchNode = searchByKey(pRoot, searchKey);
		if (NULL == searchNode) {
			printf("%04d. %d (����)\n", i + 1, searchKey);
		}
		else {
			foundCount++;
			printf("%04d. %d %s\n", i + 1, searchNode->data.studentNum, searchNode->data.name);
		}
	}
	printf("�� %d�� �߰�\n", foundCount);
	return 0;
}