#ifndef __LinkedList_H__
#define __LinkedList_H__
#define TRUE 1
#define FALSE 0
#include "SinglyLinkedList.h"

void add_last(Node * list, Data item);
void add_first(Node ** list, Data item);
void add(Node ** list, Node * pos, Data item);
Data delete(Node * list, Node * pos);
void clear(Node * list);
void replace(Node ** list, Node * pos, Data item);
int is_in_list(Node ** list, Data item);
Data get_entry(Node ** list, Node *pos);
int get_length(Node * list);
int is_empty(Node * list);
int is_full(Node * list);
void display(Node * list);
#endif // !__LinkedList_H__