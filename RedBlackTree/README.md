Red Black Tree 0.1

Description:
A Red Black Tree implementation. 

A Red Black Tree is a type of self-balancing binary search tree.

Each node has a color attribute (RED or BLACK). It has up to 2 children and only one parent (as every node of a binary tree). 
Every Red Black Tree must meet some requirements:
   - A node is either black or red
   - All leaves are black
   - The root is black
   - Both children of every red node are black
   - Every simple path from a given node to any of its descendant leaves contains the same number of black nodes



                          [21,Black]
                           /       \ 
                          /         \
                      [12,Red]      [38,Black]
                      /    \          /      \
                     /      \        /        \
             [3,Black]  [19,Black]  [30,Red]  [41,Red]
                  \         /         / \       / \
                   \       /      [nil,Black] [nil,Black]
               [7,Red]   [14,Red]
                / \        / \
            [nil,Black] [nil,Black]      

           
Example:
#include "../src/rbtree.h"

int main() {
// ...
	RBTree tree;
	
	initRBTree(&tree, destroy);
	
	//allocMem(BLOCKS, sizeof(RBNode));
// ...
	rbTreeInsert(&tree, key0, (void *)data0);
	rbTreeInsert(&tree, key1, (void *)data1);
// ...
	rbTreeRemove(&tree, key);
// ...
	rbTreeDestroy(&tree);
	//freeMemory();
// ...
	return 0;
}

* Red Black Tree was checked for memory leaks with Valgrind-3.5.0

