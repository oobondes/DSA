#ifndef _TREE_H_
#define _TREE_H_

typedef struct tree_t tree_t;


/**
 * @brief sets the comparison function for the tree to sort with
 * 
 * @param comp_f comparison function
 */
void tree_register_compare(tree_t* tree,  int (*comp_f)(const void *, const void *));

/**
 * @brief sets the destroy funtion to use for freeing any elements in the tree.
 * 
 * @param dest_f free function
 */
void tree_register_destroy(tree_t* tree,  void (*dest_f)(void *));

/**
 * @brief initializes a tree_t struct for storage
 * 
 * @param comp_f a user provided comnparison function for the data to be passed to.
 * must return positive if first parameter is greater than left, 0 if they are equal
 * and negative otherwise
 * @param dest_f a function to free up the data stored in the tree_t. This can be null 
 * for stack allocated memory.
 * @return tree_t* a pointer to the newly created struct on success, NULL otherwise
 */
tree_t *tree_initialize(int (*comp_f)(const void *, const void *), void (*dest_f)(void *));

/**
 * @brief iterates through the tree and applies action_f to each data element
 * 
 * @param tree tree to iterate through
 * @param action_f function to pass each element to
 * @param reverse if 0, iterate from minimum element to maximum, otherwise iterate in reverse order.
 */
void tree_apply_to_all(tree_t* tree,void(*action_f)(void*), int reverse);

/**
 * @brief frees all data in a tree and sets the tree to NULL
 * 
 * @param tree tree to free
 */
void tree_destroy(tree_t** tree);

/**
 * @brief searches the tree for an equivalent element to the one pointed to by data and returns a pointer to it.
 * 
 * @param tree tree to look for the value
 * @param data a pointer to comparable data to be looked for
 * @return void* a pointer to the found data element on success, otherwise NULL
 */
void* tree_contains(tree_t* tree, void *data);

/**
 * @brief retrieves the minimum value stored in the tree.
 * 
 * @param tree tree to look for min value
 * @return void* returns a pointer to the data on succes, otherwise NULL
 */
void *tree_min(tree_t* tree);

/**
 * @brief retrieves the maximum value stored in the tree.
 * 
 * @param tree tree to look for max value
 * @return void* returns a pointer to the data on succes, otherwise NULL
 */
void *tree_max(tree_t* tree);

/**
 * @brief removes the specified element from the tree
 * 
 * @param tree tree to remove data from
 * @param data a pointer to the data to remove
 * @return int 1 if remove, 0 therwise
 */
int tree_remove(tree_t* tree, void *data); 

/**
 * @brief prints a visual representation of the tree to the terminal
 * right nodes (less than their parent) are printed above their parent
 * node, while left nodes (greater than their parent) are printed below
 * @param tree tree to print
 * @param print_f 
 */
void tree_print(tree_t* tree, void(*print_f)(const void*));

/**
 * @brief gets the total number of elements in the tree_t
 * 
 * @param tree tree_t to find the size of
 * @return int number of elements in the tree_t
 */
int tree_get_size(tree_t* tree);

/**
 * @brief adds an element to the tree_t
 * while maintaining the trees balance
 * 
 * @param tree tree_t to add an element to
 * @param data a pointer to the data to add
 * @return void* returns data on success, otherwise NULL
 */
void* avl_add(tree_t *tree, void* data);

#endif
