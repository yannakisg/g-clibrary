#include "splaytree.h"

/* Inititalizes a Splay Tree */
int initSPTree(SPTree *tree, void (*destroy) (void *data)) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	tree->root = NULL;
	tree->destroy = destroy;

	return 0;
}

/* Inserts the node into the Splay Tree */
int spTreeInsert(SPTree *tree, int key ,void *data) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	SPNode *newNode,*iter,*parent;
	
	/* If the tree is empty insert new root and return */
	if (tree->root == NULL) {
		newNode = (SPNode *) malloc(sizeof(SPNode));
		if (newNode == NULL) {
			errno = ENOMEM;
			return -2;
		}
		newNode->parent = newNode->child[L] = newNode->child[R] = NULL;
		newNode->data = data;
		newNode->key = key;
		tree->root = newNode;
		return 0;
	}
	
	/* Use the same insertion method as with a binary search tree */
	iter = tree->root;
	
	while (iter != NULL) {
		parent = iter;
		if (key < iter->key)
			iter = iter->child[L];
		else if(key > iter->key)
			iter = iter->child[R];
		else
			return 1;
	}
	
	newNode = (SPNode *) malloc(sizeof(SPNode));
	if (newNode == NULL) {
		errno = ENOMEM;
		return -2;
	}
	memset(newNode,0,sizeof(SPNode));
	
	newNode->parent = parent;
	if (key < parent->key) 
		parent->child[L] = newNode;
	else
		parent->child[R] = newNode;
	
	newNode->data = data;
	newNode->key = key;
	
	/* Splay the new node to the top of the tree */
	splay(tree->root, newNode);
	
	tree->root = newNode;
	return 0;
}

/* Makes the node the new root of the tree */
static void splay(SPNode *root, SPNode *node) {
	
	while (node->parent != NULL) {
		/* If node's parent is the root */
		if (node->parent == root) {
			/* If node is a left child perform a right rotation */
			if (node->parent->child[L] == node) {
				//Right rotation
				node->parent->child[L] = node->child[R];
				if (node->child[R] != NULL)
					node->child[R]->parent = node->parent;
					
				node->child[R] = node->parent;			
			}/* else a left and return */ 
			else {
				//Left rotation
				node->parent->child[R] = node->child[L];
				if (node->child[L] != NULL)
					node->child[L]->parent = node->parent;
				
				node->child[L] = node->parent;
			}
			node->parent->parent = node;
			node->parent = NULL;
			return ;
		} else {
			SPNode *grandparent = node->parent->parent;
			SPNode *temp;
			
			/* If node is the left child of its parent and its parent is a left child of node's grandparent 
			 * perform a double right rotation */
			if (node == node->parent->child[L] && node->parent == grandparent->child[L]) {
				// double right rotation
				grandparent->child[L] = node->parent->child[R];
				if (node->parent->child[R] != NULL)
					node->parent->child[R]->parent = grandparent;
							
				node->parent->child[L] = node->child[R];
				if (node->child[R] != NULL)
					node->child[R]->parent = node->parent;
				
				node->parent->child[R] = grandparent;
				node->child[R] = node->parent;
				
				temp = grandparent->parent;
				grandparent->parent = node->parent;
				node->parent->parent = node;
				node->parent = temp;
				
				if (temp != NULL) {
					if (temp->child[L] == grandparent)
						temp->child[L] = node;
					else
						temp->child[R] = node;
				}
				
			} 
			/* If node is the right child of its parent and its parent is a right child of node's grandparent 
			 * perform a double left rotation */
			else if (node == node->parent->child[R] && node->parent == grandparent->child[R]) {
				//double left rotation

				grandparent->child[R] = node->parent->child[L];
				if (node->parent->child[L] != NULL)
					node->parent->child[L]->parent = grandparent;
							
				node->parent->child[R] = node->child[L];
				if (node->child[L] != NULL)
					node->child[L]->parent = node->parent;
				
				node->parent->child[L] = grandparent;
				node->child[L] = node->parent;
				
				temp = grandparent->parent;
				grandparent->parent = node->parent;
				node->parent->parent = node;
				node->parent = temp;
				if (temp != NULL) {
					if (temp->child[L] == grandparent)
						temp->child[L] = node;
					else
						temp->child[R] = node;
				}
			} 
			/* If node is the right child of its parent and its parent is a left child of node's grandparent 
			 * perform a left rotation at parent and then a right at node*/
			else if (node == node->parent->child[R] && grandparent->child[L] == node->parent) {
				
				//left-right rotation (parent,node)

				grandparent->child[L] = node->child[R];
				if (node->child[R] != NULL)
					node->child[R]->parent = grandparent;
				node->child[R] = grandparent;
				
				node->parent->child[R] = node->child[L];
				if (node->child[L] != NULL)
					node->child[L]->parent = node->parent;
				node->child[L] = node->parent;
				
				node->parent->parent = node;
				node->parent = grandparent->parent;
				grandparent->parent = node;
				if (node->parent != NULL) {
					if (node->parent->child[L] == grandparent) 
						node->parent->child[L] = node;
					else
						node->parent->child[R] = node;
				}
				
			}/* If node is the left child of its parent and its parent is a right child of node's grandparent 
			 * perform a right rotation at parent and then a left at node*/
			 else if (node == node->parent->child[L] && grandparent->child[R] == node->parent) {
				//right-left rotation (parent,node)

				grandparent->child[R] = node->child[L];
				if (node->child[L] != NULL)
					node->child[L]->parent = grandparent;
				node->child[L] = grandparent;
				
				node->parent->child[L] = node->child[R];
				if (node->child[R] != NULL)
					node->child[R]->parent = node->parent;
				node->child[R] = node->parent;
				
				node->parent->parent = node;
				node->parent = grandparent->parent;
				grandparent->parent = node;				
				if (node->parent != NULL) {
					if (node->parent->child[L] == grandparent)
						node->parent->child[L] = node;
					else
						node->parent->child[R] = node;
				}
			}			
		}
	}
}

