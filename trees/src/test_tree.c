#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tree_void.h"
#define BUFFER_SIZE  500
#define NAME_SIZE  500

typedef struct birth_rate {
	int year;
	int month;
	int day;
	int day_of_week;
	int births;
} birth_rate_t;
int sort_by_birth_rate(const void *one, const void *two);
int sort_by_date(const void *one, const void *two);
void print_birth_rate(const void *ptr);
birth_rate_t *initialize_birth_rate_t(int year, int month, int day,
				      int day_of_week, int births);
void destroy_birth_rate_t(void *brt);

int main(int argc, char *argv[])
{
	puts("setup");
	char *file = argc == 2 ? argv[1] : "births.csv";
	FILE *fp = fopen(file, "r");
	if (NULL == fp) {
		return 1;
	}
	tree_t *birth_tree =
	    tree_initialize(sort_by_birth_rate, destroy_birth_rate_t);
	if (birth_tree == NULL) {
		fclose(fp);
		return 2;
	}
	tree_register_compare(birth_tree, sort_by_birth_rate);
	tree_register_destroy(birth_tree, destroy_birth_rate_t);

	puts("read in");
	int year, month, day, day_of_week, births;
	char buffer[BUFFER_SIZE + 1] = { '\0' };
	fgets(buffer, BUFFER_SIZE, fp);	//eat header
	int counter = 999999999;
	while (fgets(buffer, BUFFER_SIZE, fp) != NULL && counter--) {
		//puts("");
		//tree_print(birth_tree,print_birth_rate);
		//sleep(2);
		sscanf(buffer, "%d,%d,%d,%d,%d", &year, &month, &day, &day_of_week, &births);	//could do an if check on values read to check validity
		birth_rate_t *tmp =
		    initialize_birth_rate_t(year, month, day, day_of_week,
					    births);
		avl_add(birth_tree, tmp);
	}
	tree_print(birth_tree, print_birth_rate);

	printf("everything has been added\n");
	// tree_apply_to_all(birth_tree, print_birth_rate, 0);
	puts("MIN:");
	print_birth_rate(tree_min(birth_tree));
	puts("MAX:");
	print_birth_rate(tree_max(birth_tree));
	puts("tree print");
	tree_print(birth_tree, print_birth_rate);
	fclose(fp);
	tree_destroy(&birth_tree);
}

int sort_by_birth_rate(const void *left, const void *right)
{
	birth_rate_t *left_brt = (birth_rate_t *) left;
	birth_rate_t *right_brt = (birth_rate_t *) right;
	return left_brt->births - right_brt->births;
}

int sort_by_date(const void *left, const void *right)
{
	birth_rate_t *left_brt = (birth_rate_t *) left;
	birth_rate_t *right_brt = (birth_rate_t *) right;
	int comp = left_brt->year - right_brt->year;
	if (0 != comp) {
		goto EXIT;
	}
	comp = left_brt->month - right_brt->month;
	if (0 != comp) {
		goto EXIT;
	}
	comp = left_brt->day - right_brt->day;
	if (0 != comp) {
		goto EXIT;
	}
 EXIT:
	return comp;
}

void print_birth_rate(const void *ptr)
{
	birth_rate_t *brt = (birth_rate_t *) ptr;
	printf("%d/%d/%d: %d", brt->day, brt->month, brt->year, brt->births);
}

birth_rate_t *initialize_birth_rate_t(int year, int month, int day,
				      int day_of_week, int births)
{
	birth_rate_t *brt = calloc(1, sizeof(*brt));
	if (NULL == brt) {
		return NULL;
	}
	brt->year = year;
	brt->month = month;
	brt->day = day;
	brt->day_of_week = day_of_week;
	brt->births = births;
	return brt;
}

void destroy_birth_rate_t(void *brt)
{
	free(brt);
}
