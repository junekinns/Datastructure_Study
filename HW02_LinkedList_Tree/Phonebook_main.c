#include <stdio.h>
#include <stdlib.h>
#include "Phonebook.h"
#define TRUE 1
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
char * nameTable[10] = {"�ֹν�","������","������","�輺��","������","������","������","������","������","������"};
//���������� 1�� �����ϴ� �Լ� �����.
//�ߺ��ȵǵ���, 
int main() {
	int i;
	Node * pHead = NULL;
	Data data;
	for (i = 0; i < 10; i++) {
		add_first(&pHead, data); //���������� 1�� �����ϴ� �Լ� �����.
	}
	return 0;
}