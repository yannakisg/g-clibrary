#include <stdio.h>
#include <time.h>

#include "../src/minmaxheap.h"

void traverseHeap(MinMaxHeap *heap) {
	if (heap == NULL) {
		errno = EINVAL;
		return ;
	}
	
	int i;
	for (i = 0; i < heap->size; i++) {
		printf("-----------------------\n");
		if (heap->table[i]->type == MIN)
			printf("Min");
		else
			printf("Max");
		printf(": Element(%d) => %d , key => %d\n", i, *((int *)heap->table[i]->data),heap->table[i]->key);
		printf("-----------------------\n");
	}
}

#define SIZE 2000
#define UPPERSIZE 10000
#define DEFAULTSIZE 200
#define THRESHOLD 0.75

int main() {
	MinMaxHeap heap;
	
	initMinMaxHeap(&heap, NULL, DEFAULTSIZE, THRESHOLD);
	
	srand(time(NULL));
	
	int sorted[SIZE],array[SIZE];
	int i;
	
	for (i = 0; i < SIZE; i++) {
		array[i] = (rand() % UPPERSIZE) + 1;
		mmHeapPut(&heap,array[i], &array[i]);
	}
	
	traverseHeap(&heap);
	
	int *min, *max;
	
	getMin(&heap, (void **)&min);
	getMax(&heap, (void **)&max);
	
	printf("Min => %d , Max => %d\n",*min,*max);
	
	/* Sorts in ascending order*/
	for (i = 0; i < SIZE; i++) {
		getMin(&heap, (void **)&min);
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
	
	
	destroyMinMaxHeap(&heap);
	
	return 0;
}
