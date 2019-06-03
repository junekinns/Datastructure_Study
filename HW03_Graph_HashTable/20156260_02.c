#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
//	뒤의 숫자는 이 단어를 찾기 위해 이진 탐색 트리에서 비교를 몇 번 했는지, 또는 해싱 테이블 또는 단어사전에 몇 번 접근했는지를 표시한다
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
	return 0;
}