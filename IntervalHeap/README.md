Interval Heap 0.1

Description:
An Interval Heap implementation. 

Interval Heap is a complete binary tree in which each node, except the last one, contains two elements.
A node N with two elements a,b where a <= b  represents the interval [a,b]. The node N  has a children C 
(with two elements c,d where c <= d) if and only if a <= c <= d <= b. As a result,the root has the minimum
and the maximum element of the heap (getMin,getMax -> O(1)). The operations intervalHeapPut removeMax and removeMin takes 
O(logn) time while the initialization and the deletion of n elements takes O(n).



                         [2  , 80]
                        /         \
                       /           \
                    [5,40]        [4,60]
                   /      \         \   
                  /        \         \  
                [10,12]    [15,49]    [35]

Example:
#include "src/intervalheap.h"

int main() {
// ...
	IntervalHeap heap;
	initIntervalHeap(&heap, destroy, DEFAULTSIZE,THRESHOLD);
// ...
	intervalHeapPut(&heap,key1, data1);
	intervalHeapPut(&heap,key2, data2);
// ...
	getMin(&heap, (void **)&min);
	getMax(&heap, (void **)&max);
// ...
	removeMin(&heap);
	removeMax(&heap);
// ...
	return 0;
}

* IntervalHeap was checked for memory leaks with Valgrind-3.5.0

