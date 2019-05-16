#include <stdio.h>
#include <stdlib.h>
#include "Phonebook.h"
#define TRUE 1
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
char * nameTable[10] = {"최민식","하정우","조진웅","김성균","마동석","곽도원","김혜은","김응수","김종구","윤종빈"};
//랜덤데이터 1개 생성하는 함수 만들기.
//중복안되도록, 
int main() {
	int i;
	Node * pHead = NULL;
	Data data;
	for (i = 0; i < 10; i++) {
		add_first(&pHead, data); //랜덤데이터 1개 생성하는 함수 만들기.
	}
	return 0;
}