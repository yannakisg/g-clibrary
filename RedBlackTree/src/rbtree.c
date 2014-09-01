#include "rbtree.h"

/*Initializes a Red Black Tree */
int initRBTree(RBTree *tree, void (*destroy) (void *data)) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	tree->root = NULL;
	tree->destroy = destroy;
	
	return 0;
}

/* Inserts a node into the Red Black Tree */
int rbTreeInsert(RBTree *tree, int key, void *data) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	RBNode *newNode,*iter,*parent,*uncle,*grandparent;
	
	/* If the tree is empty */
	if (tree->root == NULL) {
		newNode = (RBNode *) malloc(sizeof(RBNode));
		if (newNode == NULL) {
			errno = ENOMEM;
			return -2;
		}
		newNode->child[L] = newNode->child[R] = newNode->parent = NULL;
		newNode->color = BLACK;
		newNode->key = key;
		newNode->data = data;
		tree->root = newNode;
		
		return 0;
	}

	iter = tree->root;
	
	/* Finds the appropriate external node (like the insertion in a Binary Search tree)*/
	while (iter != NULL) {
		parent = iter;
		if (key < iter->key) 
			iter = iter->child[L];
		else if (key > iter->key) 
			iter = iter->child[R];
		else
			return 1;
	}
	
	newNode = (RBNode *) malloc(sizeof(RBNode));
	if (newNode == NULL) {
		errno = ENOMEM;
		return -2;
	}
	memset(newNode, 0, sizeof(RBNode));
	
	newNode->color = RED;
	newNode->key = key;
	newNode->data = data;
	newNode->parent = parent;
	if (parent != NULL) {
		if (newNode->key < parent->key)
			parent->child[L] = newNode;
		else
			parent->child[R] = newNode;
	}
	
	while (1) {
		
		/*Case 1 -> The new node is at the root of the tree. Repainted it black and return */
		if (parent == NULL) {
			tree->root = newNode;
			newNode->color = BLACK;
			return 0;
		}
		
		/*Case 2 -> The new node's parent is black so return */
		if (parent->color == BLACK) 
			return 0;
		
		
		uncle = parent->parent->child[L] == parent ? parent->parent->child[R] : parent->parent->child[L];
		
			
		/*Case 3 -> If both parent and uncle are red then repainted them black and the grandparent becomes red. 
		 * Perform again the entire procedure on grandparent */
		if (uncle != NULL && isRed(uncle)) {
			uncle->parent->color = RED;
			uncle->color = parent->color = BLACK;
			parent->parent->color = RED;
			
			newNode = parent->parent;
			parent = newNode->parent;
		} else 
			break;
	}
	
	grandparent = parent->parent;
	
	
	/*Case 4 -> The parent is red but the uncle is black. Also the new node is the right child of its parent
	 * and its parent is in turn the left child of the grandparent of the new node perform a left rotation at parent*/
	if (newNode == parent->child[R] && parent == grandparent->child[L]) {
		// left rotation 
		parent->child[R] = newNode->child[L];
		if (parent->child[R] != NULL)
			parent->child[R]->parent = parent;
			
		newNode->child[L] = parent;
		newNode->parent = grandparent;
		parent->parent = newNode;
		grandparent->child[L] = newNode;
		
		parent = newNode;
		newNode = newNode->child[L];
	}/* If the newNode is the left child of its parent and its parent is in turn the right child of the 
		grandparent of the new node , perform a right rotation at parent*/ 
	else if (newNode == parent->child[L] && parent == grandparent->child[R]){
		// right rotation
		parent->child[L] = newNode->child[R];
		if (parent->child[L] != NULL)
			parent->child[L]->parent = parent;
			
		newNode->child[R] = parent;
		newNode->parent = grandparent;
		parent->parent = newNode;
		grandparent->child[R] = newNode;
		
		parent = newNode;	
		newNode = newNode->child[R];
	}
	
		
	/*Case 5 -> The parent is red but the uncle black*/
	grandparent = parent->parent;
	grandparent->color = RED;
	
	/* If the newNode is the left child of its parent and its parent is in turn the left child of the 
		grandparent of the new node , perform a right rotation at grandparent*/ 
	if (newNode == parent->child[L] && parent == grandparent->child[L]) {
		// right rotation
		grandparent->child[L] = parent->child[R];
		if (grandparent->child[L] != NULL)
			grandparent->child[L]->parent = grandparent;
			
		parent->child[R] = grandparent;	
		parent->parent = grandparent->parent;
		grandparent->parent = parent;
		
		if (parent->parent != NULL) {
			if (grandparent == parent->parent->child[R])
				parent->parent->child[R] = parent;
			else
				parent->parent->child[L] = parent;
		} else {
			tree->root = parent;
		}
		parent->color = BLACK;
	} /* If the newNode is the right child of its parent and its parent is in turn the right child of the 
		grandparent of the new node , perform a left rotation at grandparent*/ 
	else if (newNode == parent->child[R] && parent == grandparent->child[R]) {
		// left rotation
		grandparent->child[R] = parent->child[L];
		if (grandparent->child[R] != NULL)
			grandparent->child[R]->parent = grandparent;
			
		parent->child[L] = grandparent;
		parent->parent = grandparent->parent;
		grandparent->parent = parent;
		
		if (parent->parent != NULL) {
			if (grandparent == parent->parent->child[R])
				parent->parent->child[R] = parent;
			else
				parent->parent->child[L] = parent;
		} else
			tree->root = parent;
		
		parent->color = BLACK;
	}
	return 0;
}

