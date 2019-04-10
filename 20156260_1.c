#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define STACK_LEN 100
#define TRUE 1
#define FALSE 0
#define INF 2147483648
//typedef, struct, Functions Declaration
typedef int bool;
typedef struct _intorchar {
	int nData;
	bool isInt;
}IntOrChar;
typedef IntOrChar Data;
typedef struct _stack {
	Data stackArr[STACK_LEN];
	int topIdx;
}Stack;

//Functions for Stack
void StackInit(Stack * pStack);
bool SIsEmpty(Stack * pStack);
void push(Stack *pStack, Data data);
Data pop(Stack * pStack);
Data peek(Stack * pStack);
int getOpPriority(char op);
int priComp(char op1, char op2);

//Functions for Calculator
Data * convertToPostfix(const char copiedExp[], int * arr_size);
int EvaluatePostExp(Data * postExp, int arr_size);
int EvaluateInExp(const char inExp[]);
void expError(const Data * errorExp, const int error_end_Idx, const int errorIdx, const char ch_error);
//typedef, struct, Functions Declaration

void StackInit(Stack * pStack) {
	pStack->topIdx = -1;
}

bool SIsEmpty(Stack * pStack) {
	if (-1 == pStack->topIdx) return TRUE;
	else return FALSE;
}

void push(Stack *pStack, Data data) {
	if (STACK_LEN == pStack->topIdx) {
		printf("Stack Overflow!\n");
		return;
	}
	pStack->stackArr[++(pStack->topIdx)] = data;
}

Data pop(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack Underflow!\n");
		return;
	}
	return pStack->stackArr[(pStack->topIdx)--];
}

Data peek(Stack * pStack) {
	if (SIsEmpty(pStack)) {
		printf("Stack is empty!\n");
		return;
	}
	return pStack->stackArr[(pStack->topIdx)];
}

int getOpPriority(char op) {
	switch (op)
	{
	case '$': //$�� **�� ġȯ�� �����̴�. �켱������ ���� ����.
		return 4;
	case '*':
	case '/':
	case '%':
		return 3;
	case '+':
	case '-':
		return 2;
	case '(':
		return 1;
	}
	return -1;
}

int priComp(char op1, char op2) { //���ÿ��� �����ڳ��� �켱������ ���ϴ� �Լ�
	int op1Pri = getOpPriority(op1);
	int op2Pri = getOpPriority(op2);
	if (op1Pri > op2Pri) return 1;
	else if (op1Pri < op2Pri) return -1;
	else return 0; //op1Pri == op2Pri(�켱������ ���� ���)
}

