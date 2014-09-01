#include "twthrtree.h"

/*Initializes a 2-3 Tree */
int initTTTree(TTTree *tree, void (*destroy) (void *data), int (*compare) (const void *key1, const void *key2)) {
	if (tree == NULL || compare == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	tree->destroy = destroy;
	tree->compare = compare;
	tree->root = NULL;
	
	return 0;
}

/* Inserts a node into the 2-3 Tree */
int ttTreeInsert(TTTree *tree, void *data) {
	if (tree == NULL || data == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	TTTreeNode *iter, *newNode, *p = NULL,*temp = NULL,*newPar = NULL;
	void *m,*newM;
	int res;
	
	/* If the tree is empty insert the root node */
	if (tree->root == NULL) {
		newNode = (TTTreeNode *)malloc(sizeof(TTTreeNode ));
		if (newNode == NULL) {
			errno = ENOMEM;
			return -2;
		}
		memset(newNode,0,sizeof(TTTreeNode));
		newNode->data1 = data;
		tree->root = newNode;
		
		return 0;
	}
	
	iter = tree->root;
	
	/* Find the appropriate leaf in order to put the item in the split nodes if necessary */
	while (iter->child[F] != NULL) {
		res = tree->compare(iter->data1, data);
		if (res == 0) {
			return 1;
		}
		if (res > 0) {			
			iter = iter->child[F];
		}  
		else if (iter->data2 != NULL && tree->compare(data, iter->data2) > 0) {
				iter = iter->child[L];
		} else 	
			iter = iter->child[M];	
	}
	/* "iter" points to the leaf */
	
	/* If the leaf is not full add the item to it*/
	if (iter->data2 == NULL) {
		if (tree->compare(data, iter->data1) >= 0)
			iter->data2 = data;
		else {
			iter->data2 = iter->data1;
			iter->data1 = data;
		}
	} else {
		/* Else split the leaf */		
		while (1) {
			newNode = (TTTreeNode *)malloc(sizeof(TTTreeNode ));
			if (newNode == NULL) {
				errno = ENOMEM;
				return -2;
			}
			memset(newNode, 0, sizeof(TTTreeNode));	
			
			/* Determine the smallest , middle and largest item. 
			 *The smallest and the largest item are located into the node pointed to by "iter"
			 * The middle is pointed to by "m"
			 */ 
			res = tree->compare(iter->data1, data);
			if (res > 0) {
				m = iter->data1;
				iter->data1 = data;
			} else if (res < 0) {
				if (tree->compare(iter->data2, data) > 0) 
					m = data;
				else {
					m = iter->data2;
					iter->data2 = data;
				}
			}
			
			p = iter->parent;
			
			/* If the leaf is full but it's parent is not
			 *	split the leaf. The parent now is full
			 */
			if (isFull(iter)  && (!isFull(p))) {
				
				newNode->data1 = iter->data2;
				iter->data2 = NULL;
				newNode->parent = p;
				
				/* Update the children of the nodes pointed to by "newNode", "iter" , and "newPar" */
				if (temp != NULL) {
					if (temp == iter->child[M]) {
						newNode->child[F] = newPar;
						newPar->parent = newNode;
						newNode->child[M] = iter->child[L];
						newNode->child[M]->parent = newNode;
						iter->child[L] = NULL;
					} else if (temp == iter->child[F]){
						newNode->child[M] = iter->child[L];
						newNode->child[M]->parent = newNode;
						iter->child[L] = NULL;
					
						newNode->child[F] = iter->child[M];
						newNode->child[F]->parent = newNode;
								
						iter->child[M] = newPar;
						newPar->parent = iter;
					} else {
						newNode->child[F] = temp;
						newNode->child[M] = newPar;
						temp->parent = newNode;
						newPar->parent = newNode;
							
						iter->child[L] = NULL;
					}
				}
				
				/* If "iter" is the root of the tree */
				if (p == NULL) {
					TTTreeNode *newRoot = (TTTreeNode *)malloc(sizeof(TTTreeNode ));
						
					if (newRoot == NULL) {
						errno = ENOMEM;
						return -2;
					}
					memset(newRoot, 0, sizeof(TTTreeNode));
					newRoot->data1 = m;
					newRoot->child[F] = iter;
					iter->parent = newRoot;
					newRoot->child[M] = newNode;
					newNode->parent = newRoot;
					tree->root = newRoot;
						
				} /* else if "iter" is the first child */ 
				else if (iter == p->child[F]) {
					p->data2 = p->data1;
					p->data1 = m;
					
					p->child[L] =p->child[M];
					p->child[M] = newNode;
				} else /* else if "iter" is the middle child */  {
					p->data2 = m;
					p->child[L] = newNode;
				}
				
				break;
			} /* else if the leaf and its parent are full split both of them */ 
			else if ( isFull(iter) && isFull(p)){
				
				newNode->data1 = iter->data2;
				iter->data2 = NULL;
				
				/* Update the children of the nodes pointed to by "newNode", "iter" , and "newPar" */
				if (temp != NULL) {
					if (temp == iter->child[M]) {
						newNode->child[F] = newPar;
						newPar->parent = newNode;
						newNode->child[M] = iter->child[L];
						newNode->child[M]->parent = newNode;
						iter->child[L] = NULL;
					} else if (temp == iter->child[F]){
						newNode->child[M] = iter->child[L];
						newNode->child[M]->parent = newNode;
						iter->child[L] = NULL;
					
						newNode->child[F] = iter->child[M];
						newNode->child[F]->parent = newNode;
								
						iter->child[M] = newPar;
						newPar->parent = iter;
					} else {
						newNode->child[F] = temp;
						newNode->child[M] = newPar;
						temp->parent = newNode;
						newPar->parent = newNode;
								
						iter->child[L] = NULL;
					}
				}
				
				
				newPar = (TTTreeNode *)malloc(sizeof(TTTreeNode ));
				if (newPar == NULL) {
					errno = ENOMEM;
					return -2;
				}
				memset(newPar, 0, sizeof(TTTreeNode));
				
				/* Determine the new middle data.  */
				res = tree->compare(p->data1, m);
				if (res > 0) {
					newM = p->data1;
					p->data1 = m;
				} else if (res < 0) {
					if (tree->compare(p->data2, m) > 0) 
						newM = m;
					else {
						newM = p->data2;
						p->data2 = m;
					}
				} else {
					return 2;
				}
				
				newPar->data1 = p->data2;
				p->data2 = NULL;
				
				/* if "iter" is the first child */
				if (iter == p->child[F]) {
					newPar->child[F] = p->child[M];
					newPar->child[M] = p->child[L];
					newPar->child[F]->parent = newPar;
					newPar->child[M]->parent = newPar;
					
					p->child[L] = NULL;
					p->child[M] = newNode;
					newNode->parent = p;
					
				}/*else if "iter" is the middle child */ 
				else if (iter == p->child[M]) {
					newPar->child[L] = p->child[L];
					newPar->child[M] = newNode;
					newPar->child[L]->parent = newPar;
					newPar->child[M]->parent = newPar;
					
					p->child[L] = NULL;
					
				}/*else if "iter" is the last child */  
				else {
					p->child[L] = NULL;
					
					newPar->child[M] = newNode;
					newPar->child[F] = iter;
					iter->parent = newPar;
					newNode->parent = newPar;
				}
				
				/* if its grandfather is null add a new root pointed to by newNode*/
				if (p->parent == NULL) {
						newNode = (TTTreeNode *)malloc(sizeof(TTTreeNode ));
						if (newNode == NULL) {
							errno = ENOMEM;
							return -2;
						}
						memset(newNode, 0, sizeof(TTTreeNode));
						
						newNode->data1 = newM;
						
						if (tree->compare(p->data1, newPar->data1) > 0) {
							newNode->child[F] = newPar;
							newNode->child[M] = p;
						} else {
							newNode->child[F] =p;
							newNode->child[M] = newPar;
						}
						
						p->parent = newNode;
						newPar->parent = newNode;
						tree->root = newNode;
						break;
				} /* else if its grandfather is not full , insert the item into it*/
				 else if (p->parent->data2 == NULL) {
					
					if (tree->compare(p->parent->data1, newM) > 0) {
						p->parent->data2 = p->parent->data1;
						p->parent->data1 = newM;
						
						if (tree->compare(iter->data1, newPar->data1) > 0) {
							p->parent->child[F] = newPar;
							p->parent->child[L] = p->parent->child[M];
							p->parent->child[M] = p;
						} else {
							p->parent->child[F] = p;
							p->parent->child[L] = p->parent->child[M];
							p->parent->child[M] = newPar;
						}
						
						newPar->parent = p->parent;
					} else {
						p->parent->data2 = newM;
						
						if (tree->compare(iter->data1, newPar->data1) > 0) {
							p->parent->child[M] = newPar;
							p->parent->child[L] = p;
						} else {
							p->parent->child[M] = p;
							p->parent->child[L] = newPar;
						}
						
						newPar->parent = p->parent;
					}
					break;
				} else {
					data = newM;
					temp = p;
					iter = p->parent;
				}	
			} 
		}
	}
	return 0;
}

/* Searches for the node specified by data 
 *	Returns 0 on success
 */
int ttTreeSearch(TTTree *tree, void *data) {
	if (tree == NULL || data == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	TTTreeNode *iter;
	int res;
	
	iter = tree->root;
	
	/* Find the appropriate leaf in order to put the item in the split nodes if necessary */
	while (iter != NULL) {
		res = tree->compare(iter->data1, data);
		if (res == 0) {
			break;
		}
		if (res > 0) {			
			iter = iter->child[F];
		}  
		else if (iter->data2 != NULL && tree->compare(data, iter->data2) > 0) {
				iter = iter->child[L];
		} else 	
			iter = iter->child[M];	
	}
	
	if (iter != NULL) 
		return 0;
	else
		return 1;
}

/* Helper function in order to free each node. Postorder traversal */
static void destroyNode(TTTreeNode *root, void (*destroy) (void *data)) {
	if (root != NULL) {
		destroyNode(root->child[F],destroy);
		destroyNode(root->child[M],destroy);
		destroyNode(root->child[L],destroy);
		
		if (destroy != NULL) {
			if (root->data1 != NULL)
				destroy(root->data1);
			if (root->data2 != NULL)
				destroy(root->data2);
		}
		
		free(root);
	}
}

/* Destroys the 2-3 Tree 
 * 	Attention: You must call freeMemory() in order to free the memory space pointed to by tree and its nodes
 */
int destroyTTree(TTTree *tree) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	destroyNode(tree->root,tree->destroy);
	
	memset(tree, 0, sizeof(TTTree));
	
	return 0;
}
