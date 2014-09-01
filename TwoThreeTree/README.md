2-3 Tree 0.1

Description:
A 2-3 Tree implementation. 

A 2-3 Tree is a balanced search tree, not a binary tree. 

Each interior node has two or three childrens(2-Nodes , 3-Nodes respectively) while a leaf has not any. 2-Nodes have 2 childs and one data item, whereas 3-Nodes have 3 childs and two data items. The leaves may contain one or two data items. The first data field is greater than the largest item in its left subtree, but less than the smallest item in its center subtree. (If it is a 3-Node) The second data field is greater than the largest item in its center subtree a, but less than the smallest item in its right subtree.

To insert an item , find a leaf to put the item in and then split if necessary.

Splitting a Leaf -> Leaf has two data items(d1,d2). The new item is pointed to by d. We determine the middle item (m) and we move it up to parent p.
Also we seperate the small and large items into two seperate nodes. These nodes will be the 2 from the 3 childrens of parent p. Now parent p is full.
If the middle item is larger than the item located at p then we move it to the second data field else in the first one and the previous item to the second one. Update its childs in order to be a valid 2-3 tree node.  

Splitting an internal Node -> As above promote the middle value of the leaf node and split the leaf. Determine the new middle value from the two data fields of the internal node (parent) and the previous middle value , move it up to parent of parent p(grandfather of leaf node) and split the internal node (as above). Continue doing the same until the situation is resolved or the root node is reached.  

                  [80    |    100]       <-- root
                  /      |       \
                 /       |        \
            [40|50]   [82|90]     [110|200]
	    /   \      / |  \       
           /     \  [81][85][95]
        [30]  [60|65]         

           
Example:
#include "../src/twthrtree.h"

int main() {
// ...
	TTTree tree;
	initTTTree(&tree, destroy, compare);

	allocMem(SIZE,sizeof(TTTreeNode));
// ...
	ttTreeInsert(&tree,(void *)data0);
	ttTreeInsert(&tree,(void *)data1);
// ...
	ttTreeSearch(&tree, &d1);
// ...
	destroyTTree(&tree);
	freeMemory();
// ...
	return 0;
}

* 2-3 Tree was checked for memory leaks with Valgrind-3.5.0

