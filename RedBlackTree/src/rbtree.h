#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* Uncomment the below lines to use MemPool instead of system's default functions malloc and free */ 

//#include "../../MemPool/src/mempool.h"
//#define MEMPOOL 

#ifdef MEMPOOL
	#define malloc(size) getMemNode()
	#define free(ptr) releaseMemNode(ptr)
#endif

/* Red Black Node
 *	Each contains up to 2 children , one parent , one key and one pointer to data
 */
typedef struct RBNode_ {
	enum Color { RED, BLACK } color;
	struct RBNode_ *child[2];
	struct RBNode_ *parent;
	void *data;
	int key;
}RBNode;

/* Red Black Tree */
typedef struct RBTree_ {
	void (*destroy) (void *data);
	RBNode *root;
}RBTree;

static char choose = 0x00; 

/*Initializes a Red Black Tree */
int initRBTree(RBTree *tree, void (*destroy) (void *data));

/* Inserts a node into the Red Black Tree */
int rbTreeInsert(RBTree *tree, int key, void *data);

/* Removes a node from the Red Black Tree */
int rbTreeRemove(RBTree *tree, int key);

/* Destroys the Red Black Tree 
 * 	Attention: If you use MemPool then you must call freeMemory() in order to free the memory space pointed to by tree and its nodes 
 */
int rbTreeDestroy(RBTree *tree);

/* Helpful Definitions */
#define L 0
#define R 1
#define isRed(node) (node != NULL && node->color == RED)
#define isBlack(node) (node != NULL && node->color == BLACK)

#endif
