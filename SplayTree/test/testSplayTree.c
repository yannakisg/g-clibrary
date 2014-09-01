#include <stdio.h>

#include "../src/splaytree.h"

/* Inorder traversal */
void traverseTree(SPNode *root, int h) {
	if (root != NULL) {
		traverseTree(root->child[L], h+1);
		
		printf("(%d) Key => %d ", h, root->key);
		if (root->data != NULL)
			printf("Data => %d ", *((int *)root->data));
		if (root->parent != NULL)
			printf("Parent %d\n", root->parent->key);
		else
			printf("Parent NULL\n");
		
		traverseTree(root->child[R], h+1);
	}
}

#define SIZE 28

int main() {
	SPTree tree;
	
	initSPTree(&tree, NULL);
	
	//allocMem(SIZE,sizeof(SPNode));
	
	int array[SIZE] = {14,21,38,30,41,3,12,7,20,19 ,39,47,35,15,37,2317,4328,2317,32189,231,85,38593,271748,128,0,8,1,13};
	int i;
	
	for (i = 0; i < SIZE; i++) {
		spTreeInsert(&tree, array[i], &array[i]);
	}
	
	traverseTree(tree.root,0);
	
	spTreeRemove(&tree, 13);
	
	printf("\n");
	
	traverseTree(tree.root,0);
	
	destroySPTree(&tree);
	
	//freeMemory();

	return 0;
}
