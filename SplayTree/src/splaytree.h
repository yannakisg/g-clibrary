#ifndef _SPLAYTREE_H_
#define _SPLAYTREE_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* Uncomment the below lines to use MemPool instead of system's default functions malloc and free */ 

#include "../../MemPool/src/mempool.h"
#define MEMPOOL 

#ifdef MEMPOOL
	#define malloc(size) getMemNode()
	#define free(ptr) releaseMemNode(ptr)
#endif

/* Splay Tree Node */
typedef struct SPNode_ {
	struct SPNode_ *child[2];
	struct SPNode_ *parent;
	int key;
	void *data;
}SPNode;

/* Splay Tree */
typedef struct SPTree_ {
	SPNode *root;
	void (*destroy) (void *data);
}SPTree;

/* Inititalizes a Splay Tree */
int initSPTree(SPTree *tree, void (*destroy) (void *data));

/* Inserts the node into the Splay Tree */
int spTreeInsert(SPTree *tree, int key ,void *data);

/* Removes the node from the Splay Tree */
int spTreeRemove(SPTree *tree, int key);

/* Makes the node the new root of the tree */
static void splay(SPNode *root, SPNode *node);

/* Destroys the Splay Tree 
 * 	Attention: If you use MemPool then you must call freeMemory() in order to free the memory space pointed to by tree and its nodes 
 */
int destroySPTree(SPTree *tree);

#define L 0
#define R 1

#endif