/* Removes a node from the Red Black Tree */
int rbTreeRemove(RBTree *tree, int key) {
	if (tree == NULL || tree->root == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	
	RBNode *iter,*remNode,*sibling;
	void *temp;
	int tKey;
	
	remNode = tree->root;
	
	/* Find the node with the specified key. If it does not exist return*/
	while (remNode != NULL) {
		if (key < remNode->key) 
			remNode = remNode->child[L];
		else if (key > remNode->key)
			remNode = remNode->child[R];
		else
			break;
	}
	
	if (remNode == NULL)
		return 1;
	
	iter = remNode;
	
	/* If the node is a red leaf or a root of a red black tree with one node just remove it and return*/
	if ((iter->color == RED || iter->parent == NULL) && iter->child[R] == NULL && iter->child[L] == NULL) {
		if (iter->parent == NULL)
			tree->root = NULL;
		else if (iter->parent->child[L] == iter)
			iter->parent->child[L] = NULL;
		else
			iter->parent->child[R] = NULL;
		
		if (tree->destroy != NULL)
				tree->destroy(iter->data);
		free(iter);
		return 0;
	}/* If it has an inorder predecessor two levels below it
		or If it is the root of the tree and the choose is 0x01  then find its predecessor */
	else if ( (iter->child[R] == NULL && iter->child[L] != NULL && iter->child[L]->child[R] != NULL) || (iter->child[R] != NULL && iter->child[R]->child[L] == NULL) || (tree->root == iter && choose == 0x01)) {
		//inorder predecessor
		if (tree->root == iter)
			choose = 0x00;
			
		if (iter->child[L] == NULL)
			iter = iter->child[R];
		else {
			iter = iter->child[L];
			while (iter->child[R] != NULL)
				iter = iter->child[R];
		}	
		
	}/* If it has an inorder successor two levels below it
		or If it is the root of the tree and the choose is 0x00  then find its successor */ 
	else if ( (iter->child[R] != NULL && iter->child[R]->child[L] != NULL) || (iter->child[L] != NULL && iter->child[L]->child[R] == NULL) || (tree->root == iter && choose == 0x00)) {
		//inorder successor
		if (tree->root == iter)
			choose = 0x01;
		
		if (iter->child[R] == NULL)
			iter = iter->child[L];
		else {
			iter = iter->child[R];
			while (iter->child[L] != NULL)
				iter = iter->child[L];
		}
	}
	
	/* Exchange the data and the key of the predecessor/successor for them of the node to be removed */
	temp = remNode->data;
	remNode->data = iter->data;
	iter->data = temp;
	
	tKey = remNode->key;
	remNode->key = iter->key;
	iter->key = tKey;
	
	temp = remNode;
	remNode = iter;
	iter = (RBNode *)temp;
	
	/* If the predecessor/successor has not any child and is a red one the update the child pointer of its parent*/
	if (remNode->child[R] == NULL && remNode->child[L] == NULL && remNode->color == RED) {
		if (iter->child[R] == remNode)
			iter->child[R] = NULL;
		else if (iter->child[L] == remNode)
			iter->child[L] = NULL;
		else {
			if (remNode->parent->child[L] == remNode)	
				remNode->parent->child[L] = NULL;
			else
				remNode->parent->child[R] = NULL;
		}
	}/* If the successor has a right child , then make it the left child of its parent*/ 
	else if (remNode->child[R] != NULL) {
		temp = remNode->data;
		remNode->data = remNode->child[R]->data;
		remNode->child[R]->data = temp;
		
		tKey = remNode->key;
		remNode->key = remNode->child[R]->key;
		remNode->child[R]->key = tKey;
		
		temp = remNode->child[R];
		remNode->child[R] = NULL;
		
		remNode = (RBNode *)temp;
	}/* If the predecessor has a left child , then make it the right child of its parent*/ 
	else if (remNode->child[L] != NULL) {
		temp = remNode->data;
		remNode->data = remNode->child[L]->data;
		remNode->child[L]->data = temp;
		
		tKey = remNode->key;
		remNode->key = remNode->child[L]->key;
		remNode->child[L]->key = tKey;
		
		temp = remNode->child[L];
		remNode->child[L] = NULL;
		
		remNode = (RBNode *)temp;
	}
	
	/* If the removed node is black */
	if (remNode->color == BLACK) {
		/* If the successor - predecessor is red , just repainted it black and return*/
		if (iter->color == RED) {
			iter->color = BLACK;
		} else {
			iter = remNode;
			while (1) {
				/*Case 1 -> Iter is the new root , so return */
				if (iter->parent == NULL) {
					tree->root = iter;
					if (tree->destroy != NULL)
						tree->destroy(remNode->data);
					free(remNode);
					return 0;
				}
				
				if (iter->parent->child[R] == iter)
					sibling = iter->parent->child[L];
				else
					sibling = iter->parent->child[R];
				
				/*Case 2 ->  If sibling is red , the we reverse the colors of parent and its sibling*/	
				if (sibling->color == RED) {
					iter->parent->color = RED;
					sibling->color = BLACK;
					
					/*If iter is the left child of its parent perform a left rotation */
					if (iter == iter->parent->child[L]) {
						//left rotation
						iter->parent->child[R] = sibling->child[L];
						sibling->child[L]->parent = iter->parent;
					
						sibling->child[L] = iter->parent;
						sibling->parent = iter->parent->parent;
						iter->parent->parent = sibling;
					
						if (sibling->parent != NULL) {
							if (sibling->parent->child[R] == iter->parent)
								sibling->parent->child[R] = sibling;
							else
								sibling->parent->child[L] = sibling;
						}
					}/* else perform a right rotation */ 
					else {
						//right rotation
						iter->parent->child[L] = sibling->child[R];
						sibling->child[R]->parent = iter->parent;
					
						sibling->child[R] = iter->parent;
						sibling->parent = iter->parent->parent;
						iter->parent->parent = sibling;
					
						if (sibling->parent != NULL) {
							if (sibling->parent->child[R] == iter->parent)
								sibling->parent->child[R] = sibling;
							else
								sibling->parent->child[L] = sibling;
						}	
					}
				}	
				
				if (iter->parent->child[R] == iter)
					sibling = iter->parent->child[L];
				else
					sibling = iter->parent->child[R];
				
				/* Case 3 -> if parent is black and sibling black then repaint sibling red and perform again the same procedure on parent */
				if (iter->parent->color == BLACK && sibling->color == BLACK && (sibling->child[L] == NULL || sibling->child[L]->color == BLACK) && (sibling->child[R] == NULL || sibling->child[R]->color == BLACK)) {
					sibling->color = RED;
					if (iter == remNode) {
						if (iter->parent->child[R] == iter)
							iter->parent->child[R] = NULL;
						else
							iter->parent->child[L] = NULL;
					}
					iter = iter->parent;
				} else
					break;
			}
			
			if (iter->parent->child[R] == iter)
				sibling = iter->parent->child[L];
			else
				sibling = iter->parent->child[R];
			
			/*Case 4 -> If sibling is red and its children black , repaint sibling red and its parent black */
			if (iter->parent->color == RED && sibling->color == BLACK && (sibling->child[L] == NULL || sibling->child[L]->color == BLACK) && (sibling->child[R] == NULL || sibling->child[R]->color == BLACK)) {
				sibling->color = RED;
				iter->parent->color = BLACK;
			} else { 
				
				if (iter->parent->child[R] == iter)
					sibling = iter->parent->child[L];
				else
					sibling = iter->parent->child[R];
				
				/* Case 5 -> If sibling is black */
				if (sibling->color == BLACK) {
					/* If iter is the left child of its parent and the right child of sibling is black while the left one red
					 * repaint the sibling red and its left child black and perform a right rotation at sibling */
					if (iter == iter->parent->child[L] &&(sibling->child[R] == NULL || sibling->child[R]->color == BLACK) && sibling->child[L]->color == RED) {
						sibling->color = RED;
						sibling->child[L]->color = BLACK;
						
						//right rotation
						RBNode *nS  = sibling->child[L];
						
						sibling->child[L] = sibling->child[L]->child[R];
						if (sibling->child[L] != NULL)
							sibling->child[L]->parent = sibling;
						
							
						nS->child[R] = sibling;
						
					
						if (sibling->parent != NULL) {
							if (sibling->parent->child[R] == sibling)
								sibling->parent->child[R] = nS;
							else
								sibling->parent->child[L] = nS;
						}	
						sibling->parent = nS;
					}/* If iter is the right child of its parent and the left child of sibling is black while the right one red
					 * repaint the sibling red and its right child black and perform a left rotation at sibling */
					 else 	if (iter == iter->parent->child[R] &&  (sibling->child[L] == NULL || sibling->child[L]->color == BLACK )&& sibling->child[R]->color == RED) {
						sibling->color = RED;
						sibling->child[R]->color = BLACK;
						
						//left rotation
						RBNode *nS  = sibling->child[R];
						
						sibling->child[R] = sibling->child[R]->child[L];
						if (sibling->child[R] != NULL)
							sibling->child[R]->parent = sibling;
						
							
						nS->child[L] = sibling;
						
					
						if (sibling->parent != NULL) {
							if (sibling->parent->child[R] == sibling)
								sibling->parent->child[R] = nS;
							else
								sibling->parent->child[L] = nS;
						}	
						sibling->parent = nS;
					}
				}	
				
				if (iter->parent->child[R] == iter)
					sibling = iter->parent->child[L];
				else
					sibling = iter->parent->child[R];
				
				
				sibling->color = iter->parent->color;
				iter->parent->color = BLACK;
				
				/* Case 6 -> If iter is the left child of its parent perform a left rotation on its parent and repaint the right child of sibling black */
				if (iter == iter->parent->child[L]) {
					sibling->child[R]->color = BLACK;
					//left rotation
						iter->parent->child[R] = sibling->child[L];
						if (sibling->child[L] != NULL)
							sibling->child[L]->parent = iter->parent;
					
						sibling->child[L] = iter->parent;
						sibling->parent = iter->parent->parent;
						
						if (iter->parent->parent != NULL) {
							if (iter->parent->parent->child[R] == iter->parent)
								iter->parent->parent->child[R] = sibling;
							else
								iter->parent->parent->child[L] = sibling;
						}
						
						iter->parent->parent = sibling;
					
						
					}
					/* else perform a right rotation and repaint the left child of sibling black */
					else {
						sibling->child[L]->color = BLACK;
						//right rotation
						iter->parent->child[L] = sibling->child[R];
						if (sibling->child[R] != NULL)
							sibling->child[R]->parent = iter->parent;
					
						sibling->child[R] = iter->parent;
						sibling->parent = iter->parent->parent;
						if (iter->parent->parent != NULL) {
							if (iter->parent->parent->child[R] == iter->parent)
								iter->parent->parent->child[R] = sibling;
							else
								iter->parent->parent->child[L] = sibling;
						}	
						
						iter->parent->parent = sibling;
						
					}
					
					if (sibling->parent == NULL)
						tree->root = sibling;
				
			}
		}
	}
	
	if (iter == remNode) {
		if (iter->parent->child[R] == iter)
			iter->parent->child[R] = NULL;
		else
			iter->parent->child[L] = NULL;
	}
			
	if (tree->destroy != NULL)
		tree->destroy(remNode->data);
		
	free(remNode);
			
	return 0;
}

/* Destroys the Red Black Tree 
 * 	Attention: If you use MemPool then you must call freeMemory() in order to free the memory space pointed to by tree and its nodes 
 */
int rbTreeDestroy(RBTree *tree) {
	if (tree == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	while (tree->root != NULL) {
		rbTreeRemove(tree, tree->root->key);
	}
	
	memset(tree, 0, sizeof(RBTree));
	
	return 0;
}
