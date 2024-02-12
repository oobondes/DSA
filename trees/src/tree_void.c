#include "tree_void.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ANSI_BRIGHT_RED     "\x1b[91m"
#define ANSI_BRIGHT_GREEN   "\x1b[92m"
#define ANSI_BRIGHT_YELLOW  "\x1b[93m"
#define ANSI_BRIGHT_BLUE    "\x1b[94m"
#define ANSI_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_BRIGHT_CYAN    "\x1b[96m"
#define ANSI_BRIGHT_BLACK  "\x1b[90m"
#define ANSI_BRIGHT_WHITE   "\x1b[97m"

//for printing tree and color coding the layers
static char *colors[] =
    { ANSI_BRIGHT_WHITE, ANSI_BRIGHT_MAGENTA, ANSI_BRIGHT_RED,
ANSI_BRIGHT_YELLOW, ANSI_BRIGHT_GREEN, ANSI_BRIGHT_CYAN, ANSI_BRIGHT_BLUE,
ANSI_BRIGHT_BLACK };

//compare function to be registered
typedef int (*comp_f)(const void *, const void *);
//destroy function to be registered
typedef void (*dest_f)(void *);

typedef struct node_t {
	struct node_t *left;
	struct node_t *right;
	short height;
	void *data;
} node_t;

struct tree_t {
	node_t *root;
	int size;
	comp_f compare_f;
	dest_f destroy_f;
};

/**
 * @brief Get the distance a given node is from the bottom of the tree
 * 
 * @param node node_t to check the height of
 * @return int height of the node. (0 if node is NULL)
 */
static int get_height(node_t * node)
{
	return NULL == node ? 0 : node->height;
}

/**
 * @brief frees a node and the data it is holding
 * 
 * @param node a node to free
 * @param destroy_f a freeing function provided by the caller
 * so their stored data can be properly freed.
 */
static void node_destroy(node_t * node, dest_f destroy_f)
{
	if (NULL == node) {
		return;
	}
	node_destroy(node->right, destroy_f);
	node_destroy(node->left, destroy_f);
	if (NULL != destroy_f) {
		destroy_f(node->data);
	}
	free(node);
}

/**
 * @brief peforms a right rotation on the node and returns
 * a pointer to the node that took its place in the tree_t
 * 
 * @param node a node to rotate
 * @return void* the address of the node_t that now sits
 * where the original node_t was.
 */
static node_t *right_rotation(node_t * node)
{
	if (NULL == node) {
		return NULL;
	}
	//rotate nodes
	node_t *temp = node->left;
	node->left = node->left->right;
	temp->right = node;
	//adjust heights after rotation
	node->height--;
	temp->height++;
	return temp;
}

/**
 * @brief peforms a left rotation on the node and returns
 * a pointer to the node that took its place in the tree_t
 * 
 * @param node a node to rotate
 * @return void* the address of the node_t that now sits
 * where the original node_t was.
 */
static node_t *left_rotation(node_t * node)
{
	if (NULL == node) {
		return NULL;
	}
	//rotate nodes
	node_t *temp = node->right;
	node->right = node->right->left;
	temp->left = node;
	//adjust heights after rotation
	node->height--;
	temp->height++;
	return temp;
}

/**
 * @brief Get the balance factor for a given node
 * 
 * @param root the node to find the balance factor for.
 * @return int the difference in height between the
 * left node and right node of root.
 */
static int get_balance_factor(node_t * root)
{
	if (NULL == root) {
		return 0;
	}
	return get_height(root->left) - get_height(root->right);
}

