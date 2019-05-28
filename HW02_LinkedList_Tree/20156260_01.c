#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRUE 1
#define FALSE 0
#define INIT_STUDENT_NUM 10
#define NAME_LEN 3
#define MAX_COMMAND_LEN 10

//typedef, struct, Functions Declaration
typedef struct _student {
	int phoneNum; //��ȭ��ȣ : 010���� �����ϴ� 11�ڸ� ����
	char name[NAME_LEN+1]; //�̸� : 3�� ũ���� ������ ������ ����
}STUDENT;
typedef STUDENT Data;
typedef struct singlyListNode {
	Data data;
	struct singlyListNode * pNext;
}ListNode;

//Tables for memoization to avoid overlap
int phoneNumTable[100000000];

//Functions for generating random data(Phone Number, Name)
int phoneNumGenerator();
void nameGenerator(char * nameArr, int len);

//Functions for LinkedList
void insertNodeToHead(ListNode ** phead, Data inputData);
int deleteNodebyName(ListNode * head, const char * inputName);
void displayList(ListNode * head);
ListNode * findWithName(ListNode * head, const char * inputName);

//Functions for SortList
ListNode * SortedMerge(ListNode * a, ListNode * b);
void FrontBackSplit(ListNode * source, ListNode ** frontRef, ListNode ** backRef);
void MergeSort_List(ListNode** head);

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
//�̸� : 10�� ũ���� ������ ������ ����
void nameGenerator(char * nameArr, int len) {
	int i;
	for (i = 0; i < len; i++) {
		nameArr[i] = (char)(rand() % 26) + 'A';
	}
	nameArr[i] = '\0';
}
//head�� �� Data�� �߰��ϴ� �Լ�
void insertNodeToHead(ListNode ** phead, Data inputData) {
	ListNode * newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = inputData;
	newNode->pNext = NULL;
	ListNode * pNode = *phead;
	if (NULL == pNode) {
		pNode = newNode;
	}
	else {
		newNode->pNext = pNode;
		pNode = newNode;
	}
	*phead = pNode;
}
//�̸����� Ž�� �� �����ϴ� �Լ�. �����ϸ� True, �����ϸ� False ��ȯ
int deleteNodebyName(ListNode ** head, const char * inputName) {
	ListNode * delNode = findWithName(*head, inputName);
	if (NULL == delNode) {
		return 0;
	}
	else {
		ListNode * pNode = *head;
		int delnum = delNode->data.phoneNum;
		if (delNode == pNode) {
			*head = pNode->pNext;
			free(delNode);
			delNode = NULL;
			return delnum;
		}
		while (delNode != pNode->pNext) {
			pNode = pNode->pNext;
		}
		pNode->pNext = pNode->pNext->pNext;
		free(delNode);
		delNode = NULL;
		return delnum;
	}
}
void displayList(ListNode * head) {
	int index = 1;
	ListNode * pNode = head;
	while (NULL != pNode) {
		printf("%02d. Name : %s\tPhoneNum : %011d\n", index++, pNode->data.name, pNode->data.phoneNum);
		pNode = pNode->pNext;
	}
}
ListNode * findWithName(ListNode * head, const char * inputName) {
	ListNode * pNode = head;
	while (NULL != pNode) {
		if (strcmp(inputName, pNode->data.name) == 0) {
			return pNode;
		}
		pNode = pNode->pNext;
	}
	return NULL;
}

//�ΰ��� ���ĵ� ���� ����Ʈ�� �ϳ��� ��ġ�� �Լ�
ListNode * SortedMerge(ListNode * a, ListNode * b)
{
	if (NULL == a)
		return b;
	else if (NULL == b)
		return a;

	ListNode * result = NULL;

	//if (a->data.phoneNum <= b->data.phoneNum) ��ȭ��ȣ���� ����
	if (strcmp(a->data.name,b->data.name) <= 0)
	{
		result = a;
		result->pNext = SortedMerge(a->pNext, b);
	}
	else
	{
		result = b;
		result->pNext = SortedMerge(a, b->pNext);
	}
	return result;
}


