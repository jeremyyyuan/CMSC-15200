#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"
#include "my_alloc.h"

/* Global variables
 * By declaring these outside of a function:
 *   Scope: Every function inside of this file may use them. 
 *   		No functions in other files may use them.
 *   Lifetime: They exist for the entire program - they are created
 *		when the program begins and exist until the program
 *		ends.
 */   		

/* This is the tree that holds the available memory. */
bst *avail_mem = NULL;

/* This includes all of the functions for the memory allocator.
 * The last two functions (my_malloc and my_free) are public 
 * functions with prototypes in the header function. The rest of the
 * functions are helper functions only called by my_malloc and 
 * my_free. You must implement these helper functions. If you wish,
 * you may add helper functions we did not define.
 */


/* compact_memory
 *
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory()
{

	bst* addr_sorted_tree = bst_new(memory_addr_cmp);

	//printf("Before calling compact_memory.\n");
	//bst_inorder_traversal(addr_sorted_tree,memory_print);

	// make the addr sorted tree
	bst_inorder_traversal_two_args(avail_mem,addr_sorted_tree, add_to_sorted_tree);

	//printf("After calling add_to_sorted_tree.\n");
	//bst_inorder_traversal(addr_sorted_tree,memory_print);

	llist* list = create_llist();
	bst_inorder_traversal_two_args(addr_sorted_tree,list,add_to_llist_two_args);

	// print the linked list
	//printf("After creating the linked list.\n\n");
	//print_linked_list(list);
	//printf("\n\n");

	bst* new_avail_mem = bst_new(memory_size_cmp);

	memory* merged_item = NULL;
	void *item;
	for(item = iterate(list); item != NULL; item = iterate(NULL)){
		memory* newly_merged_item = merge_memory(merged_item,item);
		if(newly_merged_item == NULL){
			//printf("newly_merged_item is NULL\n");
			if(merged_item != NULL){
				//printf("merged_item is not NULL.\n");
				bst_insert(new_avail_mem,merged_item);
			}
			
			merged_item = memory_new(((memory*)item)->addr,((memory*)item)->size);

		}
		else{
			//printf("newly_merged_item is not NULL\n");
			memory* item_to_free = merged_item;
			merged_item = newly_merged_item;
			if(item_to_free != NULL){
				//printf("item_to_free is not NULL");
				memory_free(item_to_free);
			}
			
		}
	}
	// handling the last item in the linked list
	if(merged_item != NULL){
		bst_insert(new_avail_mem,merged_item);
	}

	//printf("Printing the new_avail_mem.\n\n\n");
	//bst_inorder_traversal(new_avail_mem,memory_print);
	//printf("\n\n");

	bst* avail_mem_to_free = avail_mem;
	avail_mem = new_avail_mem;
	bst_free(avail_mem_to_free); 

	//printf("compact_memory - Printing the avail_mem\n\n");
	//bst_inorder_traversal(avail_mem,memory_print);


	free_llist(list);
	bst_free(addr_sorted_tree);

}

void add_to_sorted_tree(void* input_tree, void* data){
	//printf("add to sorted tree\n");
	bst* tree = (bst*)input_tree;

	memory* input_data = (memory*)data;
	memory* new_data = memory_new(input_data->addr,input_data->size);
	bst_insert(tree,new_data);
	//printf("after insert\n");
}

/* print_available_memory 
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory()
{
	bst_inorder_traversal(avail_mem,memory_print);
}

/* init_alloc
 *
 * Initializes the data structures. This initializes avail_mem so 
 * that, instead of being a NULL pointer, it points to a valid bst
 * struct whose root pointer is NULL.
 */
void init_alloc()
{
	bst* tree = bst_new(memory_size_cmp);
	avail_mem = tree;
}

/* node_item_or_successor
 * searches node for memory that comes in, determines if should go left
 * or right, then keeps going until finding a suitable piece of memory.
 */
void* node_item_or_successor(node *n, void *item,
        int (*cmp)(const void* x, const void* y)){
	/*
	memory *mem = (memory*) item;
	node *tmp = n;
	while (tmp->right != NULL){
		if((*cmp)(mem,n->data) == 1){
			tmp = tmp->right;
		}
		else {
			return tmp->data;
		}
	}
	return NULL;
*/

	if(n == NULL){
    return NULL;
	}
	int cmp_result = (*cmp)(n->data,item);
	if(cmp_result == 0){
	  return n->data;
	}
	// found a bigger piece of memory
	else if(cmp_result == 1){
		if(n->left == NULL){
			return n->data;
		}
		else {
			void* max_left_subtree = node_max(n->left);
			if((*cmp)(max_left_subtree,item) == -1){
			return n->data;
			}
			else{
				return node_item_or_successor(n->left,item,cmp);
			}
		}
	}
	// not bigger yet
	else{
	  return node_item_or_successor(n->right,item,cmp);
	}
}

/* bst_item_or_successor
 * same function as node_item_or_successor, just wrapped into the BST struct
 */
void* bst_item_or_successor(bst *b, void *item){
	return node_item_or_successor(b->root, item, b->cmp);
}

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes){
	// if avail_mem is NULL, then create the tree, but only once
	if(avail_mem == NULL){
		init_alloc();
	}

	int rounded_num_bytes;
	// if not a multiple of 8
	if(num_bytes%8 != 0){
		rounded_num_bytes = num_bytes + (8-(num_bytes%8));
	}
	else{
		rounded_num_bytes = num_bytes;
	}

	//printf("rounded_num_bytes - %d\n",rounded_num_bytes);
	// create a memory struct to wrap the information in
	memory m;
	m.addr = NULL;
	m.size = rounded_num_bytes;
	// find the correct chunk of memory
	void* item = bst_item_or_successor(avail_mem,&m);
	
	memory_print(item);

	// if that memory doesn't exist in the tree, create it
	if(item == NULL){
		void* new_page = allocate_memory_page();
		void *split_addr = split_memory(new_page,rounded_num_bytes);
		// if not possible to split, then print error and return NULL.
		if(split_addr == NULL){
			printf("Requested memory size is too big.\n");
			return NULL;
		}
		// otherwise, split the memory and put the leftover back into tree
		bst_insert(avail_mem,new_page);
	
		return split_addr;
	}
	// if appropriate memory is found
	else{
		unsigned int item_mem_size = ((memory*)item)->size;
		// appropriate memory is over double the requested, split
		if(item_mem_size >= rounded_num_bytes*2){
			memory* new_mem_to_be_inserted = memory_new(((memory*)item)->addr,((memory*)item)->size);
			void *split_addr = split_memory(new_mem_to_be_inserted,rounded_num_bytes);
			// following delete and insert is for the
			// case that leftover from split is smaller than the 
			// previous memory size
			// but maybe redundant because the leftover might always
			// be bigger than the previous memory size
			bst_delete(avail_mem,item);
			bst_insert(avail_mem,new_mem_to_be_inserted);
			return split_addr;
		}
		// otherwise, return that chunk and remove it from the BST
		else{
			bst_delete(avail_mem,item);
			return advance_in_memory(((memory*)item)->addr,8);
		}
	}
}

/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address)
{
	if(address == NULL){
		fprintf(stderr,"address to free is NULL\n");
		return;
	}
	unsigned int* sizeptr = address - 8;
	printf("Size = %u\n",*sizeptr);
	memory* new_memory = memory_new(address-8,*sizeptr);
	bst_insert(avail_mem,new_memory);
}

void display_avail_mem(){
	print_memory();
}
