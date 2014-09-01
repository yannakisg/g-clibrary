#include <stdio.h>

#include "../src/twthrtree.h"

/* Inorder Traversal */
void traverseTree(TTTreeNode *root, int h) {
	if (root != NULL) {
		traverseTree(root->child[F],h+1);
		if (root->data1 != NULL) {
			printf("(%d) Data1-> %d ",h, *((int*)root->data1));
			if (root->parent != NULL) 
				printf("parent -> %d\n",*((int*)root->parent->data1));
			else
				printf("Null parent\n");
		}
		traverseTree(root->child[M],h+1);
		if (root->data2 != NULL)  {
			printf("(%d) Data2-> %d ",h, *((int*)root->data2));		
			if (root->parent != NULL) 
				printf("parent -> %d\n",*((int*)root->parent->data1));
			else
				printf("Null parent\n");
		}
		traverseTree(root->child[L],h+1);

	}
}

int compare(const void *key1, const void *key2) {
	int k1,k2;
	
	k1 = *((int *)key1);
	k2 = *((int *)key2);
	
	if (k1 > k2)
		return 1;
	else if (k1 == k2)
		return 0;
	else
		return -1;
}

#define SIZE 35

int main() {
	TTTree tree;
	
	initTTTree(&tree, NULL, compare);
	
	allocMem(SIZE,sizeof(TTTreeNode));
	
	int array[SIZE] = {
						50,60,70,40,30,20,10,
						80,90,100,75,45,25,15,
						110,95,120,48,12,92,72,
						69,68,76,98,99,74,46,
						51,55,27,35,33,34,36
					};
	int i,res;
	
	for (i = 0; i < SIZE; i++) 
		ttTreeInsert(&tree, &array[i]);
	
	traverseTree(tree.root,0);
	
	i = 20;
	res = ttTreeSearch(&tree, &i);
	if (res != 0)
		printf("The specified item does not exist\n");
	else
		printf("The item was found :D\n");
		
	i = 200000;
	res = ttTreeSearch(&tree, &i);
	if (res != 0)
		printf("The specified item does not exist\n");
	else
		printf("The item was found :D\n");
	
	destroyTTree(&tree);
	
	freeMemory();

	return 0;
}
