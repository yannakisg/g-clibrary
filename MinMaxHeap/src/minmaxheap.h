#ifndef _MINMAXHEAP_H_
#define _MINMAXHEAP_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
 * Min-Max Heap Node
 *	type -> Does the node belong to a min level or a max one ?
 * 	key -> Priority 
 * 	dataL -> Points to the data
 */ 
typedef struct MMHeapNode_ {
	enum Type{
		MIN,
		MAX
	} type;	
	int key;
	void *data;
}MMHeapNode;

/*
 * Min-Max Heap
 * 	size -> Current size
 * 	table -> Min-Max Heap
 */
typedef struct MinMaxHeap_ {
	float threshold;
	void (*destroy) (void *data);
	int size;
	int maximumSize;
	int minimumSize;
	int positions;
	MMHeapNode **table;
}MinMaxHeap;

/* Initializes a Min-Max Heap 
 *	Attention: Threshold must be greater than 0.50 for efficient reasons
 */
int initMinMaxHeap(MinMaxHeap *heap, void (*destroy) (void *data), int defaultSize, float threshold);

/* Inserts a node into the Min-Max Heap */
int mmHeapPut(MinMaxHeap *heap, int key, void *data);

/* Returns the element with the mininum priority. It sets data to point to the data from the minimum element */
int getMin(MinMaxHeap *heap, void **data);

/* Returns the element into data with the maximum priority. It sets data to point to the data from the maximum element */
int getMax(MinMaxHeap *heap, void **data);

/* Removes the element with the minimum priority*/
int removeMin(MinMaxHeap *heap);

/* Removes the element with the maximum priority*/
int removeMax(MinMaxHeap *heap);

/* Destroys the Min-Max Heap */ 
int destroyMinMaxHeap(MinMaxHeap *heap);

/* Helpful definitions */
#define PARENT(i) ((int)(((i)-1) >> 1))
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) << 1) + 2)

#endif
