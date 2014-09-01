#include "mempool.h"

/* Allocates Memory Blocks from os and creates a memory pool to manage all memory units
 * 
 *	tBlocks -> Total Blocks
 * 	bSize -> The size of block 
 */
int allocMem(ULONG tBlocks, ULONG bSize) {
	if (tBlocks == 0 || bSize == 0) {
		errno = EINVAL;
		return VARERR;
	}
	MemPool *iter;
	ULONG i;
	MemNode *curMemNode;
	
	
	if (lmPool == NULL) {
		lmPool = (LMemPool *) malloc(sizeof(LMemPool));
		if (lmPool == NULL) {
			errno = ENOMEM;
			return MEMERRLPOOL;
		}
		lmPool->root = NULL;
	}
	
	/* Creates a memory pool and inserts it at the head of "lmPool linked list" */
	
	iter =  (MemPool *) malloc(sizeof(MemPool));
	if (iter == NULL) {
			errno = ENOMEM;
			return MEMERRPOOL;
	}
	
	iter->head = lmPool->root;
	lmPool->root = iter;
	
	/* Initializes the memory pool */
	
	iter->totalSize = tBlocks * (bSize + sizeof(MemNode));
	iter->blockSize = bSize;
	iter->memBlock = (void *) malloc(iter->totalSize);
	if (iter->memBlock == NULL) {
		free(iter);
		errno = ENOMEM;
		return MEMERRBLOCK;
	}
	
	iter->freeBlocks = NULL;
	iter->allocBlocks = NULL;
	
	/* Links all memory units and inserts them into "Free Linked List" */	
	for (i = 0; i < tBlocks; i++) {
		curMemNode = (MemNode *) ((char *)iter->memBlock + i * (bSize + sizeof(MemNode)));
		
		curMemNode->next = iter->freeBlocks;
		
		iter->freeBlocks = curMemNode;
	}
	
	return 0;
}

void *getMemNode() {
	if (lmPool == NULL || lmPool->root == NULL) {
		errno = EINVAL;
		return NULL;
	}
	/* Searches for a memory pool having a free blocks available for allocation */
	MemPool *iter = lmPool->root;
	
	while (iter != NULL && iter->freeBlocks == NULL) {
		iter = iter->head;
	}
	
	/* If there is not such a memory pool , create one */
	if (iter == NULL) {
		if ( allocMem( (ULONG)(lmPool->root->totalSize / lmPool->root->blockSize), lmPool->root->blockSize) != 0)
			return NULL;
	}
	
	iter = lmPool->root;
	
	
	/* Gets a unit from "Free Linked List" */
	MemNode *curMemNode = iter->freeBlocks;
	
	iter->freeBlocks = iter->freeBlocks->next;
	
	/* Inserts the unit to "Allocated Linked List" */
	curMemNode->next = iter->allocBlocks;
	
	iter->allocBlocks = curMemNode;
	
	/* returns a pointer to the unit */ 
	return (void *) ((char *)curMemNode + sizeof(MemNode));
}

/* Releases a Memory Unit
 * 	node -> A Memory Unit of type MemNode
 */
void releaseMemNode(void *node) {
	if (node == NULL || lmPool->root == NULL) {
		errno = EINVAL;
		return ;
	}
	
	/* Finds the memory pool in which the memory unit was allocated */
	MemPool *iter = lmPool->root;
	
	while (iter != NULL && (iter->memBlock >= node || node >= (void *) ((char *)iter->memBlock+iter->totalSize)))
		iter = iter->head;
	
	if (iter == NULL) {
		errno = EINVAL;
		return ;
	}
	
	/* Inserts it into "Free Linked List" */
	MemNode *curMemNode = (MemNode *) ((char *) node - sizeof(MemNode));	
	
	iter->allocBlocks = curMemNode->next;
		
	curMemNode->next = iter->freeBlocks;
		
	iter->freeBlocks = curMemNode;
}

/* Frees the memory space pointed to by lmPool */
void freeMemory() {
	MemPool *iter = lmPool->root;
	MemPool *temp;
	
	while (iter != NULL) {
		temp = iter;
		free(temp->memBlock);
		iter = iter->head;
		free(temp);
	}
	free(lmPool);
	lmPool = NULL;
}