Data * convertToPostfix(const char copiedExp[], int * arr_size) {
	if (NULL == copiedExp) return;
	Stack stack;
	int i = 0, j = 0, newExpIdx = 0, postExpIdx = 0;
	int expLen = strlen(copiedExp);
	Data *newInExp = (Data*)malloc(sizeof(Data) * expLen); //���� �ڸ����� **�� ����� ���ο� �迭 ����, ���� 1�ڸ��� �̻��� ���ڰ� ���ų� **�� ������ ������ ���� ���� ��.
	Data *postExp = (Data*)malloc(sizeof(Data) * expLen); //���� ������ �ڸ����� ����� ����ǥ��� �迭�� ����ǥ������� �ٲپ� ������ �迭.

	while (copiedExp[i] != '\0') { //���ڿ��� �� ���� �ݺ�
		if (isdigit(copiedExp[i])) {
			j = i;
			while (TRUE) {
				if (!isdigit(copiedExp[j])) break;
				j++;
			}
			int tmpTokLen = j - i + 1; // +1 for '\0'
			if (tmpTokLen == 2) {
				newInExp[newExpIdx].nData = copiedExp[j - 1] - '0';
				newInExp[newExpIdx].isInt = TRUE;
				newExpIdx++;
			}
			else {
				char * tmpTok = (char*)malloc(tmpTokLen);
				for (int k = 0; k < tmpTokLen - 1; k++) {
					tmpTok[k] = copiedExp[i++];
				}
				newInExp[newExpIdx].nData = atoi(tmpTok);
				newInExp[newExpIdx].isInt = TRUE;
				newExpIdx++;
				if (NULL != tmpTok)
					free(tmpTok);
			}
			i = j;
		}
		else { //Operators
			if (copiedExp[i] != '*') {
				newInExp[newExpIdx].nData = (int)copiedExp[i++];
				newInExp[newExpIdx].isInt = FALSE;
				newExpIdx++;
			}
			else {
				j = i;
				while (TRUE) {
					if (copiedExp[j] != '*') break;
					j++;
				}
				int opLen = j - i + 1; // +1 for '\0'
				if (opLen == 2) {
					newInExp[newExpIdx].nData = (int)copiedExp[i++];
					newInExp[newExpIdx].isInt = FALSE;
					newExpIdx++;
				}
				else { //** -> $
					newInExp[newExpIdx].nData = (int)'$';
					newInExp[newExpIdx].isInt = FALSE;
					newExpIdx++; i += 2;
				}
				i = j;
			}
		}
	}
	//Error Check
	//1.����, �����ڿ� ���ڿ� �̻��� �ִ��� �˻��Ѵ�.
	for (i = 0; i < newExpIdx; i++) {
		//���� ����Ű���ִ� �����ڰ� ��ȣ�� �ƴϸ鼭, ������ ��ġ�� �����ڵ� ��ȣ�� �ƴ� �����ڸ� ����!
		if (!newInExp[i].isInt && ('(' != (char)newInExp[i].nData && ')' != (char)newInExp[i].nData)) { 
			if (i != newExpIdx - 1 && !newInExp[i + 1].isInt && ('(' != (char)newInExp[i + 1].nData && ')' != (char)newInExp[i + 1].nData))
			{
				expError(newInExp, newExpIdx, i + 1, -1);
				free(newInExp);
				free(postExp);
				return NULL;
			}
		}
	}
	//2.�����ڿ� �̻��� ������ ��ȣ�� �̻��� �ִ��� �˻��Ѵ�.
	for (i = 0, j = 0; i < newExpIdx; i++) {
		if ('(' == (char)newInExp[i].nData) {
			j = i + 1;
			while (j < newExpIdx) {
				if (')' == (char)newInExp[j++].nData) {
					i = j - 1;
					break;
				}
			}
			if (j == newExpIdx && ')' != (char)newInExp[j - 1].nData) {
				expError(newInExp, newExpIdx, j, ')');
				free(newInExp);
				free(postExp);
				return NULL;
			}
		}
		else if (')' == (char)newInExp[i].nData) {
			//'('�� ������ �ʰ� ')'�� ���� ���! ������ ����
			expError(newInExp, newExpIdx, i, '(');
			free(newInExp);
			free(postExp);
			return NULL;
		}
	}

	Data tok, popOp;
	StackInit(&stack);
	//newExpIdx�� ����������� ��ȯ�ϴ� �ݺ���
	for (i = 0; i < newExpIdx; i++) {
		tok = newInExp[i];
		if (tok.isInt) {
			postExp[postExpIdx++] = tok;
		}
		else {
			switch ((char)tok.nData)
			{
			case '(':
				push(&stack, tok);
				break;
			case ')':
				while (TRUE) {
					popOp = pop(&stack);
					if ((char)popOp.nData == '(') break;
					postExp[postExpIdx++] = popOp;
				}
				break;
			case '+': case '-': case '*': case '/': case '%': case '$': //$�� '**'�� ��ȯ�� ��.
				while (!SIsEmpty(&stack) && priComp((char)peek(&stack).nData, (char)tok.nData) >= 0) //������ ���� �ʾҰ�, ���ÿ� �̹� �ִ� ���� �켱������ �� ���ٸ�
					postExp[postExpIdx++] = pop(&stack); //���ÿ� �ִ� �����ڸ� ��
				push(&stack, tok);
				break;
			}
		}
	}
	while (!SIsEmpty(&stack))
		postExp[postExpIdx++] = pop(&stack);
	////����ǥ��� ���(����� ��)
	//for (int i = 0; i < postExpIdx; i++) {
	//   if (postExp[i].isInt) printf("%d ", postExp[i].nData);
	//   else printf("%c ", (char)postExp[i].nData);
	//}
	*arr_size = postExpIdx; //���� �������� ����� ���������������� �Ҵ�. ��¦ ������ �ڵ�.
	free(newInExp);
	return postExp;
}

