#ifndef _TWTHRTREE_H_
#define _TWTHRTREE_H_

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "../../MemPool/src/mempool.h"

#define malloc(size) getMemNode()
#define free(ptr) releaseMemNode(ptr)



/* 2-3 Node Representation
 * 	Each node contains 2 data items,up to 3 children and 1 parent
 */
typedef struct TTTreeNode_ {
	void *data1, *data2;
	struct TTTreeNode_ *child[3];
	struct TTTreeNode_ *parent;
}TTTreeNode;

/*
 * 2-3 Tree
 *	root points to the root of the tree
 */
typedef struct TTTree_ {
	int (*compare) (const void *key1, const void *key2);
	void (*destroy) (void *data);
	TTTreeNode *root;
}TTTree;

/*Initializes a 2-3 Tree */
int initTTTree(TTTree *tree, void (*destroy) (void *data), int (*compare) (const void *key1, const void *key2));

/* Inserts a node into the 2-3 Tree */
int ttTreeInsert(TTTree *tree, void *data);

/* Searches for the node specified by data 
 *	Returns 0 on success
 */
int ttTreeSearch(TTTree *tree, void *data);

/* Destroys the 2-3 Tree 
 * 	Attention: You must call freeMemory() in order to free the memory space pointed to by tree and its nodes
 */
int destroyTTree(TTTree *tree);

/*Helpful Definitions */
#define F 0 // F -> The First child of a Node
#define M 1 // M -> The Middle child of a Node
#define L 2 // L -> The Last child of a Node
#define isFull(node) ( node != NULL && node->data1 != NULL && node->data2 != NULL)
#endif
