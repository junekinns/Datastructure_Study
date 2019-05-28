#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRUE 1 
#define FALSE 0
#define MAX_STUDENT_NUM 10000
#define OFFSET 100
#define NAME_LEN 10
//typedef, struct, Functions Declaration
typedef struct _student {
	int studentNum; //�й� : 13-19�⵵�� ������ �л��� 9�ڸ� ����
	int phoneNum; //��ȭ��ȣ : 010���� �����ϴ� 11�ڸ� ����
	char name[NAME_LEN + 1]; //�̸� : 10�� ũ���� ������ ������ ����
}STUDENT;
typedef STUDENT Data;

typedef struct singlyListNode {
	Data data;
	struct singlyListNode * pNext_num;
	struct singlyListNode * pNext_name;
}ListNode;
//Tables for memoization to avoid overlap
int phoneNumTable[100000000];
int studentNumTable[1000000];

//Functions for generating random data(Phone Number, Student Number, Name)
int phoneNumGenerator();
int studentNumGenerator();
void nameGenerator(char * nameArr, int len);

//Functions for LinkedList
void insertNode(ListNode ** stunum_head, ListNode ** name_head, Data inputData);
void nameOrder_displayList(ListNode * head);
void stunumOrder_displayList(ListNode * head);

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

//�� head(�й�, �̸� ����)�� �� Data�� �߰��ϴ� �Լ�
void insertNode(ListNode ** stunum_head, ListNode ** name_head, Data inputData) {
	ListNode * stunum_pNode = *stunum_head;
	ListNode * name_pNode = *name_head;
	ListNode * newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = inputData;
	newNode->pNext_name = NULL;
	newNode->pNext_num = NULL;
	if (NULL == stunum_pNode && NULL == name_pNode) {
		stunum_pNode = newNode;
		name_pNode = newNode;
		*stunum_head = stunum_pNode;
		*name_head = name_pNode;
	}
	else { //��ġ�� �°� ���� ã�ư�����.
		stunum_pNode = *stunum_head;
		name_pNode = *name_head;

		//�й� �� ó��
		if (newNode->data.studentNum <= stunum_pNode->data.studentNum) { //���ο� ��尡 �� ���� ���
			newNode->pNext_num = stunum_pNode;
			stunum_pNode = newNode;
			*stunum_head = stunum_pNode; //�� ��尡 ��尡 �Ǿ�� ��
		}
		else { //���ο� ��尡 �߰��̰ų�, ���� ū ���
			while (NULL != stunum_pNode->pNext_num) { //������ Ž��
			if (newNode->data.studentNum <= stunum_pNode->pNext_num->data.studentNum) { //�߰�
				newNode->pNext_num = stunum_pNode->pNext_num;
				stunum_pNode->pNext_num = newNode;
				break; //�ڸ��� ã�� ���������� Ż��
			}
			stunum_pNode = stunum_pNode->pNext_num;
			}
			if (NULL == stunum_pNode->pNext_num) {//��
				stunum_pNode->pNext_num = newNode;
				newNode->pNext_num = NULL;
			}
		}

		//�̸� �� ó��
		if (strcmp(newNode->data.name, name_pNode->data.name) <= 0) { //���ο� ��尡 ���������� �ռ����
			newNode->pNext_name = name_pNode;
			name_pNode = newNode;
			*name_head = name_pNode; //�� ��尡 ��尡 �Ǿ�� ��
		}
		else { //���ο� ��尡 �߰��̰ų�, ���� ū ���
			while (NULL != name_pNode->pNext_name) { //������ Ž��
				if (strcmp(newNode->data.name, name_pNode->pNext_name->data.name) <= 0) { //�߰�
					newNode->pNext_name = name_pNode->pNext_name;
					name_pNode->pNext_name = newNode;
					break; //�ڸ��� ã�� ���������� Ż��
				}
				name_pNode = name_pNode->pNext_name;
			}
			if (NULL == name_pNode->pNext_name) {//��
				name_pNode->pNext_name = newNode;
				newNode->pNext_name = NULL;
			}
		}
	}
}
void nameOrder_displayList(ListNode * head) {
	int index = 1;
	ListNode * pNode = head;
	while (NULL != pNode) {
		if(index % OFFSET == 0) //OFFSET �� �ѹ����� ���
			printf("%05d. �̸�:%s\t�й�:%d\t��ȭ��ȣ:%011d\n", index, pNode->data.name, pNode->data.studentNum, pNode->data.phoneNum);
		index++;
		pNode = pNode->pNext_name;
	}
}
void stunumOrder_displayList(ListNode * head) {
	int index = 1;
	ListNode * pNode = head;
	while (NULL != pNode) {
		if (index % OFFSET == 0) //OFFSET �� �ѹ����� ���
			printf("%05d. �й�:%d\t�̸�:%s\t��ȭ��ȣ:%011d\n", index, pNode->data.studentNum, pNode->data.name, pNode->data.phoneNum);
		index++;
		pNode = pNode->pNext_num;
	}
}
int main() {
	int i; 
	int menu;
	srand(time(NULL));
	Data stuData[MAX_STUDENT_NUM];
	ListNode * stunum_head = NULL;
	ListNode * name_head = NULL;
	for (i = 0; i < MAX_STUDENT_NUM; i++) {
		//Generate MAX_STUDENT_NUM students data
		stuData[i].studentNum = studentNumGenerator();
		stuData[i].phoneNum = phoneNumGenerator();
		nameGenerator(stuData[i].name, NAME_LEN);
		insertNode(&stunum_head, &name_head, stuData[i]);
	}
	while (TRUE)
	{
		printf("(1) �й���\n");
		printf("(2) �̸���\n");
		printf("�޴� ����(0: ����) : ");
		scanf_s("%d", &menu);
		switch (menu)
		{
		case 0:
			printf("���α׷��� �����մϴ�.\n");
			exit(0);
			break;
		case 1:
			printf("�й� ������ %d���� �����͸� ����մϴ�. %d�� ������ ��µ˴ϴ�.\n", MAX_STUDENT_NUM, OFFSET);
			stunumOrder_displayList(stunum_head);
			break;
		case 2:
			printf("�̸� ������ %d���� �����͸� ����մϴ�. %d�� ������ ��µ˴ϴ�.\n", MAX_STUDENT_NUM, OFFSET);
			nameOrder_displayList(name_head);
			break;
		}
	}
	return 0;
}