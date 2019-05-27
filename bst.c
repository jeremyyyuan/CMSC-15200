#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "llist.h"

#define BUFSIZE 120

/* malloc a new node and assign the data
 * pointer to its data field
 */
node* node_new(void* data){
  node* n = (node*)malloc(sizeof(node));
  n->data = data;
  n->left = NULL;
  n->right = NULL;
  return n;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y)){
  bst* tree = (bst*)malloc(sizeof(bst));
  tree->root = NULL;
  tree->cmp = cmp;
  return tree;
}

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  //printf("Node_insert - start\n");
  node* n = node_new(data);
  if(root == NULL){
    //printf("Root = NULL\n");
    return n;
  }
  else{
    //printf("node_insert - root is not equal to NULL.\n");
    if((*cmp)(root->data, data) == 1){ 
      root->left = node_insert(root->left,data,cmp);
    }
    else{
      root->right = node_insert(root->right,data,cmp);
    }
    return root;
  }
}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data){
  //printf("bst_insert - run\n");
  b->root = node_insert(b->root,data,b->cmp);
}

/* node_min
 * find the leftmost value from a given node using the cmp
 */
node* node_min(node* root){
  if(root->left == NULL){
    return root;
  }
  else{
    return node_min(root->left);
  }
}

/* node_max
 * find the rightmost value from a given node using the cmp
 */
node* node_max(node* root){
  if(root->right == NULL){
    return root;
  }
  else{
    return node_max(root->right);
  }
}

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  // if root is NULL, then no nodes can be deleted
  if(root == NULL){
    //printf("Root is NULL.\n");
    return NULL;
  }
  // if the correct root is found
  else if((*cmp)(root->data,data) == 0){
    // if the root is a leaf
    if(root->left == NULL && root->right == NULL){
      free(root);
      return NULL;
    }
    // if the root only has a right child
    else if(root->left == NULL){
      node* tmp = root->right;
      free(root);
      return tmp;
    }
    // if the root only has a left child
    else if (root->right == NULL){
      node *tmp = root->left;
      free(root);
      return tmp;
    }
    // if the node has two children
    else {
      // find the leftmost node in the right side of the root
      node* leftmost = node_min(root->right);
      void* orig_root_data = root->data; 
      // copy the data over
      root->data = leftmost->data;
      free(orig_root_data);
      // delete the node that was just copied over
      root->right = node_delete(root->right,leftmost->data,cmp);
      return root;
    }
  }
  else if((*cmp)(root->data,data) == 1){
    root->left = node_delete(root->left,data,cmp);
    return root;
  }
  else{
    root->right = node_delete(root->right,data,cmp);
    return root;
  }
}

/* delete a node containing data from the bst
 */
void bst_delete(bst* b, void* data){
  b->root = node_delete(b->root,data,b->cmp);
  return;
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  if(root == NULL){
    return NULL;
  }
  else if((*cmp)(root->data,data) == 0){
    return root->data;
  }
  else if((*cmp)(root->data,data) == 1){
    return node_search(root->left,data,cmp);
  }
  else{
    return node_search(root->right,data,cmp);
  }
}

/* Search a node with data in a bst. 
 */
void* bst_search(bst* b, void* data){
  return node_search(b->root,data,b->cmp);
}

void inorder_traversal_two_args(node* root, void* new_struct, void(*func)(void* data_struct,void* data)){
  //printf("Called inorder_traversal\n");
  if(root == NULL){
    //printf("NULL\n");
    return;
  }
  else{
    //printf("recursive\n");
    inorder_traversal_two_args(root->left,new_struct,func);
    (*func)(new_struct,root->data);
    //printf("root traversed\n");
    inorder_traversal_two_args(root->right,new_struct,func);
  }
}

void bst_inorder_traversal_two_args(bst* b, void* new_struct, void(*func)(void* data_struct, void* data)){
  //printf("Called bst_inorder_traversal\n");
  if(b == NULL){
    return;
  }
  inorder_traversal_two_args(b->root,new_struct,func);
}

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data)){
  //printf("Called inorder_traversal\n");
  if(root == NULL){
    //printf("NULL\n");
    return;
  }
  else{
    //printf("recursive\n");
    inorder_traversal(root->left,func);
    (*func)(root->data);
    //printf("root traversed\n");
    inorder_traversal(root->right,func);
  }
}

/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data)){
  //printf("Called bst_inorder_traversal\n");
  if(b == NULL){
    return;
  }
  inorder_traversal(b->root,func);
}


/* free the node
 */
