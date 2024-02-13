#ifndef _PQUEUE_H_
#define _PQUEUE_H_

typedef struct pqueue_t pqueue_t;

pqueue_t* pqueue_create(int size);
int pqueue_add(pqueue_t* queue, int priority, void* data);
int pqueue_is_empty(pqueue_t* queue);
void* pqueue_next(pqueue_t* queue);
void pqueue_destroy(pqueue_t ** queue, void (*destroy_f)(void*));
void pqueue_print(pqueue_t* queue, void (*print_f)(void*));



#endif
