#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pqueue.h"

typedef struct element_t {
	int priority;
	void *data;
} element_t;

struct pqueue_t {
	uint16_t tail;
	uint16_t size;
	element_t *elements;
};
static uint16_t left_child(uint16_t index)
{
	return (index * 2) + 1;
}

static uint16_t right_child(uint16_t index)
{
	return (index * 2) + 2;
}

static uint16_t get_parent(uint16_t index)
{
	return (index - 1) / 2;
}

static int bubble_up(uint16_t index, element_t * element)
{
	uint8_t status = 0;
	uint16_t parent = get_parent(index);
	if (element[index].priority < element[parent].priority) {
		element_t tmp = element[index];
		element[index] = element[parent];
		element[parent] = tmp;
		status = 1;
	}
	return status;
}

static int bubble_down(uint16_t index, element_t * element, int size)
{
	uint8_t status = 0;
	uint16_t left = left_child(index);
	uint16_t right = right_child(index);
	if (left >= size){
		return index;
	}
	uint16_t next;
	if (right >= size){
		 next = left;
	}else{
		next = element[right].priority > element[left].priority ? left : right;
	}
	if (element[index].priority > element[next].priority) {
		element_t tmp = element[index];
		element[index] = element[next];
		element[next] = tmp;
		index = next;
	}
	return index;
}

pqueue_t *pqueue_create(int size)
{
	pqueue_t *queue = calloc(1, sizeof(*queue));
	if (NULL == queue) {
		goto EXIT;
	}
	queue->size = size;
	queue->elements = calloc(size, sizeof(*(queue->elements)));
	if (NULL == queue->elements) {
		free(queue);
		queue = NULL;
	}

 EXIT:
	return queue;
}

int pqueue_add(pqueue_t * queue, int priority, void *data)
{
	int status = 0;
	if (queue->tail == queue->size) {
		element_t *tmp = realloc(queue->elements, sizeof(element_t) * queue->size * 2);
		if (NULL == tmp) {
			fprintf(stderr, "mem error");
			goto EXIT;
		}
		queue->elements = tmp;
		queue->size *= 2;
	}

	uint16_t index = queue->tail++;
	queue->elements[index].priority = priority;
	queue->elements[index].data = data;
	while (bubble_up(index, queue->elements)) {
		index = get_parent(index);
	}
	status = 1;
 EXIT:
	return status;
}

void *pqueue_next(pqueue_t * queue)
{
	void *data = NULL;
	if (NULL == queue || 0 == queue->tail) {
		goto EXIT;
	}

	data = queue->elements[0].data;
	uint16_t index = 0;
	uint16_t prev = index;
	queue->elements[index] = queue->elements[--(queue->tail)];
	while (prev != (index = bubble_down(index, queue->elements, queue->tail))) {
		prev = index;
	}

 EXIT:

	return data;
}

int pqueue_is_empty(pqueue_t * queue)
{
	if (queue) {
		return queue->tail == 0;
	}
	return 1;
}

void pqueue_destroy(pqueue_t ** queue, void (*destroy_f)(void*))
{
	if (queue && *queue) {
		if (destroy_f){
			for (int i = 0; i < (*queue)->tail; ++i){
				destroy_f((*queue)->elements[i].data);
			}
		}
		free((*queue)->elements);
		free(*queue);
		*queue = NULL;
	}
}

void pqueue_print(pqueue_t * queue, void (*print_f)(void *))
{
	for(int i = 0; i < queue->tail; ++i) {
		printf("priority: %d --- ", queue->elements[i].priority);
		print_f(queue->elements[i].data);
		puts("");
	}
}
