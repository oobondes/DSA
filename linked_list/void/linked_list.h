/**
 * @file linked_list.h
 * @author Austin Vanasse (Austin.L.Vanasse.mil@Army.mil)
 * @brief A linked list implementation that provides and interface to function as either a stack or a queue.
 * @version 1.0
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __LINKED_LIST__
#define __LINKED_LIST__

typedef struct node node_t;

typedef struct linked_list {
	node_t *head;
	node_t *tail;
	int length;
} linked_list_t;

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *push(linked_list_t * list, void *data);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *enqueue(linked_list_t * list, void *data);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *insert_at(linked_list_t * list, void *val, int pos);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void apply_to_all(linked_list_t * list, void (*user_f)(void *));

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void destroy_list(linked_list_t ** list, void (*dest_f)(void *));

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *dequeue(linked_list_t * list);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *pop(linked_list_t * list);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/
void *get(linked_list_t * list, int pos);

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/

void sort(linked_list_t * list, int (*sort_f)(void *, void *));

/*
*@brief 
*
*@param 
*@param 
*
*@return 
*/

void remove_all(linked_list_t * list, void *data,int (*comp_f)(void *, void *), void (*dest_f)(void *));

/*
*@brief returns the number of times the specified data element was found in the list
*
*@param the list to search for the value
*@param a pointer to the data the caller is looking for
*@param a compare function used to find the data the caller is looking for. comp_f
*       should return 0 when it returns 2 identical data elements.
*
*@return the number of times the element was found in the list.
*/

int count(linked_list_t * list, void *data, int (*comp_f)(void *, void *));

/*
* DELETE THIS??
*/

int replace(linked_list_t * list, void *original, void *replacement, int (*comp_f)(void *, void *), void (*dest_f)(void *));

/*
*@brief checks if the list is empty or not.
*
*@param linked_list_t the list to check whether it is empty or not
*
*@return 1 if the list is empty or NULL, 0 otherwise
*/

int is_empty(linked_list_t *list);
#endif
