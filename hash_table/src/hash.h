#ifndef _HASH_H_
#define _HASH_H_

typedef struct hashtable_t hashtable_t;

/*
*@brief creates a hash table with an initial capacity of capacity
*
*@int the maximum capacity of the list to start with.
*the capacit will expand as needed.
*/
hashtable_t *hash_table_create(int capacity);

/*
*@brief inserts the pointer to data with key as the retrieval value
*
*@hashtable_t the table to insert the key/value pair into
*@char* the string value used to enter/retrieve the specified data
*@void* a pointer to the data to store
*/
int hash_table_insert(hashtable_t * table, char *key, void *data);

/*
*@brief check the table to see if it has the specified key
*stored in it.
*
*@param hashtable_t* the table to check for the key
*@param char* a string value to check the table for.
*
*@return 1 if the key is in the table, otherwise 0
*/
int hash_table_contains(hashtable_t * table, char *key);

/*
*@brief retrieves the value stored by the key provided
* 
*@param hashtable_t the has table to get the value from
*@param char* the key used to store the value being retrieved
*
*@return a pointer to the stored value, if found, otherwise NULL
*/
void *hash_table_get(hashtable_t * table, char *key);

/*
*@brief removes the key and associated value from the hash_table
*
*@param hashtable_t the hash_table to edit.
*@param char* the key to find and remove from the table
*
*@return 1 if the item is removed, 0 if it was not found.
*/
int hash_table_delete(hashtable_t * table, char *key);

/*
*@brief prints the hash table to the terminal
*
*@param hashtable_t the hash table to print
*@param print_f the print function to print the user stored data
* contained in the hash_table.
*
*@note this is intended for troubleshooting purposes
*/
void hash_table_print(hashtable_t * table, void (*print_f)(const void *));

/*
*@brief a cleanup function to free all data stored in the hash_table
*
*@hashtable_t The table to be cleaned up. passed as a double pointer 
* so it can be set to NULL and prevent unintended access to freed memory
*@param destroy_f a user provided function to free the data they stored in the hash table. If this is NULL, then user provided data is not free'd.
*/
void hash_table_destroy(hashtable_t ** table, void (*destroy_f)(void *));

/*
*@brief iterates through the hash table and runs the supplied function against all data elements stored in the table.
*
*@param hashtable_t The table to be iterated through. 
*@param func_f the function to apply to each data element stored in the table.
*/
void hash_table_iter(hashtable_t* table, void (*func_f)(void*));
#endif
