#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "pqueue.h"
#define BUFFER_SIZE  500
#define NAME_SIZE  500
void print_num(void* data){
	printf("%d",* (int*)data);
}
int main(int argc, char *argv[])
{
	pqueue_t *queue = pqueue_create(4);
	int words[100] = {0};
	for (int i = 0; i < 40; ++i) {
		words[i] = rand()%0xFFFF;
		printf("adding '%d'\n", words[i]);
		pqueue_add(queue,rand()%5000, &words[i]);
		pqueue_print(queue, print_num);
	}
	print_num("print q");
	pqueue_print(queue, print_num);
	puts("================");
	puts("test next/empty");
	while (!pqueue_is_empty(queue)) {
		int *tmp = pqueue_next(queue);
		printf("[%d]\n",*tmp);
		pqueue_print(queue,print_num);
        puts("");
	}
	puts("end loop");
	pqueue_destroy(&queue, NULL);
}