static node_t *node_delete(node_t * node, dest_f destroy_f)
{
	//if node is null
	if (NULL == node) {
		return NULL;
	}
	//if the node has no kids
	if (NULL == node->left && NULL == node->right) {
		node_destroy(node, destroy_f);
		node = NULL;
		goto EXIT;
		//if the node only has a right child
	} else if (NULL == node->left) {
		node_t *temp = node->right;
		if (NULL != destroy_f) {
			destroy_f(node->data);
		}
		free(node);
		node = temp;
		goto EXIT;
		//if the node only has a left child
	} else if (NULL == node->right) {
		node_t *temp = node->left;
		if (NULL != destroy_f) {
			destroy_f(node->data);
		}
		free(node);
		node = temp;
		goto EXIT;
		//if the node has left and right children
	} else {
		//if the left child has not right child
		if (NULL == node->left->right) {
			node_t *temp = node->left;
			if (NULL != destroy_f) {
				destroy_f(node->data);
			}
			node->data = node->left->data;
			node->left = node->left->left;
			free(temp);
			node = node;
			goto EXIT;
		}
		//if the left child has a right child
		node_t *temp = node->left;
		while (NULL != temp->right->right) {
			temp = temp->right;
		}
		if (NULL != destroy_f) {
			destroy_f(node->data);
		}
		node->data = temp->right->data;
		node_t *to_free = temp->right;
		temp->right = temp->right->left;
		free(to_free);
		node = node;
	}
 EXIT:

	return node;
}

/**
 * @brief recursively finds a node holding the specified data and
 * deletes the node if found. As the call stack returns, the balance
 * factor of every node the function was called on the check if
 * balancing needs to be done. The balancing is then done before
 * returning to the next level of the call stack.
 * 
 * @param node The current node to check if it needs to be deleted.
 * @param data the data being deleted from the tree_t
 * @param success pointer of where to store whether the data was deleted or not
 * (1 for success, 0 for failure)
 * @param compare_f user provided function to compare data and search for the 
 * right element to delete
 * @param destroy_f user provided function to free up the removed data element if found.
 * @return node_t* If the data was found in the tree, a pointer to the node that took
 * this nodes spot after deletion and restructuring of the tree is returned, otherwise
 * the node itself is returned.
 * If the node provided was NULL, then NULL is returned.
 */
static node_t *node_remove(node_t * node, void *data, int *success,
			   comp_f compare_f, dest_f destroy_f)
{
	if (NULL == node) {
		return NULL;
	}
	//find and delete
	int comp = compare_f(node->data, data);
	if (comp < 0) {
		node->right =
		    node_remove(node->right, data, success, compare_f,
				destroy_f);
		node->height =
		    node->right == NULL ? 0 : get_height(node->right) + 1;
	} else if (comp > 0) {
		node->left =
		    node_remove(node->left, data, success, compare_f,
				destroy_f);
		node->height =
		    node->left == NULL ? 0 : get_height(node->left) + 1;
	} else {
		*success = 1;
		return node_delete(node, destroy_f);
	}
	//restructure tree if needed
	switch (get_balance_factor(node)) {
	case -2:
		if (get_balance_factor(node->right) > 0) {
			// puts("RIGHT LEFT");
			node->right = right_rotation(node->right);
		}
		// puts("RIGHT RIGHT");
		node = left_rotation(node);
		break;
	case 2:
		if (get_balance_factor(node->left) < 0) {
			// puts("LEFT RIGHT");
			node->left = left_rotation(node->left);
		}
		// puts("LEFT LEFT");
		node = right_rotation(node);
		break;
	default:
		printf("bf:%d\n", get_balance_factor(node));
		break;
	}
	return node;
}

static void node_print(node_t * node, int stop, void (*print_f)(const void *))
{
	if(NULL == node) {
		return;
	}
	//recursively call right
	node_print(node->right, stop + 1, print_f);

	//print bars for lines that precede this node
	//switch color for every layer
	int i;
	for (i = 1; i < stop; ++i) {
		printf("%s%s  ", colors[i % 8], "║");
	}
	//print corner if not root
	if (stop) {
		printf("%s╠══", colors[i % 8]);
	}
	//print data and then reset color
	print_f(node->data);
	printf(" (%d)", node->height);
	puts("\x1b[0m");

	//recursively call left
	node_print(node->left, stop + 1, print_f);
}

static void node_apply_to_all(node_t * node, void (*action_f)(void *),
			      int reverse)
{
	if(NULL == node) {
		return;
	}
	if (reverse) {
		node_apply_to_all(node->right, action_f, reverse);
		action_f(node->data);
		node_apply_to_all(node->left, action_f, reverse);
	} else {
		node_apply_to_all(node->left, action_f, reverse);
		action_f(node->data);
		node_apply_to_all(node->right, action_f, reverse);
	}
}

