#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Uncomment the below lines to use MemPool instead of system's default functions malloc and free */ 

//#include "../../MemPool/src/mempool.h"
//#define MEMPOOL 

#ifdef MEMPOOL
	#define malloc(size) getMemNode()
	#define free(ptr) releaseMemNode(ptr)
#endif
	

typedef unsigned int UINT;
 
 /* Stack Element */
typedef struct StackElement_t {
	void *data;
	struct StackElement_t *next;
}StackElement;

/* Stack
 *	destroy -> Appropriate function in order to free an element's data  
 */ 
typedef struct Stack_t {
	StackElement *head;
	UINT size;
	void (*destroy) (void *data);
}Stack;

/* Initializes a stack */
int initStack(Stack *stack, void (*destroy) (void *data));

/* Adds a StackElement to the top of the stack */
int push(Stack *stack, void *data);

/* Removes a StackElement from the top of the stack */
int pop(Stack *stack, void **data);

/* Destroys the stack 
 *	Attention: If you use MemPool then after destroyStack you must call freeMemory()
 */  
void destroyStack(Stack *stack);

/* Returns the size of the stack */
#define Stack_size(stack) ((stack)->size)
/* Returns the top element of the stack */
#define Stack_peek(stack) ((stack)->head == NULL ? NULL : (stack)->head->data)

#endif
