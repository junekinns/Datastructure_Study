#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define STACK_LEN 100
#define TRUE 1
#define FALSE 0
typedef struct _intchar {
	int nData;
	int isInt;
}IntOrChar;
typedef IntOrChar Data;
typedef int bool;
typedef struct _stack {
	Data stackArr[STACK_LEN];
	int topIdx;
}Stack;
void StackInit(Stack * pStack);
int SIsEmpty(Stack * pStack);
void push(Stack *pStack, Data data);
Data pop(Stack * pStack);
Data peek(Stack * pStack);
int Evaluate(Data * postExp);

//Stack Functions
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
	case '$':
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

int priComp(char op1, char op2) {
	int op1Pri = getOpPriority(op1);
	int op2Pri = getOpPriority(op2);
	if (op1Pri > op2Pri) return 1;
	else if (op1Pri < op2Pri) return -1;
	else return 0; //op1Pri == op2Pri
}

int Evaluate(Data * postExp, size_t arr_size) {
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
				result.nData = num1 / num2;
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

void calc(char exp[]) {
	Stack stack;
	int i = 0, j = 0, newExpIdx = 0, postExpIdx=0;
	int expLen = strlen(exp);
	IntOrChar *newExp = (IntOrChar*)malloc(sizeof(IntOrChar) * expLen); //만약 1자리수 이상의 숫자가 많거나 **가 많으면 조금 여유로운 공간.
	IntOrChar *postExp = (IntOrChar*)malloc(sizeof(IntOrChar) * expLen);
	//ascii of / : 47
	while (exp[i] != '\0') {
		if (isdigit(exp[i])) {
			j = i;
			while (TRUE) {
				if (!isdigit(exp[j])) break;
				j++;
			}
			int tmpTokLen = j - i + 1; // +1 for '\0'
			if (tmpTokLen <= 2) {
				newExp[newExpIdx].nData = exp[j - 1] - '0';
				newExp[newExpIdx].isInt = TRUE;
				newExpIdx++;
			}
			else {
				char * tmpTok = (char*)malloc(tmpTokLen);
				for (int k = 0; k < tmpTokLen; k++) {
					tmpTok[k] = exp[i++];
				}
				newExp[newExpIdx].nData = atoi(tmpTok);
				newExp[newExpIdx].isInt = TRUE;
				newExpIdx++;
				if (NULL != tmpTok)
					free(tmpTok);
			}
			i = j;
		}
		else { //Operators
			if (exp[i] != '*') {
				newExp[newExpIdx].nData = (int)exp[i++];
				newExp[newExpIdx].isInt = FALSE;
				newExpIdx++;
			}
			else {
				j = i;
				while (TRUE) {
					if (exp[j] != '*') break;
					j++;
				}
				int opLen = j - i + 1; // +1 for '\0'
				if (opLen == 2) {
					newExp[newExpIdx].nData = (int)exp[i++];
					newExp[newExpIdx].isInt = FALSE;
					newExpIdx++;
				}
				else { //** -> $
					newExp[newExpIdx].nData = (int)'$';
					newExp[newExpIdx].isInt = FALSE;
					newExpIdx++;
				}
				i = j;
			}
		}
	}
	IntOrChar tok, popOp;
	//문제 : 두자릿수 숫자와 ** 처리
	StackInit(&stack);

	for (i = 0; i < newExpIdx; i++) {
		tok = newExp[i];
		//Do Something and push into postExp
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
				while(TRUE) {
					popOp = pop(&stack);
					if ((char)popOp.nData == '(') break;
					postExp[postExpIdx++] = popOp;
				} //여기서 ( 못만나면 처리 해야함.
				break;
			case '+': case '-': case '*': case '/': case '%': case '$': //case '**':
				while (!SIsEmpty(&stack) && priComp((char)peek(&stack).nData, (char)tok.nData) >= 0) //스택이 비지 않았고, 스택에 이미 있는 것이 우선순위가 더 높다면
					postExp[postExpIdx++] = pop(&stack); //스택에 있던 연산자를 팝
				push(&stack, tok);
				break;
			}
		}
	}

	while (!SIsEmpty(&stack))
		postExp[postExpIdx++] = pop(&stack);
	//Print the Postfix result
	for (int i = 0; i < postExpIdx; i++) {
		if (postExp[i].isInt) printf("%d ", postExp[i].nData);
		else printf("%c ", (char)postExp[i].nData);
	}
	printf(" = %d",Evaluate(postExp, postExpIdx));
	//return postExp;
}

int main() {
	char exp3[] = "((1-2)+3)*(5-2)";
	calc(exp3);
	//free(postExp);
	return 0;
}