//�־��� ��带 ��/�� ������ ������.
void FrontBackSplit(ListNode * source, ListNode ** frontRef, ListNode ** backRef)
{
	if (NULL == source || NULL == source->pNext)
	{
		*frontRef = source;
		*backRef = NULL;
		return;
	}

	ListNode * slow = source;
	ListNode * fast = source->pNext;

	//fast�����Ͱ� 2������, slow�����ʹ� 1�� ������Ų��.
	while (NULL != fast)
	{
		fast = fast->pNext;
		if (NULL != fast)
		{
			slow = slow->pNext;
			fast = fast->pNext;
		}
	}

	//slow�����Ͱ� ����Ʈ�� �߰��� ��ġ�Ѵ�. �ű⼭ �ѷ� �ɰ���.
	*frontRef = source;
	*backRef = slow->pNext;
	slow->pNext = NULL;
}

void MergeSort_List(ListNode** head)
{
	//���̰� 0�̳� 1�ΰ��
	if (NULL == *head || NULL == (*head)->pNext)
		return;

	ListNode * a = NULL;
	ListNode * b = NULL;

	// head�� a,b�� �ѷ� ������.
	FrontBackSplit(*head, &a, &b);

	// �������� sublist�� ���� ��������� �ٽ� �����Ѵ�.
	MergeSort_List(&a);
	MergeSort_List(&b);
	*head = SortedMerge(a, b);
}
int main() {
	srand(time(NULL));
	ListNode * head = NULL;
	Data stuData[INIT_STUDENT_NUM];
	int i;
	char commandStr[MAX_COMMAND_LEN];
	int numofData = INIT_STUDENT_NUM;
	for (i = 0; i < INIT_STUDENT_NUM; i++) {
		stuData[i].phoneNum = phoneNumGenerator();
		nameGenerator(stuData[i].name, NAME_LEN);
		insertNodeToHead(&head, stuData[i]);
	}
	while (TRUE)
	{
		printf("(%d���� �����Ͱ� �ֽ��ϴ�.)\n", numofData);
		printf("(��ɾ �Է��ϼ���. �̸������� ���� ���ĺ� 3���ڷ� ���ѵ˴ϴ�.)\n");
		printf("(���α׷��� �����Ϸ��� \"����\" �� �Է��ϼ���.)\n");
		printf(">> ");
		gets_s(commandStr, MAX_COMMAND_LEN);
		if (strlen(commandStr) == 4) {
			if (strcmp(commandStr, "���") == 0) {
				displayList(head);
			}
			else if((strcmp(commandStr, "����") == 0)){
				MergeSort_List(&head);
				displayList(head);
			}
			else if ((strcmp(commandStr, "����") == 0)) {
				exit(0);
			}
		}
		else if (strlen(commandStr) > 4) {
			char delName[NAME_LEN + 1];
			strcpy_s(delName, sizeof(delName), &commandStr[5]);//���� �̸� ���� �̸� �κи� ������
			int delNum = deleteNodebyName(&head, delName);
			if (0 != delNum) {
				numofData--;
				printf("(%s %011d �� �����Ǿ����ϴ�.)\n", &commandStr[5], delNum);
			} else {
				printf("(���� ����!)\n");
			}
		}
		else if (strlen(commandStr) == NAME_LEN){
			ListNode * tmpNode = findWithName(head, commandStr);
			if (tmpNode != NULL) {
				//ã�ҽ��ϴ�. ��ȭ��ȣ �����ֱ�
				printf("(ã�ҽ��ϴ�.) %011d\n", tmpNode->data.phoneNum);
			}
			else {
				int newPhoneNum;
				Data * newData = (Data*)malloc(sizeof(Data));
				printf("(�������� �ʴ� �̸��Դϴ�. ��ȭ��ȣ�� �Է��ϼ���.)\n");
				printf("��ȭ��ȣ >>> ");
				scanf_s("%d", &newPhoneNum);
				printf("(%s %011d �� �ԷµǾ����ϴ�.)\n",commandStr,newPhoneNum);
				getchar();
				strcpy_s(newData->name, NAME_LEN + 1, commandStr);
				newData->phoneNum = newPhoneNum;
				insertNodeToHead(&head, *newData); numofData++;
				free(newData);
				newData = NULL;
			}
		}
		else {
			printf("(�߸��� ��ɾ��Դϴ�. �ٽ� �Է��ϼ���.)\n");
		}
	}
	return 0;
}