/* Removes the node from the Splay Tree */
int spTreeRemove(SPTree *tree, int key) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	SPNode *iter = tree->root, *remNode;
	void *t;
	/* Use the same removal method as with a binary search tree */
	while (iter != NULL) {
		if (key < iter->key)
			iter = iter->child[L];
		else if (key > iter->key)
			iter = iter->child[R];
		else
			break;
	}
	
	if (iter == NULL)
		return 1;
	
	remNode = iter;
	
	if (iter->child[L] != NULL) {
		//in order predecessor
		iter = iter->child[L];
		while (iter->child[R] != NULL)
			iter = iter->child[R];
	} else if (iter->child[R] != NULL) {
		//in order successor
		iter = iter->child[R];
		while (iter->child[L] != NULL)
			iter = iter->child[L];
	}
	
	remNode->key = iter->key;
	t = remNode->data ;
	remNode->data = iter->data;
	iter->data = remNode;
	
	remNode = iter;
	iter = remNode->parent;

	if (remNode->child[L] != NULL) {
		if (remNode->parent->child[L] == remNode) {
			remNode->parent->child[L] = remNode->child[L];
			remNode->child[L]->parent = remNode->parent;
		} else {
			remNode->parent->child[R] = remNode->child[L];
			remNode->child[L]->parent = remNode->parent;
		}
	} else if (remNode->child[R] != NULL) {
		if (remNode->parent->child[L] == remNode) {
			remNode->parent->child[L] = remNode->child[R];
			remNode->child[R]->parent = remNode->parent;
		} else {
			remNode->parent->child[R] = remNode->child[R];
			remNode->child[R]->parent = remNode->parent;
		}
	} else if (remNode->parent != NULL) {
		
		if (remNode->parent->child[L] == remNode) 
			remNode->parent->child[L] = NULL;
		else
			remNode->parent->child[R] = NULL;
	} 

	if (tree->destroy != NULL)
		tree->destroy(remNode->data);
	free(remNode);
	
	/* Splay the parent of the removed node to the top of the tree */
	if (iter != NULL)
		splay(tree->root, iter);
	
	tree->root = iter;
	
	return 0;
}

/* Destroys the Splay Tree 
 * 	Attention: If you use MemPool then you must call freeMemory() in order to free the memory space pointed to by tree and its nodes 
 */
int destroySPTree(SPTree *tree) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	while (tree->root != NULL) 
		spTreeRemove(tree, tree->root->key);
	
	memset(tree,0,sizeof(SPTree));
	
	return 0;
}
