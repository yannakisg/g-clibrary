#include <stdio.h>

#include "../src/queue.h"

#define BLOCKS 20

void traverseQueue(Queue *queue) {
	if (queue == NULL) {
		errno = EINVAL;
		return;
	}
	
	QueueElement *iter;
	int i = queue->size - 1;
	for (iter = queue->tail; iter != NULL; iter = iter->prev) 
		printf("Element(%d) => %d\n", i--, *((int *)iter->data));
}

int main() {
	Queue queue;
	int d0,d1,d2,d3,d4;
	
	d0 = 20;
	d1 = 2;
	d2 = 493;
	d3 = 3938;
	d4 = 932837;
	
#ifdef MEMPOOL
	allocMem(BLOCKS,sizeof(QueueElement));
#endif
	
	if ( initQueue(&queue, NULL) != 0 ) {
		fprintf(stderr, "An error occurred while initializing a queue\n");
		return -1;
	}
	
	if ( put(&queue , &d0) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( put(&queue , &d1) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( put(&queue, &d2) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( put(&queue , &d3) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	if ( put(&queue , &d4) != 0) {
		fprintf(stderr, "An error occurred while adding an element\n");
		return -1;
	}
	
	int *d5;
	get(&queue, (void **)&d5);
	
	printf("First => %d\n",*d5);
	
	traverseQueue(&queue);
	
	destroyQueue(&queue);

#ifdef  MEMPOOL	
	freeMemory();
#endif	
	
	return 0;
}

