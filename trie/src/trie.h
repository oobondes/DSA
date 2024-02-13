/**
 * @file trie.h
 * @author Austin Vanasse (Austin.L.Vanasse.mil@Army.mil)
 * @brief 
 * @version 1.0
 * @date 2024-1-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _TRIE_H_
#define _TRIE_H_
#define NUM_ASCII 255

/*
*@brief used to create a graph of connections that letters make through words that are fed into it.
*/
typedef struct trie_t{
    struct trie_t* next[NUM_ASCII];
} trie_t;

/*
 *@brief heap allocates a trie struct
 *
 *@return a pointer to a trie_t on success, NULL otherwise.
*/
trie_t * create_trie_t(void);

/*
*@brief inserts a new word into the trie.
*
*@param the trie_t to add str into
*@param the strings to store in the trie_t
*
*@return 1 on success, 0 otherwise
*/

int add(trie_t* trie, char* str);

/*
*@brief prints every combination of chars that can be made from trie, while
prepending each one with the string in start.
*
*@param the trie_t that is storing all letter combinations stored after start
*@param the start string to print before all possible letter combinations.
*/

void print_word_possibilities(trie_t* trie, char* start);

/*
*@brief frees the memory stored in *trie_t and sets *trie_t to NULL
*
*@param the trie_t to be freed
*/

void trie_t_destroy(trie_t** trie);


#endif
