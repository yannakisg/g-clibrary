#include <stdio.h>

#include "../../Stack/src/stack.h"
#include "../src/rbtree.h"

/* Inorder traversal */
void traverseTree(RBNode *root, int h) {
	if (root != NULL) {
		traverseTree(root->child[L], h+1);
		
		printf("(%d) Key => %d ", h, root->key);
		if (root->data != NULL)
			printf("Data => %d ", *((int *)root->data));
		if (root->color == RED)
			printf("RED ");
		else
			printf("BLACK ");
		if (root->parent != NULL)
			printf("Parent %d\n", root->parent->key);
		else
			printf("Parent NULL\n");
		
		traverseTree(root->child[R], h+1);
	}
}

/* Checks if the Red Black Tree is a valid one */
void debugRBTree(RBTree *rb) {
	if (rb->root == NULL) 
		return;
		
	RBNode *l,*r,*iter,*temp;
	int black = 0;
	Stack stack;
	
	if (initStack(&stack, NULL) != 0) {
 		printf("Error while initializing stack\n");
 		return;
	}
	
	if (push(&stack, rb->root) != 0) {
		printf("Error while adding an element into the stack\n");
		return;
	}
	
	
	while (stack.size != 0) {

		if (pop(&stack, (void **)&iter) != 0) {
			printf("Error while removing the top element from the stack\n");
			return ;
		}
		
		l = iter->child[L];
		r = iter->child[R];
	
		if (isRed(iter)) {
			if (isRed(l) || isRed(r)) {
				printf("Red violation\n");
				return;
			}
		}

		if ( (l != NULL && iter->key <= l->key) || (r != NULL && r->key < iter->key)) {
			printf("Binary tree violation\n");
			return;
		}
		
		temp = iter;
		while (temp != NULL) {
			temp = temp->child[L];
			if (isBlack(temp))
				black++;
		}
		temp = iter;
		while (temp != NULL) {
			temp = temp->child[R];
			if (isBlack(temp))
				black--;
		}
		if (black != 0) {
			printf("Black => %d, for iter => %d\n",black,iter->key);
			printf("Black violation\n");
			return ;
		}
		
		
		if (l != NULL) {
			if (push(&stack, l) != 0) {
				printf("Error while adding an element into the stack\n");
				return;
			}
		}
		if (r != NULL) {
			if (push(&stack, r) != 0) {
				printf("Error while adding an element into the stack\n");
				return;
			}
		}
	}
	printf("Valid Red Black Tree :D\n");
}

#define SIZE 28

int main() {
	RBTree tree;
	
	initRBTree(&tree, NULL);
	
	//allocMem(SIZE,sizeof(RBNode));
	
	int array[SIZE] = {14,21,38,30,41,3,12,7,20,19 ,39,47,35,15,37,2317,4328,2317,32189,231,85,38593,271748,128,0,8,1,13};
	int i;
	
	for (i = 0; i < SIZE; i++)
		rbTreeInsert(&tree, array[i], &array[i]);
	
	traverseTree(tree.root,0);
	
	debugRBTree(&tree);
	
	printf("\nLets remove the root %d\n",tree.root->key);
	
	rbTreeRemove(&tree, tree.root->key);
	
	traverseTree(tree.root,0);
	
	debugRBTree(&tree);
	
	printf("New root %d\n",tree.root->key);
	
	rbTreeDestroy(&tree);
	
	//freeMemory();
		
	return 0;
}
