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
	case '$': //$는 **를 치환한 문자이다. 우선순위가 가장 높다.
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

int priComp(char op1, char op2) { //스택에서 연산자끼리 우선순위를 비교하는 함수
	int op1Pri = getOpPriority(op1);
	int op2Pri = getOpPriority(op2);
	if (op1Pri > op2Pri) return 1;
	else if (op1Pri < op2Pri) return -1;
	else return 0; //op1Pri == op2Pri(우선순위가 같은 경우)
}

Data * convertToPostfix(const char copiedExp[], int * arr_size) {
	if (NULL == copiedExp) return;
	Stack stack;
	int i = 0, j = 0, newExpIdx = 0, postExpIdx = 0;
	int expLen = strlen(copiedExp);
	Data *newInExp = (Data*)malloc(sizeof(Data) * expLen); //숫자 자릿수와 **을 고려한 새로운 배열 생성, 만약 1자리수 이상의 숫자가 많거나 **가 많으면 공간이 조금 남게 됨.
	Data *postExp = (Data*)malloc(sizeof(Data) * expLen); //위에 생성한 자릿수를 고려한 중위표기식 배열을 후위표기식으로 바꾸어 저장할 배열.

	while (copiedExp[i] != '\0') { //문자열의 끝 까지 반복
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
	//1.먼저, 연산자와 숫자에 이상이 있는지 검사한다.
	for (i = 0; i < newExpIdx; i++) {
		//현재 가리키고있는 연산자가 괄호가 아니면서, 다음에 위치한 연산자도 괄호가 아닌 연산자면 에러!
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
	//2.연산자에 이상이 없으면 괄호에 이상이 있는지 검사한다.
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
			//'('을 만나지 않고 ')'을 만난 경우! 무조건 에러
			expError(newInExp, newExpIdx, i, '(');
			free(newInExp);
			free(postExp);
			return NULL;
		}
	}

	Data tok, popOp;
	StackInit(&stack);
	//newExpIdx를 후위방식으로 변환하는 반복문
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
			case '+': case '-': case '*': case '/': case '%': case '$': //$은 '**'를 변환한 것.
				while (!SIsEmpty(&stack) && priComp((char)peek(&stack).nData, (char)tok.nData) >= 0) //스택이 비지 않았고, 스택에 이미 있는 것이 우선순위가 더 높다면
					postExp[postExpIdx++] = pop(&stack); //스택에 있던 연산자를 팝
				push(&stack, tok);
				break;
			}
		}
	}
	while (!SIsEmpty(&stack))
		postExp[postExpIdx++] = pop(&stack);
	////후위표기식 출력(디버깅 용)
	//for (int i = 0; i < postExpIdx; i++) {
	//   if (postExp[i].isInt) printf("%d ", postExp[i].nData);
	//   else printf("%c ", (char)postExp[i].nData);
	//}
	*arr_size = postExpIdx; //최종 후위식의 사이즈를 간접참조연산으로 할당. 살짝 찝찝한 코드.
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
	if (NULL == postExpCpy) return NULL; //이게 왜 되지?
	result = EvaluatePostExp(postExpCpy, arr_size);
	free(expCpy);
	return result;
}

void expError(const Data * errorExp, const int error_end_Idx, const int errorIdx, const char ch_error) {
	Stack stack;
	StackInit(&stack);
	Data tmpData;
	int i = error_end_Idx; //수식의 끝 Index ('\0')
	//수식의 끝 부분부터 에러위치를 검사하며 스택에 저장한다.
	//역순으로 스택에 저장했기 때문에, 모두 pop을 하면 원래 수식의 순서에 맞게 에러표시(!)와 함께 출력된다.
	while (i >= 0) {
		if (i == errorIdx) { //i가 에러가 발생한 위치이면
		   //그 위치 바로 뒤에 (!)을 출력 한다.
			tmpData.nData = ')';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			tmpData.nData = '!';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			tmpData.nData = '(';
			tmpData.isInt = FALSE;
			push(&stack, tmpData);

			if (i == error_end_Idx) //i가 에러가 발생한 위치이면서 끝 위치면('\0')
				i--; //스택에 push 하지 않는다.(출력하지 않는다.)
			else
				push(&stack, errorExp[i--]);
		}
		else {//i가 에러가 발생한 위치가 아니면,
			if (i == error_end_Idx)//i가 에러가 발생한 위치가 아니면서 끝 위치면('\0')
				i--; //스택에 push 하지 않는다.(출력하지 않는다.)
			else
				push(&stack, errorExp[i--]);
		}
	}
	i = 0;
	//Stack에서 꺼내면서 수식 출력
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
	printf(" 이 위치에 오류가 있습니다.");
	//전달받은 ch_error의 종류에 따라 메시지가 다르다. 
	switch (ch_error)
	{
	case '(': case ')':
		printf(" %c가 부족합니다.\n", ch_error);
		break;
	case -1:
		printf(" 숫자가 와야 합니다.\n");
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