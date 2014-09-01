Min-Max Heap 0.1

Description:
A Min-Max Heap implementation. 

In Min-Max Heap all nodes are stored in a complete binary tree with alternating levels being min levels and max levels. The root level is a min level.
Every min (max) node has the property that its priority is the smallest (largest) in its subtree of which it is the root.
As a result,the minimum element is located at the root while the maximum in one of its children(getMin,getMax -> O(1)). The operations mmHeapPut removeMax and removeMin takes O(logn) time while the initialization and the deletion of n elements takes O(n).



                         [2]            <--- min level
                        /   \
                       /     \
                   [80]      [60]       <--- max level
                   /  \      /  \ 
                  /    \    /    \
                [10] [15] [11]   [30]   <--- min level
                /  \
               /    \
             [40]   [50]                <--- max level
Example:
#include "../src/minmaxheap.h"

int main() {
// ...
	MinMaxHeap heap;
	initMinMaxHeap(&heap, NULL, DEFAULTSIZE, THRESHOLD);
// ...
	mmHeapPut(&heap,key2, data2);
	mmHeapPut(&heap,key2, data2);
// ...
	getMin(&heap, (void **)&min);
	getMax(&heap, (void **)&max);
// ...
	removeMin(&heap);
	removeMax(&heap);
// ...
	return 0;
}

* Min-Max Heap was checked for memory leaks with Valgrind-3.5.0

