#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

struct node {
	struct node *next;
	void *value;
};

void *push(linked_list_t * list, void *data)
{
	if (NULL == list || NULL == data) {
		return NULL;
	}
	node_t *node = calloc(1, sizeof(*node));
	if (NULL == node) {
		return NULL;
	}
	node->value = data;
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		node->next = list->head;
		list->head = node;
	}
	list->length++;
	return data;
}

void *pop(linked_list_t * list)
{
	if (NULL == list || NULL == list->head) {
		return NULL;
	}
	void *rv = list->head->value;
	node_t *node = list->head;
	list->head = list->head->next;
	free(node);
	list->length--;
	return rv;
}

void *enqueue(linked_list_t * list, void *data)
{
	if (NULL == list || NULL == data) {
		return NULL;
	}
	node_t *node = calloc(1, sizeof(*node));
	node->value = data;
	if (NULL == node) {
		return NULL;
	}
	if (NULL == list->head) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	list->length++;
	return data;
}

void *dequeue(linked_list_t * list)
{
	return pop(list);
}

void *insert_at(linked_list_t * list, void *val, int pos)
{
	if (0 == pos) {
		return push(list, val);
	}
	node_t *node = calloc(1, sizeof(*node));
	if (NULL == node) {
		return NULL;
	}
	node->value = val;
	node_t *insert = list->head;
	while (--pos && NULL != insert->next) {
		insert = insert->next;
	}
	node->next = insert->next;
	insert->next = node;
	(list->length)++;
	return val;
}

void apply_to_all(linked_list_t * list, void (*user_f)(void *))
{
	if (NULL == list || NULL == list->head) {
		return;
	}
	node_t *node = list->head;
	do {
		user_f(node->value);
	} while ((node = node->next) != NULL);
	puts("");
}

void destroy_list(linked_list_t ** list, void (*dest_f)(void *))
{
	if (NULL == *list) {
		return;
	}

	if (NULL != (*list)->head) {
		node_t *next = (*list)->head;
		do {
			next = (*list)->head->next;
			if (NULL != (*list)->head->value && NULL != dest_f) {
				dest_f((*list)->head->value);
				(*list)->head->value = NULL;
			}
			free((*list)->head);
			(*list)->head = next;
		} while ((*list)->head != NULL);

	}

	free(*list);
	*list = NULL;
}

void *get(linked_list_t * list, int pos)
{
	if (list->length <= pos) {
		return NULL;
	}
	node_t *node = list->head;
	while (pos-- && NULL != NULL) {
		node = node->next;
	}
	return node->value;
}

void sort(linked_list_t * list, int (*sort_f)(void *, void *))
{
	if (NULL == list->head) {
		return;
	}
	int b_changed = 1;
	while (b_changed) {
		b_changed = 0;
		for (node_t * node = list->head; node->next; node = node->next) {
			if (sort_f(node->value, node->next->value) < 0) {
				void *temp = node->next->value;
				node->next->value = node->value;
				node->value = temp;
				b_changed = 1;
			}
		}
	}
}

void remove_all(linked_list_t * list, void *data,int (*comp_f)(void *, void *), void (*dest_f)(void *))
{
	if (NULL == list || NULL == list->head) {
		return;
	}
	node_t *node = list->head;
	do {
		if (comp_f(data, node->value) == 0) {
			dest_f(node->value);
			node_t *temp = node->next;
			node->value = node->next->value;
			node->next = node->next->next;
			free(temp);
		}
	} while ((node = node->next) != NULL);
	puts("");
}

int count(linked_list_t * list, void *data, int (*comp_f)(void *, void *))
{
	if (NULL == list || NULL == list->head) {
		return 0;
	}
	node_t *node = list->head;
	int count = 0;
	do {
		if (comp_f(data, node->value) == 0) {
			count++;
		}
	} while ((node = node->next) != NULL);
	return count;
}

int replace(linked_list_t * list, void *original, void *replacement, int (*comp_f)(void *, void *), void (*dest_f)(void *))
{
	int replaced = 0;
	if (NULL == list || NULL == list->head) {
		return 0;
	}
	node_t *node = list->head;
	do {
		if (comp_f(original, node->value) == 0) {
			dest_f(node->value);
			node->value = replacement;
			replaced = 1;
			break;
		}
	} while ((node = node->next) != NULL);
	return replaced;
}

int is_empty(linked_list_t *list){
	return list->head == NULL;
}