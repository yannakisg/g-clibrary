Queue 0.1

Description:
A simple queue implementation. 


queue->head -->|QueueElement| ---> |QueueElement| --> |QueueElement| <-- queue->tail


Example:
#include "src/queue.h"

int main() {
// ...
	Queue queue;
	initQueue(&queue, destroy);
// ...
	put(&queue , (void *)data0);
	put(&queue , (void *)data1);
// ...
	get(&queue, (void **)retValue);
	destroyQueue(&queue);
// ...
	return 0;
}

* Queue was checked for memory leaks with Valgrind-3.5.0

