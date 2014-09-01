#include <stdio.h>
#include <time.h>

#include "../src/intervalheap.h"


void traverseHeap(IntervalHeap *heap) {
	if (heap == NULL) {
		errno = EINVAL;
		return ;
	}
	
	int i;
	for (i = 0; i < heap->size; i++) {
		printf("-----------------------\n");
		printf("Min: Element(%d) => %d , key => %d\n", i, *((int *)heap->table[i]->dataL),heap->table[i]->keyL);
		if (isFull(heap->table[i]))
			printf("Max: Element(%d) => %d , key => %d\n", i, *((int *)heap->table[i]->dataR),heap->table[i]->keyR);
		printf("-----------------------\n");
	}
}

#define SIZE 500
#define UPPERSIZE 10000
#define DEFAULTSIZE 20
#define THRESHOLD 0.75

int main() {
	IntervalHeap heap;
	
	initIntervalHeap(&heap, NULL, DEFAULTSIZE,THRESHOLD);
	
	srand(time(NULL));
	
	int sorted[SIZE],array[SIZE];
	int i;
	
	for (i = 0; i < SIZE; i++) {
		array[i] = (rand() % UPPERSIZE) + 1;
		intervalHeapPut(&heap,array[i], &array[i]);
	}

	traverseHeap(&heap);
	
	int *min,*max;
	
	getMin(&heap, (void **)&min);
	getMax(&heap, (void **)&max);
	
	printf("Min => %d , Max => %d\n",*min,*max);
	
	/* Sorts in ascending order*/
	for (i = 0; i < SIZE; i++) {
		getMin(&heap,(void **)&min);
		sorted[i] = *min;
		removeMin(&heap);
	}
	
	int prev = sorted[0];
	
	/* Is the array sorted ? */
	
	for (i = 1; i < SIZE; i++) {
		if (prev > sorted[i]) {
			printf("Random Array :(\n");
			break;
		}
		prev = sorted[i];
	}
	if (i == SIZE)
		printf("Sorted Array :D\n");
	
	destroyIntervalHeap(&heap);
	
	return 0;	
}
