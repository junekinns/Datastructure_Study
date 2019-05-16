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

Node * findWithName(Node * list, const char * name); //�̸����� ã��
void add_first(Node ** list, Data item); //�߰��ϱ�
Data deleteNode(Node * list, Node * pos); //�����ϱ�
void sortList(Node ** list); //�����ϱ�
void display(Node * list); //��Ϻ���
#endif // !__Phonebook_H__