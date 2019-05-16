#ifndef __LinkedList_H__
#define __LinkedList_H__
#define TRUE 1
#define FALSE 0
typedef int Data;
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
void replace(Node ** list, Node * pos, Data item);
int is_in_list(Node ** list, Data item);
Data get_entry(Node ** list, Node *pos);
int get_length(Node * list);
int is_empty(Node * list);
int is_full(Node * list);
void display(Node * list);
#endif // !__LinkedList_H__