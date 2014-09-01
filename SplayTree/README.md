Splay Tree 0.1

Description:
A Splay Tree implementation. 

A splay Tree is a self-balancing binary search tree with the additional property that recently accessed elements are
quick to access again.

           
Example:
#include "../src/splaytree.h"

int main() {
// ...
	SPTree tree;
	
	initSPTree(&tree,destroy);
	
	//allocMem(SIZE,sizeof(SPNode));
// ...
	spTreeInsert(&tree, key0, (void *)data0);
	spTreeInsert(&tree, key1, (void *)data1);
// ...
	spTreeRemove(&tree, key);
// ...
	destroySPTree(&tree);
	//freeMemory();
// ...
	return 0;
}

* Splay Tree was checked for memory leaks with Valgrind-3.5.0

