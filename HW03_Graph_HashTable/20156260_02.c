#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
//
//51840 개의 단어로 이루어진 단어 파일이 제공된다.
//단어 파일은 단어와 뜻으로 이루어져 있다.
//예)  apple : n.사과
//단어 파일은 정렬되어 있지 않다.단어 파일을 가져와 메모리에 담아둔다.파일 입출력 방법을 미리 알아야 한다.
//단어를 입력하면 뜻을 표시하는 프로그램을 작성한다.
//단어 사전은 이진 탐색 트리와 해싱의 2가지 기법을 이용한다.프로그램 안에는 2개의 방식이 모두 있어야 한다.(선택이 아님)
//2가지 기법(탐색트리, 해싱)의 코드가 소스에 포함되어 있어야 한다.

//실행 예)
//단어 : apple
//	n.사과(13) < -이진 탐색 트리를 통해 찾은 단어
//	n.사과(6) < -해싱 테이블을 통해 찾은 단어
//	뒤의 숫자는 이 단어를 찾기 위해 이진 탐색 트리에서 비교를 몇 번 했는지, 또는 해싱 테이블 또는 단어사전에 몇 번 접근했는지를 표시한다.
/*
데이터가 있으면 각 데이터에 키가 있다. 51840개. 키는 알파벳(단어)
1.테이블을 어떻게, 몇개 만들지? ->51840개는 있으면 컬리젼이 한번도 안발생하면 하나씩 착착 들어갈 것이고, 500개면 100번정도 충돌이 발생할 것
해시테이블의 크기는 일반적으로 데이터의 크기보다 조금 더 크게 두는게 일반적이다(약 7만개 정도?)
->Trade-Off:공간을 더 쓴대신, 시간을 아끼자
충돌처리를 피할 수는 없을 것이다.(해시함수가 생각보다 잘 흐트려주지 않음) 
2.테이블을 배열로 할까? 연결리스트로 할까?(체이닝?)
3. 해시함수는 어떻게 만들까?
4. 충돌이 발생하면? 오버플로우가 발생하면?

*/
typedef int Data;
typedef struct bstNode {
	Data data;
	struct bstNode * pRight;
	struct bstNode * pLeft;
}BstNode;

//Functions for BST
void insertBNode(BstNode ** root, Data inputData);
void inorderBST(BstNode * pNode);
BstNode * searchByKey(BstNode * root, int key);

void insertBNode(BstNode ** root, Data inputData) {
	BstNode * newBt = (BstNode*)malloc(sizeof(BstNode));
	newBt->data = inputData;
	newBt->pLeft = NULL;
	newBt->pRight = NULL;
	BstNode * pRoot = *root;

	if (NULL == pRoot) {
		*root = newBt;
	}
	else {
		BstNode * pNode = pRoot;
		while (TRUE) {
			if (inputData < pNode->data) {
				if (NULL == pNode->pLeft) {
					pNode->pLeft = newBt;
					break;
				}
				else {
					pNode = pNode->pLeft;
				}
			}
			else if (inputData > pNode->data) {
				if (NULL == pNode->pRight) {
					pNode->pRight = newBt;
					break;
				}
				else {
					pNode = pNode->pRight;
				}
			}
		}
	}
}

void inorderBST(BstNode * pNode) {
	if (NULL == pNode) return;
	inorderBST(pNode->pLeft);
	printf("%d\n", pNode->data);
	inorderBST(pNode->pRight);
}

BstNode * searchByKey(BstNode * root, int key) {
	BstNode * pNode = root;
	while (TRUE) {
		if (NULL == pNode)
			return NULL;
		if (key == pNode->data) {
			break;
		}
		else {
			if (key < pNode->data)
				pNode = pNode->pLeft;
			else
				pNode = pNode->pRight;
		}
	}
	return pNode;
}
int main() {
	FILE * fp = NULL;
	char word[100];
	char wordtable[100][100];
	fp = fopen("new_dict.txt","r");
	if (NULL == fp) {
		printf("File open Error!\n");
		exit(0);
	}
	for (int i = 0; i < 10; i++) {
		fgets(word, 100, fp);
		strcpy(wordtable[i], word);
	}
	for (int i = 0; i < 10; i++) {
		printf("%s\n", wordtable[i]);
	}
	return 0;
}