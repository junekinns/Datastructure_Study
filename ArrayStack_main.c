#include <stdio.h>
#include "ArrayStack.h"
int main() {
	Stack stack;
	StackInit(&stack);
	for (int i = 1; i <= STACK_LEN; i++) {
		SPush(&stack, i);
	}

	while (!SIsEmpty(&stack)) {
		printf("%.3d\n",SPop(&stack));
	}
	return 0;
}