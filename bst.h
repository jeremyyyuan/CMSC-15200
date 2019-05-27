#ifndef BST_H
#define BST_H

#include "llist.h"
#include "memory.h"

typedef struct _node{
  void* data;
  struct _node* left;
  struct _node* right;
}node;

typedef struct{
  node* root;
  int (*cmp)(const void* x, const void* y);
}bst;


/* ******* BST ******** */
/* These functions are what programmers using the BST would use */

/* create a new bst, initialize its root to be NULL
*/
bst* bst_new(int (*cmp)(const void* x, const void* y));

/* Insert a new node to the bst
*/
void bst_insert(bst* b, void* data);

/* Search a node with data in a bst. 
*/
void* bst_search(bst* b, void* data);

/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data));

/* free the bst with 
*/
void bst_free(bst* b);

/* delete a node containing data from the bst
*/
void bst_delete(bst* b, void* data);


/* ******* Node ****** */
/* These functions are only in the .h file so you can 
 * call them for testing purposes. Normally,they would
 * not be in the .h because they aren't intended to be 
 * called from outside bst.c
 */


/* malloc a new node and assign the data
 * pointer to its data field
 */
node* node_new(void* data);

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data, 
    int (*cmp)(const void* x, const void* y));

/* node_min
 * find the leftmost value from a given node using the cmp
 */
node* node_min(node* root);

/* node_max
 * find the rightmost value from a given node using the cmp
 */
node* node_max(node* root);

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data, 
    int (*cmp)(const void* x, const void* y));

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data, 
    int (*cmp)(const void* x, const void* y));

void inorder_traversal_two_args(node* root, void* new_struct, void(*func)(void* data_struct,void* data));

void bst_inorder_traversal_two_args(bst* b, void* new_struct, void(*func)(void* data_struct, void* data));

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data));

void add_to_llist(void* data);

void add_to_llist_two_args(void* input_list, void* data);

/* bst_iterate
 * accesses all the items in the binary search tree in an in-order traversal
 */
void* bst_iterate(bst* tree);

void print_linked_list(llist *list);

/* display_file
 * reads in a file and displays the contents in a readable way.
 * inputs:
 * 		char *filename - the name of the file
 * outputs:
 *		none - just printing to the screen
 */
void display_file(char *filename);

/* read_memory_blocks
 * reads in a file and breaks it down into a bst struct
 * inputs:
 * 		char *filename - the name of the file
 *		int (*cmp) - comparison function for the struct
 * outputs:
 *		bst* - returns a pointer to the bst struct that is created
 */
bst* read_memory_blocks(char *filename, 
  int (*cmp)(const void* x, const void* y));

void convert_to_llist(bst* tree);

#endif