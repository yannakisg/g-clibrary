#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* Uncomment the below lines to use MemPool instead of system's default functions malloc and free */ 

#include "../../MemPool/src/mempool.h"
#define MEMPOOL 

#ifdef MEMPOOL
	#define malloc(size) getMemNode()
	#define free(ptr) releaseMemNode(ptr)
#endif

typedef unsigned int UINT;

/* Queue Element */
typedef struct QueueElement_t {
	void *data;
	struct QueueElement_t *next;
	struct QueueElement_t *prev;
}QueueElement;

/* Queue
 *	destroy -> Appropriate function in order to free an element's data  
 *	tail -> The item at the front of the queue
 * 	head -> The item at the back of the queue
 */ 
typedef struct Queue_t {
	UINT size;
	QueueElement *tail; 
	QueueElement *head;
	void (*destroy) (void *data);
}Queue;

/* Initializes a queue */
int initQueue(Queue *queue, void (*destroy) (void *data));

/* Adds a QueueElement to the tail of the queue */
int put(Queue *queue, void *data);

/* Removes a QueueElement from the head of the queue */
int get(Queue *queue, void **data);

/* Destroys the queue
 *	Attention: If you use MemPool then after destroyQueue you must call freeMemory()
 */
void destroyQueue(Queue *queue);

/* Returns the size of the queue */
#define Queue_size(queue) ((queue)->size)
/* Returns the front element of the queue */
#define Queue_get(queue) ((queue)->tail == NULL ? NULL : (queue)->tail->data)

#endif
