#include "minmaxheap.h"

/* Initializes a Min-Max Heap 
 *	Attention: Threshold must be greater than 0.50 for efficient reasons
 */
int initMinMaxHeap(MinMaxHeap *heap, void (*destroy) (void *data), int defaultSize, float threshold) {
	if (heap == NULL || defaultSize == 0) {
		errno = EINVAL;
		return -1;
	}
	
	heap->table = (MMHeapNode **) malloc(sizeof(MMHeapNode**) *defaultSize);
	if (heap->table == NULL) {
		errno = ENOMEM;
		return -2;
	}
	
	memset(heap->table, 0, sizeof(MMHeapNode**) *defaultSize);
	
	heap->destroy = destroy;
	heap->size = 0;
	heap->positions = defaultSize;
	heap->threshold = threshold;
	heap->maximumSize = defaultSize * threshold; 
	heap->minimumSize = (1 - threshold) * defaultSize;
	
	return 0;
}

/* Inserts a node into the Min-Max Heap */
int mmHeapPut(MinMaxHeap *heap, int key, void *data) {
	if (heap == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	MMHeapNode **temp, *newNode,*sw;
	int newSize;
	
	newNode = (MMHeapNode *) malloc(sizeof(MMHeapNode));
	if (newNode == NULL) {
		errno = ENOMEM;
		return -2;
	}
	
	newNode->key = key;
	newNode->data = data;
	
	if (heap->size == 0) {
		heap->table[heap->size++] = newNode;
		newNode->type = MIN;
		
		return 0;
	}
	
	/* Does it need reallocation ? */
	if (heap->size + 1 >= heap->maximumSize) {
		newSize = heap->positions << 1;
		
		temp = (MMHeapNode **) realloc(heap->table, sizeof(MMHeapNode **) * newSize);
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	int parent = PARENT(heap->size);
	int curPos = heap->size;
	
	heap->table[heap->size++] = newNode;
	
	newNode->type = heap->table[parent]->type == MIN ? MAX : MIN;
	
	/* All min- and max-nodes properties are satisfied when the node has the same priority as its parent*/
	if (heap->table[parent]->key == key)
		return 0;
	/* If parentKey > newKey */
	else if (heap->table[parent]->key > key) {
		/* min node violation */
		if (newNode->type == MAX) { 
			heap->table[curPos] = heap->table[parent];
			heap->table[curPos]->type = MAX;
			heap->table[parent] = newNode;
			newNode->type = MIN;
			
			curPos = parent;
			parent = PARENT(PARENT(parent));
		} else {
			parent = PARENT(parent);
		}
		
		/* appropriate "heapify" */
		while (parent >= 0 && heap->table[parent]->key > key) {
			sw = heap->table[parent];
			heap->table[parent] = heap->table[curPos];
			heap->table[curPos] = sw;
			
			curPos = parent;
			parent = PARENT(PARENT(parent));
		}
	}
	/* If parentKey < newKey */ 
	else {
		 if (newNode->type == MIN) {
			 /* max node violation */
				sw = heap->table[parent];
				heap->table[parent] = heap->table[curPos];
				heap->table[curPos] = sw;
				
				heap->table[curPos]->type = MIN;
				heap->table[parent]->type = MAX;
				curPos = parent;
				parent = PARENT(PARENT(parent));
		 }
		 else
			parent = PARENT(parent);
		
		/* appropriate "heapify" */
		while (parent >= 1 && heap->table[parent]->key < key) {
			sw = heap->table[parent];
			heap->table[parent] = heap->table[curPos];
			heap->table[curPos] = sw;
				
			curPos = parent;
			parent = PARENT(PARENT(parent));
		} 
	}
	
	return 0;
}

/* Returns the element with the mininum priority. It sets data to point to the data from the minimum element */
int getMin(MinMaxHeap *heap, void **data) {
	if (heap == NULL || data == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	*data = heap->table[0]->data;
	
	return 0;
}

/* Returns the element into data with the maximum priority. It sets data to point to the data from the maximum element */
int getMax(MinMaxHeap *heap, void **data) {
	if (heap == NULL || data == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	if (heap->size == 1)
		*data = heap->table[0]->data;
	else if (heap->size == 2)
		*data = heap->table[1]->data;
	else
		*data = heap->table[1]->key > heap->table[2]->key ? heap->table[1]->data : heap->table[2]->data;
	
	return 0;
}

/* Removes the element with the minimum priority*/
int removeMin(MinMaxHeap *heap) {
	if (heap == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	/* If the heap has only one node , remove it*/
	if (heap->size == 1) {
		if (heap->destroy != NULL)
			heap->destroy(heap->table[0]->data);
		
		free(heap->table[0]);
		heap->table[0] = NULL;
		heap->size = 0;
			
		return 0;
	}
	
	MMHeapNode **temp, *remNode,*sw;
	int newSize;
	
	/* Does it need reallocation ? */
	if (heap->size - 1 < heap->minimumSize) {
		newSize = heap->positions >> 1;
			
		temp = (MMHeapNode **) realloc(heap->table, newSize * sizeof(MMHeapNode **));
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	remNode = heap->table[0];
	heap->table[0] = heap->table[--heap->size];
	heap->table[0]->type = MIN;
	heap->table[heap->size] = NULL;
	
	int curPos = 0;
	int left = LEFT(curPos);
	int right = RIGHT(curPos);
	int leftLG = LEFT(left) , leftRG = RIGHT(left);
	int rightLG = LEFT(right), rightRG = RIGHT(right);
	int min;
	
	/* If the new temporary root has at least one child */
	for (; ;) {
		
		/* Finds node with the minimum priority. That node is one of the children or grandchildren of the root. */
		if (left < heap->size) 	
			min = left;
		else 
			break;

		if (right < heap->size && heap->table[right]->key <  heap->table[min]->key)
			min = right;
		if (leftLG < heap->size) {
			
			if(heap->table[leftLG]->key < heap->table[min]->key)
				min = leftLG;
				
			if (leftRG < heap->size) { 
				if (heap->table[leftRG]->key < heap->table[min]->key) 
					min = leftRG;
					
				if (rightLG < heap->size) {
					if (heap->table[rightLG]->key < heap->table[min]->key) 
						min = rightLG;
						
					if (rightRG < heap->size && heap->table[rightRG]->key < heap->table[min]->key) 
						min = rightRG;
				}
			}
		}
		
		/* Apparently there is no smaller element in the heap*/			
		if (heap->table[min]->key >= heap->table[curPos]->key)
			break;
		
		/* If the smaller element is a left or right child of the root then interchange them and terminate */	
		if (heap->table[curPos]->key > heap->table[min]->key && (min == left || min == right)) { 
			sw = heap->table[curPos];
			heap->table[curPos] = heap->table[min];
			heap->table[curPos]->type = MIN;
			sw->type = MAX;
			heap->table[min] = sw;
			
			break;
		}
		/* If the smaller element is a grandchild of the root then interchange them*/
		else if (heap->table[curPos]->key > heap->table[min]->key) { 
			sw = heap->table[curPos];
			heap->table[curPos] = heap->table[min];
			
			/* If the parent of the grandchild has smaller priority , interchange them*/
			int parent = PARENT(min);
			if (sw->key > heap->table[parent]->key) {
				heap->table[min] = heap->table[parent];
				heap->table[min]->type = MIN;
				heap->table[parent] = sw;
				sw->type = MAX;
				
			} else {
				heap->table[min] = sw;
			}
			
			/* Continues with the min as the root */
			curPos = min;
			left = LEFT(curPos);
			right = RIGHT(curPos);
			leftLG = LEFT(left);
			leftRG = RIGHT(left);
			rightLG = LEFT(right);
			rightRG = RIGHT(right);
		}
	}
	
	if (heap->destroy != NULL)
		heap->destroy(remNode->data);
	
	free(remNode);
	
	return 0;
}

/* Removes the element with the maximum priority*/
int removeMax(MinMaxHeap *heap) {
	if (heap == NULL || heap->size == 0) {
		errno = EINVAL;
		return -1;
	}
	
	
	MMHeapNode **temp, *remNode,*sw;
	int newSize;
	
	/* If the heap has only one node , remove it*/
	if (heap->size == 1) {
		if (heap->destroy != NULL)
			heap->destroy(heap->table[0]->data);
		
		free(heap->table[0]);
		heap->size = 0;
			
		return 0;
	}
	/* If the heap has only two nodes , remove the left child of the root*/
	if (heap->size == 2) {
		remNode = heap->table[1];
		heap->table[1] = NULL;
		if (heap->destroy != NULL)
			heap->destroy(remNode->data);
		
		free(remNode);
		heap->size--;
		return 0;
	}

	/* Does it need reallocation ? */
	if (heap->size - 1 < heap->minimumSize) {
		newSize = heap->positions >> 1;
			
		temp = (MMHeapNode **) realloc(heap->table, newSize * sizeof(MMHeapNode **));
		if (temp == NULL) {
			errno = ENOMEM;
			return -2;
		}
		
		heap->table = temp;
		heap->maximumSize = newSize * heap->threshold;
		heap->minimumSize = (1 - heap->threshold) * newSize;
		heap->positions = newSize;
	}
	
	/* Finds the maximum child of the heap */
	int curPos;
	if (heap->table[1]->key > heap->table[2]->key) {
		remNode = heap->table[1];
		curPos = 1;
	} else {
		remNode = heap->table[2];
		curPos = 2;
	}
	heap->table[curPos] = heap->table[--heap->size];
	heap->table[curPos]->type = MAX;
	heap->table[heap->size] = NULL;
	
	int left = LEFT(curPos);
	int right = RIGHT(curPos);
	int leftLG = LEFT(left) , leftRG = RIGHT(left);
	int rightLG = LEFT(right), rightRG = RIGHT(right);
	int max;
	
	/* If the new temporary root has at least one child */
	for (; ;) {
		
		/* Finds node with the maximum priority. That node is one of the children or grandchildren of the root. */
		if (left < heap->size) 	
			max = left;
		else 
			break;
	
		if (right < heap->size && heap->table[right]->key >  heap->table[max]->key)
			max = right;
		if (leftLG < heap->size) {
			
			if(heap->table[leftLG]->key > heap->table[max]->key)
				max = leftLG;
				
			if (leftRG < heap->size) { 
				if (heap->table[leftRG]->key > heap->table[max]->key) 
					max = leftRG;
					
				if (rightLG < heap->size) {
					if (heap->table[rightLG]->key > heap->table[max]->key) 
						max = rightLG;
						
					if (rightRG < heap->size && heap->table[rightRG]->key > heap->table[max]->key) 
						max = rightRG;
				}
			}
		}
		/* Apparently there is no larger element in the heap*/						
		if (heap->table[max]->key <= heap->table[curPos]->key)
			break;
			
		/* If the larger element is a left or right child of the root then interchange them and terminate */
		if (heap->table[curPos]->key < heap->table[max]->key && (max == left || max == right)) { 
			sw = heap->table[curPos];
			heap->table[curPos] = heap->table[max];
			heap->table[curPos]->type = MAX;
			sw->type = MIN;
			heap->table[max] = sw;
			
			break;
		}
		
		/* If the larger element is a grandchild of the root then interchange them*/
		if (heap->table[curPos]->key < heap->table[max]->key) { 
			sw = heap->table[curPos];
			heap->table[curPos] = heap->table[max];
			
			/* If the parent of the grandchild has larger priority , interchange them*/
			int parent = PARENT(max);
			if (sw->key < heap->table[parent]->key) {
				heap->table[parent]->type = MAX;
				heap->table[max] = heap->table[parent];
				
				heap->table[parent] = sw;
				sw->type = sw->type == MIN;
			} else {
				heap->table[max] = sw;
			}	
			
			/* Continues with the max as the root */
			curPos = max;
			left = LEFT(curPos);
			right = RIGHT(curPos);
			leftLG = LEFT(left);
			leftRG = RIGHT(left);
			rightLG = LEFT(right);
			rightRG = RIGHT(right);
		}
	}
	
	if (heap->destroy != NULL)
		heap->destroy(remNode->data);
	
	free(remNode);
	
	return 0;
}

/* Destroys the Min-Max Heap */ 
int destroyMinMaxHeap(MinMaxHeap *heap) {
	if (heap == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	int i;
	
	if (heap->destroy != NULL) {
		for (i = 0; i < heap->size; i++) {
			heap->destroy(heap->table[i]->data);
			free(heap->table[i]);
		}
	} else {
		for (i = 0; i < heap->size; i++) {
			free(heap->table[i]);
		}
	}
	
	free(heap->table);
	
	memset(heap, 0, sizeof(MinMaxHeap));
	
	return 0;	
}
