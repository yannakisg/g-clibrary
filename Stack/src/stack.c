#include "stack.h"

/* Initializes a stack */
int initStack(Stack *stack, void (*destroy) (void *data)) {
	if (stack == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	stack->size = 0;
	stack->head = NULL;
	stack->destroy = destroy;
	
	return 0;
}

/* Adds a StackElement to the top of the stack */
int push(Stack *stack, void *data) {
	if (stack == NULL || data == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	/* Creates the new top element */
	StackElement *newElmnt = (StackElement *) malloc(sizeof(StackElement));
	if (newElmnt == NULL) {
		errno = ENOMEM;
		return -2;
	}
	
	newElmnt->data = data;
	newElmnt->next = stack->head;
	stack->head = newElmnt;
	
	stack->size++;
	
	return 0; 
}

/* Removes a StackElement from the top of the stack */
int pop(Stack *stack, void **data) {
	if (stack == NULL || stack->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	StackElement *rem = stack->head;
	stack->head = rem->next;
	
	*data = rem->data;
	free(rem);
	
	stack->size--;
	
	return 0;
}

/* Destroys the stack 
 *	Attention: If you use MemPool then after destroyStack you must call freeMemory()
 */ 
void destroyStack(Stack *stack) {
	if (stack == NULL) {
		errno = EINVAL;
		return;
	}
	
	void *data;
	
	while (stack->size > 0) {
		if ( pop(stack, &data) == 0 && stack->destroy != NULL) 
			stack->destroy(data);
	}
	
	memset(stack, 0, sizeof(stack));
}
