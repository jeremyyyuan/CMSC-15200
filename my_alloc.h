#ifndef MY_ALLOC_H
#define MY_ALLOC_H
#include <stdio.h>
#include <stdlib.h>


/* print_available_memory 
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory();

/* init_alloc
 *
 * Initializes the data structures of the allocator.
 * This must be called prior to any my_malloc and my_free calls.
 */
void init_alloc();

/* node_item_or_successor
 * searches node for memory that comes in, determines if should go left
 * or right, then keeps going until finding a suitable piece of memory.
 */
void* node_item_or_successor(node *n, void *item,
        int (*cmp)(const void* x, const void* y));

/* bst_item_or_successor
 * same function as node_item_or_successor, just wrapped into the BST struct
 */
void* bst_item_or_successor(bst *b, void *item);

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes);


/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address);

void display_avail_mem();

/* compact_memory
 *
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory();

void add_to_sorted_tree(void* input_tree, void* data);


#endif
