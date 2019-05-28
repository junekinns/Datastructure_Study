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
	int phoneNum; //전화번호 : 010으로 시작하는 11자리 숫자
	char name[NAME_LEN+1]; //이름 : 3자 크기의 영문자 임의의 문자
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
//이름 : 10자 크기의 영문자 임의의 문자
void nameGenerator(char * nameArr, int len) {
	int i;
	for (i = 0; i < len; i++) {
		nameArr[i] = (char)(rand() % 26) + 'A';
	}
	nameArr[i] = '\0';
}
//head에 새 Data를 추가하는 함수
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
//이름으로 탐색 후 삭제하는 함수. 성공하면 True, 실패하면 False 반환
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

//두개의 정렬된 연결 리스트를 하나로 합치는 함수
ListNode * SortedMerge(ListNode * a, ListNode * b)
{
	if (NULL == a)
		return b;
	else if (NULL == b)
		return a;

	ListNode * result = NULL;

	//if (a->data.phoneNum <= b->data.phoneNum) 전화번호기준 정렬
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


//주어진 노드를 앞/뒤 반절로 나눈다.
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

	//fast포인터가 2번갈때, slow포인터는 1번 전진시킨다.
	while (NULL != fast)
	{
		fast = fast->pNext;
		if (NULL != fast)
		{
			slow = slow->pNext;
			fast = fast->pNext;
		}
	}

	//slow포인터가 리스트의 중간에 위치한다. 거기서 둘로 쪼갠다.
	*frontRef = source;
	*backRef = slow->pNext;
	slow->pNext = NULL;
}

void MergeSort_List(ListNode** head)
{
	//길이가 0이나 1인경우
	if (NULL == *head || NULL == (*head)->pNext)
		return;

	ListNode * a = NULL;
	ListNode * b = NULL;

	// head를 a,b에 둘로 나눈다.
	FrontBackSplit(*head, &a, &b);

	// 나누어진 sublist에 대해 재귀적으로 다시 정렬한다.
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
		printf("(%d명의 데이터가 있습니다.)\n", numofData);
		printf("(명령어를 입력하세요. 이름형식은 영문 알파벳 3글자로 제한됩니다.)\n");
		printf("(프로그램을 종료하려면 \"종료\" 를 입력하세요.)\n");
		printf(">> ");
		gets_s(commandStr, MAX_COMMAND_LEN);
		if (strlen(commandStr) == 4) {
			if (strcmp(commandStr, "목록") == 0) {
				displayList(head);
			}
			else if((strcmp(commandStr, "정렬") == 0)){
				MergeSort_List(&head);
				displayList(head);
			}
			else if ((strcmp(commandStr, "종료") == 0)) {
				exit(0);
			}
		}
		else if (strlen(commandStr) > 4) {
			char delName[NAME_LEN + 1];
			strcpy_s(delName, sizeof(delName), &commandStr[5]);//삭제 이름 에서 이름 부분만 떼오기
			int delNum = deleteNodebyName(&head, delName);
			if (0 != delNum) {
				numofData--;
				printf("(%s %011d 가 삭제되었습니다.)\n", &commandStr[5], delNum);
			} else {
				printf("(삭제 실패!)\n");
			}
		}
		else if (strlen(commandStr) == NAME_LEN){
			ListNode * tmpNode = findWithName(head, commandStr);
			if (tmpNode != NULL) {
				//찾았습니다. 전화번호 보여주기
				printf("(찾았습니다.) %011d\n", tmpNode->data.phoneNum);
			}
			else {
				int newPhoneNum;
				Data * newData = (Data*)malloc(sizeof(Data));
				printf("(존재하지 않는 이름입니다. 전화번호를 입력하세요.)\n");
				printf("전화번호 >>> ");
				scanf_s("%d", &newPhoneNum);
				printf("(%s %011d 가 입력되었습니다.)\n",commandStr,newPhoneNum);
				getchar();
				strcpy_s(newData->name, NAME_LEN + 1, commandStr);
				newData->phoneNum = newPhoneNum;
				insertNodeToHead(&head, *newData); numofData++;
				free(newData);
				newData = NULL;
			}
		}
		else {
			printf("(잘못된 명령어입니다. 다시 입력하세요.)\n");
		}
	}
	return 0;
}