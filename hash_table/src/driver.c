#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash.h"
#define BUFFER_SIZE  500
#define NAME_SIZE  500

typedef struct birthrt {
	char* date;
	int count;
} birthrt;
void print_birthrt(const void *ptr);
birthrt *initialize_birthrt(char *date, int count);
void destroy_birthrt(void *value);

int main(int argc, char* argv[])
{
	puts("setup");
	char *file = argc==2 ? argv[1] : "births.csv";	//_75.csv";
	FILE *fp = fopen(file, "r");
	if (NULL == fp) {
		return 1;
	}
	hashtable_t *population = hash_table_create(4);
	if (population == NULL) {
		fclose(fp);
		return 2;
	}

	puts("read in");
	int year, day, month, dow, count;
	char buffer[BUFFER_SIZE + 1] = { '\0' };
	//eat header
	fgets(buffer, BUFFER_SIZE, fp);
	int counter = -1;
	while (fgets(buffer, BUFFER_SIZE, fp) != NULL && counter--) {
		//puts("");
		//hash_table_print(population, print_birthrt);
		//sleep(2);
		sscanf(buffer, "%d,%d,%d,%d,%d", &year, &month, &day, &dow, &count);	//could do an if check on values read to check validity
		sprintf(buffer, "%2d/%2d/%4d", day, month, year);
		birthrt *tmp = initialize_birthrt(strdup(buffer), count);
		hash_table_insert(population, tmp->date, tmp);
	}
	hash_table_print(population, print_birthrt);

	printf("everything has been added\n");
	int b_loop = 1;
	char c[10] = {0};
	birthrt* tmp = NULL;
	do{
		hash_table_print(population, print_birthrt);
		printf("add [a] delete [d] quit [q] : ");
		scanf("%s", c);
        if (*c == 'q')
        {
            break;
        }
		printf("you chose '%s'\n",c);
		if ( *c == 'd'){	
			printf("enter a date to delete from the system:");
			b_loop = scanf("%d,%d,%d",&day,&month,&year)  == 3;
			sprintf(buffer, "%2d/%2d/%4d", day, month, year);
			if (b_loop){
				printf("\ncontains '%s': %s\n", buffer, hash_table_contains(population,buffer) ? "true" : "false");
				tmp = hash_table_get(population,buffer);
				if (NULL != tmp)
				{
					hash_table_delete(population,buffer);
					printf("removing '%s'\n",buffer);
					destroy_birthrt(tmp);
				}
			}
		}else{
			printf("enter a date to add to the system:");
			b_loop = scanf("%d,%d,%d",&day,&month,&year)  == 3;
			sprintf(buffer, "%2d/%2d/%4d", day, month, year);
			if (b_loop)
			{
				birthrt* tmp = initialize_birthrt(strdup(buffer),200);
				hash_table_insert(population,buffer,tmp);
			}
		}
	} while (b_loop);	
	
	hash_table_print(population, print_birthrt);


	fclose(fp);
	hash_table_destroy(&population, destroy_birthrt);
	hash_table_print(population, print_birthrt);
	
}

birthrt *initialize_birthrt(char *date, int count)
{
	birthrt *data = calloc(1, sizeof(*data));
	data->date = date;
	data->count = count;
	return data;
}

void destroy_birthrt(void *ptr)
{
	birthrt *data = ptr;
	free(data->date);
	free(data);
}

void print_birthrt(const void *ptr)
{
	if (NULL == ptr) {
		printf("no birthrt specified\n");
		return;
	}
	const birthrt *data = ptr;
	printf("'%s': %d births", data->date, data->count);
}
