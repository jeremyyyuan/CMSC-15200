#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "memory.h"
#include "my_alloc.h"
#include "llist.h"

void display_addr_leading_8_bytes(void* addr){
	if(addr == NULL){
		printf("address is NULL.\n\n");
		return;
	}
	printf("Return address for user to use is %u.\n", (unsigned int)addr);
	void* sizeptr = retreat_in_memory(addr,8);
	unsigned int* sizeptr2 = (unsigned int*)sizeptr;
	printf("Size stored in the preceding 8 bytes of split memory is %u.\n", *sizeptr2);
}

int main(){

	printf("\n\n*******************************************\n\n");


	printf("The following test is for the newly written comparison functions.\n\n");

	printf("Display the tree in order of address:\n");
	bst* b = read_memory_blocks("testfile.txt", memory_addr_cmp);
	bst_inorder_traversal(b,memory_print);

	printf("Display the tree in order of size:\n");
	bst* b2 = read_memory_blocks("testfile.txt",memory_size_cmp);
	bst_inorder_traversal(b2,memory_print);
	printf("\n\n");

	printf("\n\n*******************************************\n\n");

	printf("The following tests are for memory_print.\n\n");
	memory* m = memory_new(NULL, 4882);
	memory_print(m);

	printf("Some memory was allocated for memory_print.\n");
	printf("The size of the memory was changed to 128.\n");
	void *v1 = malloc(136);
	memory* m_2 = memory_new(v1,128);
	memory_print(m_2);

	printf("\n\n*******************************************\n\n");

	printf("The following tests are for allocate_memory_page.\n\n");
	memory* m2 = allocate_memory_page();
	memory_print(m2);
	memory* m3 = allocate_memory_page();
	memory_print(m3);
	printf("\n\n");

	printf("\n\n*******************************************\n\n");

	printf("The following tests are for bst_iterate\n\n");

	printf("The original tree b is:\n");
	bst_inorder_traversal(b,memory_print);
	printf("\n");

	printf("The iterator test:\n");
	void *item;
	for(item=bst_iterate(b); item != NULL; item = bst_iterate(NULL)){
		printf("The iterated item that is returned is: ");
		memory_print(item);
	}

	printf("\n\nThe original tree b2 is:\n");
	bst_inorder_traversal(b2,memory_print);
	printf("\n");

	item = NULL;
	printf("The iterator test:\n");
	for(item=bst_iterate(b2); item != NULL; item = bst_iterate(NULL)){
		printf("The iterated item that is returned is: ");
		memory_print(item);
	}

printf("\n\n*******************************************\n\n");

	printf("\nThe following test is for free_llist\n");
	printf("list_is_empty - %d\n", list_is_empty(NULL));
	free_llist(NULL);
	printf("\n");
	llist* test_list = create_llist();
	printf("list_is_empty - %d\n",list_is_empty(test_list));
	free_llist(test_list);
	printf("\n");

	llist* test_list2 = create_llist();
	insert_tail(test_list2,m2);
	printf("list_is_empty - %d\n",list_is_empty(test_list2));
	free_llist(test_list2);

/*
	printf("\n\n Test for print_linked_list\n\n");
	print_linked_list(testlist);
	bst_inorder_traversal(testlist,add_to_llist);
	print_linked_list(b);
	*/
	/*
	printf("convert_to_llist(b)\n\n");
	convert_to_llist(b);
	printf("\n\n\n");
	printf("convert_to_llist(b2)\n\n");
	convert_to_llist(b2);

	printf("Inorder traversal of b2\n\n");
	bst_inorder_traversal(b2,memory_print);
	*/

	printf("\n\n*******************************************\n\n");

	bst* b3 = read_memory_blocks("testfile.txt",memory_size_cmp);

	printf("\n\n*******************************************\n\n");

	printf("\n\nThe following tests are for node_item_or_successor.\n\n");
	printf("Original BST:\n");
	bst_inorder_traversal(b3,memory_print);

	memory mem;
	mem.addr = NULL;
	mem.size = 200;
	void* val = bst_item_or_successor(b3,&mem);
	printf("\nThe requested size was 200 bytes. The found memory is:\n");
	memory_print(val);
	printf("\n");
	mem.size = 175;
	printf("\nThe requested size was 175 bytes. The found memory is:\n");
	val= bst_item_or_successor(b3,&mem);
	memory_print(val);

	mem.size = 1200;
	printf("\nThe requested size was 1200 bytes. There should be no found memory.\n");
	val = bst_item_or_successor(b3,&mem);
	memory_print(val);


	printf("\n\n*******************************************\n\n");

	// tests for split memory
	printf("\n\nThe following tests are for split_memory\n\n");
	memory* new_mem = allocate_memory_page();
	printf("Original memory:\n");
	memory_print(new_mem);
	printf("\n");


	void *mem_split = split_memory(new_mem, 128);
	printf("After Split memory: ");
	printf("Return address for user to use is %u.\n", (unsigned int)mem_split);
	void* sizeptr = retreat_in_memory(mem_split,8);
	unsigned int* sizeptr2 = (unsigned int*)sizeptr;
	printf("Size stored in the preceding 8 bytes of split memory is %u.\n", *sizeptr2);
	printf("Leftover memory: ");
	memory_print(new_mem);



	printf("\n\n*******************************************\n\n");

	printf("\nThe following tests are for my_malloc.\n\n");
	printf("Original Tree should be empty because avail_mem is NULL:\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 200 bytes of memory.\n");
	printf("Should create a new page of memory and split it.\n");
	//
	void* my_addr = my_malloc(200);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so only 3880 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 3881 bytes of memory.\n");
	printf("Should create a new page of memory and split it.\n");
	my_addr = my_malloc(3881);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so only 192 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 571 bytes of memory.\n");
	printf("Memory should be split from the 3880 block.\n");
	my_addr = my_malloc(571);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so 3296 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 2083 bytes of memory.\n");
	printf("Memory should be split from the 3296 block.\n");
	my_addr = my_malloc(2083);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so 1200 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 1500 bytes of memory.\n");
	printf("Memory should be created, and should be split\n");
	my_addr = my_malloc(1500);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so 2576 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 2689 bytes of memory.\n");
	printf("Memory should be created, and should split.\n");
	my_addr = my_malloc(2689);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out, so 1384 bytes should remain.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 5000 bytes of memory.\n");
	printf("Should return an error message and nothing in tree should change.\n");
	my_addr = my_malloc(5000);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("No memory has been changed, so tree should remain the same.\n");
	display_avail_mem();
	printf("\n");

	printf("my_malloc is called for 1150 bytes of memory.\n");
	printf("Memory should be taken from the 1200 block and given out.\n");
	my_addr = my_malloc(1150);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out. Block should be removed.\n");
	printf("Only the 1200 block should be gone, all other blocks should remain.\n");
	display_avail_mem();
	printf("\n\n");

	printf("my_malloc is called for 4000 bytes of memory.\n");
	printf("Memory should be taken from the 4088 block and given out.\n");
	my_addr = my_malloc(4000);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out. Block should be removed.\n");
	printf("Only the 4088 block should be gone, all other blocks should remain.\n");
	display_avail_mem();
	printf("\n\n");

	printf("my_malloc is called for 2850 bytes of memory.\n");
	printf("Memory should be split.\n");
	my_addr = my_malloc(2850);
	display_addr_leading_8_bytes(my_addr);

	printf("\nNew list:\n");
	printf("The new memory has been given out.\n");
	printf("New page allocated, and memory split.\n");
	display_avail_mem();
	printf("\n\n");

	printf("The following tests are for my_free.\n\n");
	printf("Original avail_mem tree: \n");
	display_avail_mem();
	printf("\n\n");

	printf("Should put a new page of 4088 bytes into the tree.\n");
	my_free(allocate_memory_page());
	printf("\nNew avail_mem:\n");
	display_avail_mem();
	printf("\n\n");

	printf("\n\n*******************************************\n\n");

	printf("The following tests are for merge_memory.\n\n");
	memory* memory3 = allocate_memory_page();
	memory* memory4 = allocate_memory_page();

	memory_print(memory3);
	memory_print(memory4);
	

	memory_print(merge_memory(memory3,memory4));

	printf("\n\n");

	void* split_addr = split_memory(memory3,128);
	memory* split_mem = memory_new(retreat_in_memory(split_addr,8),128);
	printf("Memory 3 after split:\n");
	memory_print(memory3);
	printf("\nsplit_mem:\n");
	memory_print(split_mem);

	printf("\n\nMerged memory:\n");
	memory_print(merge_memory(memory3,split_mem));

	printf("\n\n*******************************************\n\n");

	printf("\nThe following tests are for my_free.\n\n");
	memory* memory5 = allocate_memory_page();

	memory_print(memory5);
	printf("\n\n");

	void* split_addr2 = split_memory(memory3,128);
	display_addr_leading_8_bytes(split_addr2);

	printf("Avail_mem before my_free\n");
	display_avail_mem();

	my_free(split_addr2);

	printf("\nAvail_mem after my_free\n");
	display_avail_mem();

	printf("\n\n*******************************************\n\n");


	printf("\n\nThe following tests are for compact_memory.\n\n");
	printf("Here is the existing avail_mem:\n");
	display_avail_mem();

	printf("\n");

	printf("\nCompact Memory is called here.\n\n");
	compact_memory();
	
	printf("\nThe new tree after compact_memory.\n\n");
	printf("There should be nothing to compact after this call.\n");
	printf("Current avail_mem:\n");
	display_avail_mem();

	printf("\n\n");

	printf("Call my_malloc with 400 bytes requested here.\n\n");

	void* my_malloc_addr = my_malloc(400);
	printf("Size of the malloc'd chunk is: ");
	display_addr_leading_8_bytes(my_malloc_addr);
	printf("\n\nHere is the current avail_mem.\n\n");
	display_avail_mem();
	printf("\n\n");

	printf("Call my_free on my_malloc_addr\n");
	my_free(my_malloc_addr);
	printf("\n\nHere is the new avail_mem. 400 size chunk should be inserted.\n");
	display_avail_mem();
	printf("\n\ncompact_memory is called, should be compacted. 400 size chunk should be gone.\n\n");
	compact_memory();
	display_avail_mem();

	



}