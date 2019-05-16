#ifndef __Phonebook_H__
#define __Phonebook_H__
#define NAME_LEN 10
#define PHONENUM_LEN 14

typedef struct _phoneData {
	char name[NAME_LEN];
	char phoneNum[PHONENUM_LEN];
}PhoneData;

typedef PhoneData Data;

typedef struct _singlyNode {
	Data data;
	struct _node * link;
}SNode;

typedef SNode Node;

Node * findWithName(Node * list, const char * name); //이름으로 찾기
void add_first(Node ** list, Data item); //추가하기
Data deleteNode(Node * list, Node * pos); //삭제하기
void sortList(Node ** list); //정렬하기
void display(Node * list); //목록보기
#endif // !__Phonebook_H__