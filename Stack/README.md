Stack 0.1

Description:
A simple stack implementation. 


stack->head -->	|StackElement| ---> |StackElement| --> |StackElement|


Example:
#include "src/stack.h"

int main() {
// ...
	Stack stack;
	initStack(&stack, destroy);
// ...
	push(&stack , (void *)data0);
	push(&stack , (void *)data1);
// ...
	pop(&stack, (void **)retValue);
	destroyStack(&stack);
// ...
	return 0;
}

* Stack was checked for memory leaks with Valgrind-3.5.0

