#include "intervalheap.h"

/* Initializes an Interval Heap 
 *	Attention: Threshold must be greater than 0.50 for efficient reasons
 */
int initIntervalHeap(IntervalHeap *heap, void (*destroy) (void *data), int defaultSize, float threshold) {
	if (heap == NULL || defaultSize == 0) {
		errno = EINVAL;
		return -1;
	}
	
	heap->table = (IntervNode **) malloc(sizeof(IntervNode**) *defaultSize);
	if (heap->table == NULL) {
		errno = ENOMEM;
		return -2;
	}
	
	memset(heap->table, 0, sizeof(IntervNode**) *defaultSize);
	
	heap->destroy = destroy;
	heap->size = 0;
	heap->positions = defaultSize;
	heap->threshold = threshold;
	heap->maximumSize = defaultSize * threshold; 
	heap->minimumSize = (1 - threshold) * defaultSize;
	
	return 0;
}

/* Inserts an element into the Interval Heap */
int intervalHeapPut(IntervalHeap *heap, int key, void *data) {
	if (heap == NULL || data == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	IntervNode **temp,*newNode;
	int newSize;
	
	/* Does it need reallocation ? */
	if (heap->size + 1 >= heap->maximumSize) {
		newSize = heap->positions << 1;
		
		temp = (IntervNode **) realloc(heap->table, sizeof(IntervNode*)*newSize);
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	/* Creates a new node , because this heap has an even number of nodes  */
	if (heap->size == 0 || isFull(heap->table[heap->size - 1])) {
		newNode = (IntervNode *) malloc(sizeof(IntervNode));
		if (newNode == NULL) {
			errno = ENOMEM;
			return -2;
		}
		newNode->keyL = key;
		newNode->dataL = data;
		newNode->dataR = NULL;
		newNode->keyR = 0;
		heap->table[heap->size++] = newNode;
	} else { /* The heap has an odd number of nodes */
		newNode = heap->table[heap->size - 1];
		
		if (newNode->keyL < key) {
			newNode->keyR = key;
			newNode->dataR = data;
		} else {
			newNode->keyR = newNode->keyL;
			newNode->keyL = key;
			newNode->dataR = newNode->dataL;
			newNode->dataL = data;
		}
	}
	
	if (heap->size == 1) {
		return 0;
	}
	
	int pos = heap->size - 1;
	int parent = PARENT(pos);
	int tempKey;
	void *tempData;
	
	/* 
	 * The new element's key is less than the left endpoint of the parent interval. 
	 * So it is inserted into the min heap embedded in the interval heap.
	 */
	if (key < heap->table[parent]->keyL) { 
		
		for (; ;) {
			if (heap->table[parent]->keyL > key) {
				tempData = heap->table[parent]->dataL;
				tempKey = heap->table[parent]->keyL;
				
				heap->table[parent]->keyL = heap->table[pos]->keyL;
				heap->table[parent]->dataL = heap->table[pos]->dataL;
				
				heap->table[pos]->keyL = tempKey;
				heap->table[pos]->dataL = tempData;
				
				pos = parent;
				parent = PARENT(pos);
				if (pos == parent || parent < 0)
					break;
			} else
				break;
		}
	}/* 
	 * The new element's key is greater than the right endpoint of the parent interval. 
	 * So it is inserted into the max heap embedded in the interval heap.
	 */ 
	else if(key > heap->table[parent]->keyR) { 
	
		for (; ;) {
			if (heap->table[parent]->keyR < key) {
				tempData = heap->table[parent]->dataR;
				tempKey = heap->table[parent]->keyR;
				
				if (heap->table[pos]->dataR != NULL) {
					heap->table[parent]->keyR = heap->table[pos]->keyR;
					heap->table[parent]->dataR = heap->table[pos]->dataR;
					heap->table[pos]->keyR = tempKey;
					heap->table[pos]->dataR = tempData;
				} else {
					heap->table[parent]->keyR = heap->table[pos]->keyL;
					heap->table[parent]->dataR = heap->table[pos]->dataL;
					heap->table[pos]->keyL = tempKey;
					heap->table[pos]->dataL = tempData;
				}
				
				pos = parent;
				parent = PARENT(pos);
				if (pos == parent || parent < 0)
					break;
			} else
				break;
		}
	}
	
	return 0;
}

/* Returns the element with the mininum priority. It sets data to point to the data from the minimum element */
int getMin(IntervalHeap *heap, void **data) {
	if (heap == NULL || data == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	*data = heap->table[0]->dataL;
	
	return 0;
}

/* Returns the element into data with the maximum priority. It sets data to point to the data from the maximum element */
int getMax(IntervalHeap *heap, void **data) {
	if (heap == NULL || data == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	*data = heap->table[0]->dataR == NULL ? heap->table[0]->dataL : heap->table[0]->dataR;
	
	return 0;
}

/* Removes the element with the minimum priority*/
int removeMin(IntervalHeap *heap) {
	if (heap == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	/* When the interval heap has only one element, this element is the element to be removed.*/
	if (heap->size == 1 && heap->table[0]->dataR == NULL) {
		if (heap->destroy != NULL)
			free(heap->table[0]->dataL);
		free(heap->table[0]);
		
		heap->table[0] = NULL;
		
		return 0;
	}
	
	int newSize;
	IntervNode **temp;
	int pos = 0;
	int left = LEFT(pos);
	int right = RIGHT(pos);
	int min;
	int tempKey;
	void *tempData;
	
	/* Does it need reallocation ? */
	if (heap->size - 1 < heap->minimumSize) {
		newSize = heap->positions >> 1;
			
		temp = (IntervNode **) realloc(heap->table, newSize * sizeof(IntervNode *));
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	heap->table[0]->keyL = heap->table[heap->size - 1]->keyL;
	heap->table[0]->dataL = heap->table[heap->size - 1]->dataL;
	
	heap->table[heap->size - 1]->keyL = heap->table[heap->size - 1]->keyR;
	heap->table[heap->size - 1]->dataL = heap->table[heap->size - 1]->dataR;
	heap->table[heap->size - 1]->keyR = 0;
	heap->table[heap->size - 1]->dataR = NULL;
	
	/* Deletes the empty node (if there is one) */
	if (heap->table[heap->size-1]->dataL == NULL) {
		heap->size--;
		free(heap->table[heap->size]);
	}
	
	/* Appropriate "heapify" in order to be a valid interval heap*/
	for (; ;) {
		if (left >= heap->size && right >= heap->size)
			break;
			
		if (right >= heap->size)
			min = left;
		else if (left >= heap->size)	
			min = right;
		else
			min = heap->table[left]->keyL < heap->table[right]->keyL ? left : right;
		
		if (heap->table[pos]->keyL > heap->table[min]->keyL) {
			tempKey = heap->table[pos]->keyL;
			tempData = heap->table[pos]->dataL;
			
			heap->table[pos]->keyL = heap->table[min]->keyL;
			heap->table[pos]->dataL = heap->table[min]->dataL;
			
			heap->table[min]->keyL = tempKey;
			heap->table[min]->dataL = tempData;
			
			if (heap->table[min]->keyL > heap->table[min]->keyR) {
				tempKey = heap->table[min]->keyL;
				tempData = heap->table[min]->dataL;
			
				heap->table[min]->keyL = heap->table[min]->keyR;
				heap->table[min]->dataL = heap->table[min]->dataR;
			
				heap->table[min]->keyR = tempKey;
				heap->table[min]->dataR = tempData;
			}
			
			pos = min;
			left = LEFT(pos);
			right = RIGHT(pos);
		} else
			break;
	}
	
	return 0;
}

/* Removes the element with the maximum priority*/
int removeMax(IntervalHeap *heap) {
	if (heap == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	/* When the interval heap has only one element, this element is the element to be removed.*/
	if (heap->size == 1 && heap->table[0]->dataR == NULL) {
		if (heap->destroy != NULL)
			free(heap->table[0]->dataL);
		free(heap->table[0]);
		
		heap->table[0] = NULL;
		
		return 0;
	}
	
	int newSize;
	IntervNode **temp;
	
	int pos = 0;
	int left = LEFT(pos);
	int right = RIGHT(pos);
	int max;
	int tempKey;
	void *tempData;
	
	/* Does it need reallocation ? */
	if (heap->size - 1 < heap->minimumSize) {
		newSize = heap->positions >> 1;
			
		temp = (IntervNode **) realloc(heap->table, newSize * sizeof(IntervNode *));
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	if (isFull(heap->table[heap->size - 1])) {
		heap->table[0]->keyR = heap->table[heap->size - 1]->keyR;
		heap->table[0]->dataR = heap->table[heap->size - 1]->dataR;
		heap->table[heap->size - 1]->keyR = 0;
		heap->table[heap->size - 1]->dataR = NULL;
	} else {
		heap->table[0]->keyR = heap->table[heap->size - 1]->keyL;
		heap->table[0]->dataR = heap->table[heap->size - 1]->dataL;
		heap->table[heap->size - 1]->keyL = 0;
		heap->table[heap->size - 1]->dataL = NULL;
	}
	
	/* Deletes the empty node (if there is one) */
	if (heap->table[heap->size-1]->dataL == NULL) {
		heap->size--;
		free(heap->table[heap->size]);
	}
	
	/* Appropriate "heapify" in order to be a valid interval heap*/
	for (; ;) {
		if (left >= heap->size && right >= heap->size)
			break;
			
		if (right >= heap->size)
			max = left;
		else if (left >= heap->size)	
			max = right;
		else
			max = heap->table[left]->keyR > heap->table[right]->keyR ? left : right;
		
		if (heap->table[pos]->keyR < heap->table[max]->keyR) {
			tempKey = heap->table[pos]->keyR;
			tempData = heap->table[pos]->dataR;
			
			heap->table[pos]->keyR = heap->table[max]->keyR;
			heap->table[pos]->dataR = heap->table[max]->dataR;
			
			heap->table[max]->keyR = tempKey;
			heap->table[max]->dataR = tempData;
			
			if (heap->table[max]->keyL > heap->table[max]->keyR) {
				tempKey = heap->table[max]->keyL;
				tempData = heap->table[max]->dataL;
			
				heap->table[max]->keyL = heap->table[max]->keyR;
				heap->table[max]->dataL = heap->table[max]->dataR;
			
				heap->table[max]->keyR = tempKey;
				heap->table[max]->dataR = tempData;
			}
			
			pos = max;
			left = LEFT(pos);
			right = RIGHT(pos);
		} else
			break;
	}
	
	return 0;
}

/* Destroys the Interval Heap */ 
int destroyIntervalHeap(IntervalHeap *heap) {
	if (heap == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	int i;
	
	if (heap->destroy != NULL) {
		for (i = 0; i < heap->size; i++) {
			if (heap->table[i]->dataR != NULL)
				heap->destroy(heap->table[i]->dataR);
			if (heap->table[i]->dataL != NULL)
				heap->destroy(heap->table[i]->dataL);
			free(heap->table[i]);
		}
	} else {
		for (i = 0; i < heap->size; i++) {
			free(heap->table[i]);
		}
	}
	
	free(heap->table);
	
	memset(heap, 0, sizeof(IntervalHeap));
	
	return 0;
}
