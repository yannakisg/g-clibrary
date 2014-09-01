#ifndef _INTERVALHEAP_H_
#define _INTERVALHEAP_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
 * Interval Heap Node
 *	keyL -> The left endpoint of the node interval
 * 	keyR -> The right endpoint of the node interval
 * 	dataL -> Points to the data from the node which belongs to min heap
 * 	dataR -> Points to the data from the node which belongs to max heap
 */ 
typedef struct IntervNode_ {
	int keyL;
	int keyR;
	void *dataL;
	void *dataR;
}IntervNode;

/*
 * Interval Heap
 * 	size -> Current size
 * 	table -> Min-Max Heap
 */
typedef struct IntervalHeap_ {
	float threshold;
	void (*destroy) (void *data);
	int size;
	int maximumSize;
	int minimumSize;
	int positions;
	IntervNode **table;
}IntervalHeap;

/* Initializes an Interval Heap 
 *	Attention: Threshold must be greater than 0.50 for efficient reasons
 */
int initIntervalHeap(IntervalHeap *heap, void (*destroy) (void *data), int defaultSize, float threshold);

/* Inserts an element into the Interval Heap */
int intervalHeapPut(IntervalHeap *heap, int key, void *data);

/* Returns the element with the mininum priority. It sets data to point to the data from the minimum element */
int getMin(IntervalHeap *heap, void **data);

/* Returns the element into data with the maximum priority. It sets data to point to the data from the maximum element */
int getMax(IntervalHeap *heap, void **data);

/* Removes the element with the minimum priority*/
int removeMin(IntervalHeap *heap);

/* Removes the element with the maximum priority*/
int removeMax(IntervalHeap *heap);

/* Destroys the Interval Heap */ 
int destroyIntervalHeap(IntervalHeap *heap);

/* Helpful definitions */

#define PARENT(i) ((int)(((i)-1) >> 1))
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) << 1) + 2)
#define isFull(node) ( node->dataL != NULL && node->dataR != NULL ) 

#endif
