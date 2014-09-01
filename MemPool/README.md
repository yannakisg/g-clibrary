MemPool 0.1

Description:
MemPool is a simple memory pool implementation. 

You can allocate as many memory pools as your system is capable of support by calling allocMem(totalBlocks,blockSize).
The memory pool allocates a big amount of memory (totalBlocks * (blockSize + sizeof(MemNode)bytes ) and separates this block into smaller chunks(MemNode).
You can request memory from the pools by calling getMemNode() and release by calling releaseMemNode(node). 
Finally by calling freeMemory you free the memory space pointed to by lmPool.

		    	  
lmPool->root  -->|MemPool| ---> |MemPool|
                      |            |
                      |            |	
lmPool->root->next->head------>	 |MemNode| --> |MemNode|
                      |	 
                      |
lmPool->root->head ->|MemNode| --> |MemNode| --> |MemNode|
		      

Example:
#include "src/mempool.h"

int main() {
// ...
	allocMem(256 ,sizeof(struct Foo));
	Foo *f = (Foo *) getMemNode();
// .....
	releaseMemNode(f);
	releaseMemory();
// ...
	return 0;
}

* MemPool was checked for memory leaks with Valgrind-3.5.0