int EvaluatePostExp(Data * postExp, int arr_size) {
	if (NULL == postExp) return;
	Stack stack;
	StackInit(&stack);
	for (int i = 0; i < arr_size; i++) {
		if (postExp[i].isInt) {
			push(&stack, postExp[i]);
		}
		else {
			int num2 = pop(&stack).nData;
			int num1 = pop(&stack).nData;
			Data result = { 1, TRUE };
			switch ((char)postExp[i].nData)
			{
			case '+':
				result.nData = num1 + num2;
				break;
			case '-':
				result.nData = num1 - num2;
				break;
			case '*':
				result.nData = num1 * num2;
				break;
			case '/':
				if (0 != num2)
					result.nData = num1 / num2;
				else if(0 == num2)
					result.nData = INF;
				break;
			case '%':
				result.nData = num1 % num2;
				break;
			case '$':
				while (num2--)
					result.nData *= num1;
				break;
			}
			push(&stack, result);
		}
	}
	free(postExp);
	return pop(&stack).nData;
}

int EvaluateInExp(const char inExp[]) {
	int len = strlen(inExp);
	int arr_size, result;
	char * expCpy = (char*)malloc(len + 1);
	strcpy(expCpy, inExp);
	Data * postExpCpy = convertToPostfix(expCpy, &arr_size);
	if (NULL == postExpCpy) return NULL; //�̰� �� ����?
	result = EvaluatePostExp(postExpCpy, arr_size);
	free(expCpy);
	return result;
}

void expError(const Data * errorExp, const int error_end_Idx, const int errorIdx, const char ch_error) {
	Stack stack;
	StackInit(&stack);
	Data tmpData;
	int i = error_end_Idx; //������ �� Index ('\0')
	//������ �� �κк��� ������ġ�� �˻��ϸ� ���ÿ� �����Ѵ�.
	//�������� ���ÿ� �����߱� ������, ��� pop�� �ϸ� ���� ������ ������ �°� ����ǥ��(!)�� �Բ� ��µȴ�.
	while (i >= 0) {
		if (i == errorIdx) { //i�� ������ �߻��� ��ġ�̸�
		   //�� ��ġ �ٷ� �ڿ� (!)�� ��� �Ѵ�.
			tmpData.nData = ')';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			tmpData.nData = '!';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			tmpData.nData = '(';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			if (i == error_end_Idx) //i�� ������ �߻��� ��ġ�̸鼭 �� ��ġ��('\0')
				i--; //���ÿ� push ���� �ʴ´�.(������� �ʴ´�.)
			else
				push(&stack, errorExp[i--]);
		}
		else {//i�� ������ �߻��� ��ġ�� �ƴϸ�,
			if (i == error_end_Idx)//i�� ������ �߻��� ��ġ�� �ƴϸ鼭 �� ��ġ��('\0')
				i--; //���ÿ� push ���� �ʴ´�.(������� �ʴ´�.)
			else
				push(&stack, errorExp[i--]);
		}
	}
	i = 0;
	//Stack���� �����鼭 ���� ���
	while (!SIsEmpty(&stack)) {
		tmpData = pop(&stack);
		if (tmpData.isInt)
			printf("%d", tmpData.nData);
		else {
			if ('$' == (char)tmpData.nData)
				printf("**");
			else
				printf("%c", (char)tmpData.nData);
		}
	}
	printf(" �� ��ġ�� ������ �ֽ��ϴ�.");
	//���޹��� ch_error�� ������ ���� �޽����� �ٸ���. 
	switch (ch_error)
	{
	case '(': case ')':
		printf(" %c�� �����մϴ�.\n", ch_error);
		break;
	case -1:
		printf(" ���ڰ� �;� �մϴ�.\n");
		break;
	}
	printf("\n");
	return;
}

int main() {
	char exp1[] = "2+3*4**2-1";
	char exp2[] = "2+(3*4)**2-12";
	char exp3[] = "2+(3*4**2-12";
	char exp4[] = "(11+3)*2**3-12";
	char exp5[] = "2+(3+*4**2-12";

	if (NULL != EvaluateInExp(exp1))
		printf("%s = %d\n\n", exp1, EvaluateInExp(exp1));

	if (NULL != EvaluateInExp(exp2))
		printf("%s = %d\n\n", exp2, EvaluateInExp(exp2));

	if (NULL != EvaluateInExp(exp3))
		printf("%s = %d\n\n", exp3, EvaluateInExp(exp3));

	if (NULL != EvaluateInExp(exp4))
		printf("%s = %d\n\n", exp4, EvaluateInExp(exp4));

	if (NULL != EvaluateInExp(exp5))
		printf("%s = %d\n\n", exp5, EvaluateInExp(exp5));

	return 0;
}