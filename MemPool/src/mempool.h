#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include <stdlib.h>
#include <errno.h>

#define VARERR -1
#define MEMERRPOOL -2
#define MEMERRLPOOL -3
#define MEMERRBLOCK -4

typedef unsigned long ULONG;

/*
 * Memory Unit - Linked list consists of MemNode nodes
 */ 
typedef struct MemNode_ {
	struct MemNode_ *next;
}MemNode;

/*
 * MemPool -> The type of the node of the LMemPool 
 * 	memBlock -> The address of Memory Pool
 * 	head -> Head pointer of MemNode linked list
 * 	allocBlocks -> The head pointer of "Allocated Linked List"
 *  freeBlocks -> The head pointer of "Free Linked List"
 * 	totalSize -> The total size of each memory pool 
 * 	blockSize ->  The size of block
 */
typedef struct MemPool_ {
	void *memBlock;
	MemNode *allocBlocks;
	MemNode *freeBlocks;
	struct MemPool_ *head;
	ULONG totalSize; 
	ULONG blockSize; 
}MemPool;

/*
 *LMemPool -> A linked list constists of MemPool nodes
 *	root -> Head pointer of MemPool linked list
 */
typedef struct LMemPool_ {
	MemPool *root;
}LMemPool;


static LMemPool *lmPool = NULL;

/* Allocates Memory Blocks from os and creates a memory pool to manage all memory units */
int allocMem(ULONG tBlocks, ULONG bSize);

/* Allocates a Memory Unit */
void *getMemNode();

/* Releases a Memory Unit */
void releaseMemNode(void *node);

/* Frees the memory space pointed to by lmPool */
void freeMemory();

#endif