static void *node_add(node_t * root, node_t * node, comp_f compare_f)
{
	if (NULL == node) {
		return NULL;
	}
	if (NULL == root) {
		return node;
	}
	int comp = compare_f(root->data, node->data);
	if (comp > 0) {
		root->left = node_add(root->left, node, compare_f);
	} else {
		root->right = node_add(root->right, node, compare_f);
	}

	switch (get_balance_factor(root)) {
	case -2:
		if (get_balance_factor(root->right) > 0) {
			// puts("RIGHT LEFT");
			root->right = right_rotation(root->right);
		} else {
			// puts("RIGHT RIGHT");
		}
		root = left_rotation(root);
		break;
	case 2:
		if (get_balance_factor(root->left) < 0) {
			// puts("LEFT RIGHT");
			root->left = left_rotation(root->left);
		} else {
			// puts("LEFT LEFT");
		}
		root = right_rotation(root);
		break;
	default:
		break;
	}

	int left_height = get_height(root->left);
	int right_height = get_height(root->right);
	root->height =
	    right_height > left_height ? right_height + 1 : left_height + 1;
	return root;
}

void tree_register_compare(tree_t * tree,
			   int (*comp_f)(const void *, const void *))
{
	tree->compare_f = comp_f;
}

void tree_register_destroy(tree_t * tree, void (*dest_f)(void *))
{
	tree->destroy_f = dest_f;
}

tree_t *tree_initialize(comp_f compare_f, dest_f destroy_f)
{
	tree_t *tree = NULL;
	if (NULL == compare_f) {
		goto EXIT;
	}
	tree = calloc(1, sizeof(tree_t));
	if (NULL == tree) {
		goto EXIT;
	}
	tree->compare_f = compare_f;
	tree->destroy_f = destroy_f;
 EXIT:
	return tree;
}

void tree_apply_to_all(tree_t * tree, void (*action_f)(void *), int reverse)
{
	if(NULL == tree || NULL == action_f) {
		return;
	}
	node_t *node = tree->root;
	node_apply_to_all(node, action_f, reverse);
}

void tree_destroy(tree_t ** tree)
{
	if (NULL == tree || NULL == *tree) {
		return;
	}
	node_destroy((*tree)->root, (*tree)->destroy_f);
	free(*tree);
	*tree = NULL;
}

void *tree_max(tree_t * tree)
{
	if (NULL == tree || NULL == (*tree).root) {
		return NULL;
	}
	node_t *node = tree->root;
	while (node->right) {
		node = node->right;
	}
	return node->data;
}

void *tree_min(tree_t * tree)
{
	if (NULL == tree || NULL == (*tree).root) {
		return NULL;
	}
	node_t *node = tree->root;
	while (node->left) {
		node = node->left;
	}
	return node->data;
}

void *tree_contains(tree_t * tree, void *data)
{
	node_t *node = tree->root;
	while (node) {
		int comp = tree->compare_f(node->data, data);
		if (comp < 0) {
			node = node->right;
		} else if (comp > 0) {
			node = node->left;
		} else {
			break;
		}
	}
	return node != NULL ? node->data : NULL;
}

int tree_remove(tree_t * tree, void *data)
{
	if (NULL == tree) {
		return 0;
	}
	int success = 0;
	tree->root =
	    node_remove(tree->root, data, &success, tree->compare_f,
			tree->destroy_f);
	if (success) {
		tree->size--;
	}
	return success;
}

void tree_print(tree_t * tree, void (*print_f)(const void *))
{

	node_print(tree->root, 0, print_f);
}

void *avl_add(tree_t * tree, void *data)
{
	node_t *node = calloc(1, sizeof(*node));
	if (NULL == node) {
		return NULL;
	}
	node->data = data;
	tree->root = node_add(tree->root, node, tree->compare_f);
	tree->size++;
	return data;
}
