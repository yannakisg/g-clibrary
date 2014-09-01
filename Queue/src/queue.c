#include "queue.h"

/* Initializes a queue */
int initQueue(Queue *queue, void (*destroy) (void *data)) {
	
	if (queue == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	queue->size = 0;
	queue->head = queue->tail = NULL;
	queue->destroy = destroy;
	
	return 0;
}

/* Adds a QueueElement to the tail of the queue */
int put(Queue *queue, void *data) {
	if (queue == NULL || data == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	/* Creates the new element */
	QueueElement *newElmnt = (QueueElement *) malloc(sizeof(QueueElement));
	if (newElmnt == NULL) {
		errno = ENOMEM;
		return -2;
	}
	
	newElmnt->data = data;
	newElmnt->next = newElmnt->prev = NULL;
	
	if (queue->size == 0) {
		queue->head = queue->tail = newElmnt;
	} else {
		newElmnt->next = queue->head;
		queue->head->prev = newElmnt;
		queue->head = newElmnt;
	}
	
	queue->size++;
	
	return 0;
}

/* Removes a QueueElement from the head of the queue */
int get(Queue *queue, void **data) {
	if (queue == NULL || queue->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	QueueElement *rem = queue->tail;
	
	if (queue->size == 1) {
		queue->head = queue->tail = NULL;
	} else {
		
		queue->tail = rem->prev;
		queue->tail->next = NULL;
	}
	
	*data = rem->data;
	free(rem);
	
	queue->size--;
	
	return 0;
}

/* Destroys the queue
 *	Attention: If you use MemPool then after destroyQueue you must call freeMemory()
 */
void destroyQueue(Queue *queue) {
	if (queue == NULL) {
		errno = EINVAL;
		return;
	}
	
	void *data;
	
	while (queue->size > 0) {
		if (get(queue, &data) == 0 && queue->destroy != NULL) 
			queue->destroy(data);
	}
	
	memset(queue, 0, sizeof(Queue));
}
