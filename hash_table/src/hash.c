#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

#define DELETED (void*) 1
#define LIMIT .7f
#define PRIME 4294967291 

typedef struct element_t{
    char* key;
    void* data;
}element_t;

struct hashtable_t{
    element_t* elements;
    uint32_t capacity;
    uint32_t num_elements;
    uint32_t usage;
};
static int rehash(hashtable_t* table);

//https://stackoverflow.com/questions/11489428/how-to-make-vim-paste-from-and-copy-to-systems-clipboard
static uint32_t hash(char* value){
    int key = 1;
    while (*value)
    {
        key = *(value++) + (key * PRIME );
    }
    return key;
}

hashtable_t* hash_table_create(int capacity){
    hashtable_t* table = calloc(1,sizeof(*table));
    if (NULL == table){
        goto EXIT;
    }
    table->capacity = capacity;
    table->elements = calloc(capacity, sizeof(*(table->elements)));
    if (NULL == table->elements){
        free(table);
        table = NULL;
    }

    EXIT:
    return table;
}

void hash_table_destroy(hashtable_t** table, void (*destroy_f)(void*)){
    //ABC
    if (NULL == table || NULL == *table){
        return;
    }

    for (uint32_t i = 0; i < (*table)->capacity; ++i){
        if ((*table)->elements[i].data > DELETED && NULL != destroy_f){
            destroy_f(((*table)->elements[i].data));
        }
        if ((*table)->elements[i].data > DELETED){
            free((*table)->elements[i].key);
        }
    }

    free((*table)->elements);
    free(*table);
    *table = NULL;
}

int hash_table_insert(hashtable_t* table, char* key, void* data){
    int rv = 0;
    if (NULL == table || NULL == data){
        goto EXIT;
    }
    uint32_t index = hash(key) % table->capacity;
    //check the index and linearly probe until a spot is found
    //for the data to be place
    while (table->elements[index].data > DELETED){
        if (!strcmp(key,table->elements[index].key)){
            goto EXIT;
        }
        index = (index + 1) % table->capacity;
    }
    table->elements[index].data = data;
    table->elements[index].key = strdup(key);
    table->num_elements++;
    table->usage++;
    rv =  1;
    
    //resize the hash_table if it is close to capacity
    if (((float) (table->usage)) / ((float) (table->capacity)) > LIMIT){
        rehash(table);
    }

    EXIT:
    return rv;
}

int hash_table_contains(hashtable_t* table, char* key){
    int rv = 0;
    if (NULL == table || NULL == key){
        goto EXIT;
    }
    uint32_t key_hash = hash(key);
    uint32_t index = key_hash % table->capacity;
    //linearly probe the table until the key is found or a NULL is found
    while (table->elements[index].data && hash(table->elements[index].key) != key_hash){
        index = (index + 1) % table->capacity;
    }
    rv = table->elements[index].data > DELETED;
    
    EXIT:
    return rv;
}

void* hash_table_get(hashtable_t* table, char* key){
    void* rv = NULL;
    if (NULL == table){
        goto EXIT;
    }

    uint32_t key_hash = hash(key);
    uint32_t index = hash(key) % table->capacity;
    //linearly prove the table until the data to be deleted is found, or a NULL pointer is found
    while (table->elements[index].data != NULL && hash(table->elements[index].key) != key_hash){
        index = (index + 1) % table->capacity;
    }

    rv = table->elements[index].data > DELETED ? table->elements[index].data : NULL;
    
    EXIT:
    return rv;
}

int hash_table_delete(hashtable_t* table, char* key){
    int rv = 0;
    if (NULL == table){
        goto EXIT;
    }
    uint32_t key_hash = hash(key);
    uint32_t index = hash(key) % table->capacity;
    //linearly prove the table until the data to be deleted is found, or a NULL pointer is found
    while (table->elements[index].data != NULL && hash(table->elements[index].key) != key_hash){
        index = (index +1) % table->capacity;
    }
    if (table->elements[index].data > DELETED){
        free(table->elements[index].key);
        table->elements[index].data = DELETED;
        rv = 1;
        table->num_elements--;
    }
    EXIT:
    return rv;
}

void hash_table_print(hashtable_t* table, void (*print_f)(const void*)){
    if(NULL == table){
        printf("(nil)\n");
        return;
    }
    for (int i = 0; i < table->capacity; ++i){
        if (table->elements[i].data > DELETED){
            printf("%3d: ",i);
            print_f(table->elements[i].data);
        }else{
            printf("%s", table->elements[i].data == DELETED ? "[del]" : "[NIL]");
        }
        puts("");
    }
}

static int rehash(hashtable_t* table){
    int rv = 0;
    //save all attributes of table in case this fails
    int capacity = table->capacity;
    int usage = table->usage;
    int size = table->num_elements;
    element_t* elements = table->elements;
    table->elements = calloc(table->capacity*2,sizeof(*elements));
    if (NULL == table->elements){
        table->elements = elements;
        goto EXIT;
    }
    //double capacity and rehash/add all elements to the table
    table->capacity = 2*capacity;
    table->num_elements = 0;
    table->usage = 0;
    for (int i = 0; i < capacity; ++i){
        if(elements[i].data > DELETED && 0 == hash_table_insert(table, elements[i].key, elements[i].data)){
            goto CLEANUP;
        }
        free(elements[i].key);
    }
    rv = 1;
    free(elements);
    goto EXIT;

    CLEANUP:
    free(table->elements);
    table->elements = elements;
    table->capacity = capacity;
    table->usage = usage;
    table->num_elements = size;


    EXIT:
    return rv;
}

void hash_table_iter(hashtable_t* table, void (*func_f)(void*)){
    if(NULL == table){
        return;
    }
    for (int i = 0; i < table->capacity; ++i){
        if (table->elements[i].data > DELETED){
            func_f(table->elements[i].data);
        }
    }
}
