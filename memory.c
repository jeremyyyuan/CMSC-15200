#include<stdio.h>
#include<stdlib.h>
#include "memory.h"
#include "bst.h"
#include "llist.h"
#include "string.h"


/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void *addr, unsigned int size){
  memory* m = (memory*)malloc(sizeof(memory));
  m->addr = addr;
  m->size = size;
  if(m->addr != NULL){
  	// set the leading 8 bytes to the size
	void* sizeptr = addr;
	unsigned int* sizeptr2 = (unsigned int*) sizeptr;
	*sizeptr2 = size;
  }
  //printf("memory_new - Address: %u, size: %u\n",(unsigned int)m->addr,m->size);
  return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
  memory* m = (memory*)p;
  free(m);
}

/* compare two memory variables x and y by address 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y){
  memory* x2 = (memory*)x;
  memory* y2 = (memory*)y;
  //printf("compare address\n");
  //printf("memory_addr_cmp - X address: %u   Y address: %u\n", (unsigned int)x2->addr,(unsigned int)y2->addr);

  if(x2->addr > y2->addr){
    return 1;
  }
  else if(x2->addr < y2->addr){
    return -1;
  }
  else{
    return 0;
  }
}

/* compare two memory variables x and y by size 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y){
  memory* x2 = (memory*)x;
  memory* y2 = (memory*)y;

  if(x2->size > y2->size){
    return 1;
  }
  else if(x2->size < y2->size){
    return -1;
  }
  else{
    return 0;
  }
}

/* print the memory address and size
 */
void memory_print(void* data){
 memory* m = (memory*)data;
	if(m == NULL){
		printf("Data is NULL.\n");
	}
	else{
		if(m->addr != NULL){
			void* sizeptr = m->addr;
			unsigned int* sizeptr2 = (unsigned int*) sizeptr;
			printf("memory_print - Address = %u, Size = %d, Leading 8 bytes = %u\n",
			(unsigned int)m->addr,m->size,*sizeptr2);
		}
		else{
			printf("memory_print - Address = %u, Size = %d, Leading 8 bytes is not defined due to address is NULL.\n",
			(unsigned int)m->addr,m->size);
		}
	}
}

/* allocate_memory_page
 * allocates a single page (4096 bytes) and creates the memory structure
 * that points to it.
 * inputs:
 *		none
 * output:
 *		*memory - returns a memory structure that points to the new memory
 */
memory *allocate_memory_page(){
	void *m = malloc(4096);
	memory* ret_val = memory_new(m,4088);
	return ret_val;
}

/* advance_in_memory
 * function to advance the pointer in split_memory
 */
void* advance_in_memory(void *v, unsigned int distance){
	char *pc = (char*)v;
	pc += distance;
	return (void*)pc;
}

void* retreat_in_memory(void *v, unsigned int distance){
	char *pc = (char*)v;
	pc -= distance;
	return (void*)pc;
}

/* split_memory
 *
 * Given a memory struct and a desired size of memory,
 * perform the operations necessary to remove the desired
 * size of memory from the end of the chunk and record
 * the new information for the smaller chunk. Return a 
 * pointer to the beginning of the chunk you are handing out.
 */
void *split_memory(memory* data, unsigned int size_desired){
	void* ret_addr = data->addr;
	//unsigned int ret_size = size_desired;

	if(data->size > size_desired){
		data->size= data->size - size_desired - 8;
		// update the leading 8 bytes of the original memory chunk
		void* original_sizeptr = data->addr;
		unsigned int* original_sizeptr2 = (unsigned int*) original_sizeptr;
		*original_sizeptr2 = (unsigned int)data->size;

		unsigned int adv = data->size + 16;
		ret_addr = advance_in_memory(data->addr,adv);
	}
	else{
		fprintf(stderr,"Not enough memory to split.\n");
		return NULL;
	}

	void* sizeptr = retreat_in_memory(ret_addr,8);
	unsigned int* sizeptr2 = (unsigned int*) sizeptr;
	*sizeptr2 = (unsigned int)size_desired;

	return ret_addr;
}


/* display_data
 * takes a *data, but casts it into a memory and displays it.
 * inputs:
 * 		void* data - the data that is read in
 * outputs:
 *		none - just printing to the screen
 *
void display_data(void *data){
	memory* m = (memory*)data;
	if(m == NULL){
		printf("Data is not found.\n");
	}
	else{
		printf("display_data - Address = %u, Size = %d\n",(unsigned int)m->addr,m->size);
	}
}
*/

/* merge_memory
 *
 * Given two memory structs, check to see if the two can be 
 * merged. They can be merged if the two are next to each other 
 * in memory with no break in between. If they can be merged,
 * return a memory struct pointer to a struct containing the information
 * for a single memory chunk containing the old two chunks.
 * If they cannot be merged (there is space between them), then
 * return NULL; 
 *
 * Make sure that you free any memory structs that you need to.
 */
memory *merge_memory(memory *first, memory *second){
	if(first == NULL || second == NULL){
		return NULL;
	}

	memory* new_memory = NULL;
	// if addresses line up
	// check with teacher about correct structure
	if(first->addr + first->size + 8 == second->addr){
		new_memory = memory_new(first->addr,first->size + second->size + 8);
	}
	return new_memory;

}