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
	int studentNum; //학번 : 13-19년도에 입학한 학생의 9자리 숫자
	int phoneNum; //전화번호 : 010으로 시작하는 11자리 숫자
	char name[NAME_LEN + 1]; //이름 : 10자 크기의 영문자 임의의 문자
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

//두 head(학번, 이름 기준)에 새 Data를 추가하는 함수
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
	else { //위치에 맞게 각자 찾아가도록.
		stunum_pNode = *stunum_head;
		name_pNode = *name_head;

		//학번 순 처리
		if (newNode->data.studentNum <= stunum_pNode->data.studentNum) { //새로운 노드가 더 작은 경우
			newNode->pNext_num = stunum_pNode;
			stunum_pNode = newNode;
			*stunum_head = stunum_pNode; //새 노드가 헤드가 되어야 함
		}
		else { //새로운 노드가 중간이거나, 가장 큰 경우
			while (NULL != stunum_pNode->pNext_num) { //끝까지 탐색
			if (newNode->data.studentNum <= stunum_pNode->pNext_num->data.studentNum) { //중간
				newNode->pNext_num = stunum_pNode->pNext_num;
				stunum_pNode->pNext_num = newNode;
				break; //자리를 찾고 삽입했으니 탈출
			}
			stunum_pNode = stunum_pNode->pNext_num;
			}
			if (NULL == stunum_pNode->pNext_num) {//끝
				stunum_pNode->pNext_num = newNode;
				newNode->pNext_num = NULL;
			}
		}

		//이름 순 처리
		if (strcmp(newNode->data.name, name_pNode->data.name) <= 0) { //새로운 노드가 사전순으로 앞선경우
			newNode->pNext_name = name_pNode;
			name_pNode = newNode;
			*name_head = name_pNode; //새 노드가 헤드가 되어야 함
		}
		else { //새로운 노드가 중간이거나, 가장 큰 경우
			while (NULL != name_pNode->pNext_name) { //끝까지 탐색
				if (strcmp(newNode->data.name, name_pNode->pNext_name->data.name) <= 0) { //중간
					newNode->pNext_name = name_pNode->pNext_name;
					name_pNode->pNext_name = newNode;
					break; //자리를 찾고 삽입했으니 탈출
				}
				name_pNode = name_pNode->pNext_name;
			}
			if (NULL == name_pNode->pNext_name) {//끝
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
		if(index % OFFSET == 0) //OFFSET 당 한번씩만 출력
			printf("%05d. 이름:%s\t학번:%d\t전화번호:%011d\n", index, pNode->data.name, pNode->data.studentNum, pNode->data.phoneNum);
		index++;
		pNode = pNode->pNext_name;
	}
}
void stunumOrder_displayList(ListNode * head) {
	int index = 1;
	ListNode * pNode = head;
	while (NULL != pNode) {
		if (index % OFFSET == 0) //OFFSET 당 한번씩만 출력
			printf("%05d. 학번:%d\t이름:%s\t전화번호:%011d\n", index, pNode->data.studentNum, pNode->data.name, pNode->data.phoneNum);
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
		printf("(1) 학번순\n");
		printf("(2) 이름순\n");
		printf("메뉴 선택(0: 종료) : ");
		scanf_s("%d", &menu);
		switch (menu)
		{
		case 0:
			printf("프로그램을 종료합니다.\n");
			exit(0);
			break;
		case 1:
			printf("학번 순으로 %d명의 데이터를 출력합니다. %d개 단위로 출력됩니다.\n", MAX_STUDENT_NUM, OFFSET);
			stunumOrder_displayList(stunum_head);
			break;
		case 2:
			printf("이름 순으로 %d명의 데이터를 출력합니다. %d개 단위로 출력됩니다.\n", MAX_STUDENT_NUM, OFFSET);
			nameOrder_displayList(name_head);
			break;
		}
	}
	return 0;
}