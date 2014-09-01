#include <stdio.h>

#include "../src/stack.h"

#define BLOCKS 20

void traverseStack(Stack *stack) {
	if (stack == NULL) {
		errno = EINVAL;
		return;
	}
	
	StackElement *iter;
	int i = 0;
	for (iter = stack->head; iter != NULL; iter = iter->next) 
		printf("Element(%d) => %d\n", i++, *((int *)iter->data));
}

int main() {
	Stack stack;
	int d0,d1,d2,d3,d4;
	
	
	d0 = 20;
	d1 = 2;
	d2 = 493;
	d3 = 3938;
	d4 = 932837;
	
#ifdef MEMPOOL
	allocMem(BLOCKS,sizeof(StackElement));
#endif

	if ( initStack(&stack, NULL) != 0 ) {
		fprintf(stderr, "An error occurred while initializing a stack\n");
		return -1;
	}
	
	if ( push(&stack , &d0) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( push(&stack , &d1) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( push(&stack , &d2) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( push(&stack , &d3) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( push(&stack , &d4) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	
	int *d5;
	pop(&stack, (void **)&d5);
	
	printf("Top => %d\n",*d5);
	
	traverseStack(&stack);
	
	destroyStack(&stack);
	
#ifdef MEMPOOL	
	freeMemory();
#endif
	return 0;
}
