#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "trie.h"

trie_t * create_trie_t(void){
    return calloc(1,sizeof(trie_t));
}

int add(trie_t* trie, char* str){
    if (NULL == trie || NULL == str){
        return 0;
    }
    int rv = 0;
    unsigned char letter = *str;
    if(trie->next[letter] == NULL){
        trie->next[letter] = create_trie_t();
    }
    if ('\0' == letter){
        rv = 1;
    } else{
        rv = add(trie->next[letter],str+1);
    }
    return rv;
}

void print_words(trie_t* trie, char *start, int letter){
    unsigned char* word = NULL;
    size_t len = strlen(start);
    if (NULL != trie->next[0]){
        puts(start);
    }
    for(int i = 1; i < NUM_ASCII; ++i){
        if (NULL != trie->next[i]){
            word = calloc(len+2,sizeof(*word));
            strcat(word,start);            
	    word[len]= (unsigned char) letter;
            print_words(trie->next[i],word,i);
            free(word);
        }
    }
}

void print_word_possibilities(trie_t* trie, char* start){
    //if ('\0' == *start){
    //    return;
    //}
    unsigned char* word = start;
    while (trie && *start){
        trie = trie->next[*start];
        start++;
	if ('\0' == *start){
		break;
	}
    }
    if (NULL == trie){
        return;
    }
    word[strlen(word)] = 0;
    print_words(trie,word, 0);

}


void trie_t_destroy(trie_t** trie){
    if(*trie == NULL || trie == NULL){
        return;
    }
    for (int i = 0; i < NUM_ASCII; ++i){
        trie_t_destroy(&((*trie)->next[i]));
    }
    free(*trie);
    *trie = NULL;
}