void node_free(node* root){
  if(root->left == NULL && root->right == NULL){
    free(root->data);
    free(root);
  }
  else
    if(root->left != NULL) {
      node_free(root->left);
    }
    if(root->right != NULL){
      node_free(root->right);
    }
}

/* free the bst 
 */
void bst_free(bst* b){
  if(b->root != NULL){
    node_free(b->root);
  }
  free(b);
}

static llist* bst_inorder_list = NULL;

void add_to_llist(void* data){

  if(bst_inorder_list == NULL){
    bst_inorder_list = create_llist();
    //printf("add_to_llist - new linked list created\n");
  } 
  /*
  llist_node* n = malloc(sizeof(llist_node));
  n->item = data;
  bst_inorder_list->tail->next = n;
  bst_inorder_list->tail = n;
  if(bst_inorder_list->head == NULL){
    bst_inorder_list->head = bst_inorder_list->tail;
  }
  */
  insert_tail(bst_inorder_list,data);
}

void add_to_llist_two_args(void* input_list, void* data){
  llist* list = (llist*)input_list;
  memory* mem = memory_new(((memory*)data)->addr,((memory*)data)->size);
  insert_tail(list,mem);
}

// This function was just for testing purposes
void convert_to_llist(bst* tree){

  bst_inorder_list = NULL;

  print_linked_list(bst_inorder_list);

  bst_inorder_traversal(tree,add_to_llist);

  print_linked_list(bst_inorder_list);

}

void print_linked_list(llist *list){
  if(list == NULL){
    printf("print_linked_list - list is null.\n");
    return;
  }
  if(list->head == NULL){
    printf("print_linked_list - head is null.\n");
    return;
  }
  llist_node *cursor = list->head;

  while(cursor != NULL){
    memory_print(cursor->item);
    cursor = cursor->next;
  }
  
}

/* bst_iterate
 * accesses all the items in the binary search tree in an in-order traversal
 */
void* bst_iterate(bst* tree){
 
  static int traversed = 0;
  //printf("bst_iterate - traversed = %d\n", traversed);

  //print_linked_list(bst_inorder_list);

  if(traversed == 0){
    bst_inorder_traversal(tree,add_to_llist);
    traversed = 1;
    //printf("bst_iterate - traversed = %d\n", traversed);
  }

  //printf("bst_iterate - traversed = %d\n", traversed);
  //print_linked_list(bst_inorder_list);

  void* ret_val = NULL;

  if(tree == NULL){
    ret_val = iterate(NULL);
  }
  else{
    ret_val = iterate(bst_inorder_list);
  }
  // iterator has reached the end
  if(ret_val == NULL){
    traversed = 0;
    // free the static variable
    free_llist(bst_inorder_list);
    // set to NULL
    bst_inorder_list = NULL;
  }
  return ret_val;
}


/* display_file
 * reads in a file and displays the contents in a readable way.
 * inputs:
 *    char *filename - the name of the file
 * outputs:
 *    none - just printing to the screen
 */
void display_file(char *filename)
{
  FILE *fp;
  char buf[BUFSIZE] = "Garbage";
  int i;

  // attempt to open the file, then check whether that worked.
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr,"Could not open file %s\n",filename);
    //return (1);
  }

  // for each line of the file, read it in and then print it out
  i = 0;
  while (fgets(buf, sizeof(buf), fp) != NULL)
  {
    printf ("Line %4d: %s", i, buf);
    i++;
  }

  fclose(fp);
}

/* read_memory_blocks
 * reads in a file and breaks it down into a bst struct
 * inputs:
 *    char *filename - the name of the file
 *    int (*cmp) - comparison function for the struct
 * outputs:
 *    bst* - returns a pointer to the bst struct that is created
 */
bst* read_memory_blocks(char *filename, 
  int (*cmp)(const void* x, const void* y)){

  FILE *fp;
  char buf[BUFSIZE] = "Garbage";
  int i;
  unsigned int size;
  memory *m;
  bst *tree;

  // attempt to open the file, then check whether that worked.
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr,"Could not open file %s\n",filename);
    //return (1);
  }

  tree = bst_new(cmp);

  // for each line of the file, read it in and then print it out
  i = 0;
  while (fgets(buf, sizeof(buf), fp) != NULL)
  {
    printf ("read_memory_blocks - Line %4d: %s", i, buf);
    i++;
  // initialize the string tokenizer and receive pointer to first token
  //printf("%s\n",tokenPtr);
  size = atoi(buf);

  void *v = malloc(size);
  m = memory_new(v,size);
  bst_insert(tree,m);
  }

  fclose(fp);
  return tree;
}

