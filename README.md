# Concurrent-Binary-Tree
The code implements 4 functions insert,delete,search and print functions of the DS binary tree.
The Implementation was done in C language,using linked lists,queue and the "pthread.h" library for implementing the concurrent DS.

Complilation - gcc code_p.c -lpthread
Execution	- a.exe < test_case_filename.c (on windows)

Implementation-

1.Insertion in level order-
	It uses a helper function "tnode* create()" for creating a new node with the key and intialises the left and right pointer to NULL.
	The tree->head node points to the root node of the tree.
	If the tree is empty, the 1st node is made the root of the tree and for consecutive node insertion we use a queue."queue" is an array which holds
	the tree nodes.It has 2 pointers front and rear.Everytime a new node has to be inserted, we push the nodes starting from the root of the tree onto 
	the queue and pop them and store it a temporary node and push its children onto the queue while trying to find the last position in the tree where the new node is to be inserted.
	Each time we pop a node and perform a dequeue operation we increment the front pointer and every time the child nodes are pushed onto the queue, we 
	increment the rear pointer. When the position to insert the new node is found, we insert it as the child of the temporary node.

2.Deletion-
	It uses a helper function "delete_last" to delete the last node of the tree or the rightmost leaf node.
	If the tree has only one node, if the node's data is the key then we delete it and return an empty tree else we return just the root node of the tree.
	We traverse the tree in level order, to find the node to be deleted and store its value in a temporary variable called "dlt", and traverse till the 
	rightmost leaf in the tree.The "dlt" node value is replaced with the rightmost leaf node's value and the "delete_last" function is called with the root
	and the righmost leaf node.
	"delete_last" node uses a queue. The root is pushed onto the queue and its value is compared to the last_node value,if its not equal then we deque
	the node and push its children and perform the same until the rightmost leaf is found,we free the node and make the parent's pointer value NULL.

3.Search-
	To search if the given key is present, we use a queue.If the key is found, 1 is returned else 0 is returned.Each node is pushed onto the queue,
	checked if its data value is equal to the key if not,its dequed and its children are enqueued.While enqueing we increment rear pointer and while dequeueing
	we increment front pointer.This process is iteratively done until we reach the end of the tree that is when front and rear pointers have the same value.
	
4.Print-
	Uses a queue.The node is enqueued, its value is printed and its dequeued and its children are enqueued,this process is repeated until we have printed all nodes in the tree.


These 4 functions are run concurrently on various threads. Read-write locks were used.For every input read,a thread is forked and based on the choice,
4 helper functions p_insert,p_delete,p_search or p_print are called where the thread acquires the lock and calls the original functions and after the execution,
releases the lock.
Since insert and delete modify the tree, we use write locks to protect them and for search and print we use read locks.The concurrency is achieved by the simultaneous 
execution of insert or delete and search or print function.
