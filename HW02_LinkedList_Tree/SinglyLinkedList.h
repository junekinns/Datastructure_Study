#ifndef __SinglyLinkedList_H__
#define __SinglyLinkedList_H__
#define TRUE 1
#define FALSE 0
#define NAME_LEN 10
#define PHONENUM_LEN 14
typedef struct _phoneData {
	char name[NAME_LEN];
	char phoneNum[PHONENUM_LEN];
}PhoneData;
typedef PhoneData Data;
typedef struct _node {
	Data data;
	struct _node * link;
}SNode;
typedef SNode Node;
void add_last(Node * list, Data item);
void add_first(Node ** list, Data item);
void add(Node ** list, Node * pos, Data item);
Data deleteNode(Node * list, Node * pos);
void clear(Node * list);
void replace(Node ** list, Node * pos, Data item); //미구현
int is_in_list(Node ** list, Data item); //미구현
Data get_entry(Node ** list, Node *pos);//미구현
int get_length(Node * list);
int is_empty(Node * list);
int is_full(Node * list); //미구현
void display(Node * list);
#endif // !__SinglyLinkedList_H__