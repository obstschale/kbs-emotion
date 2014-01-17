#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct set
{
	int frame;
	int eye;
	int forehead;
	int mouth;
	struct set * prev;
	struct set * next;
};

void print_set( struct set * obs) {
	printf("---Set #%i---\n", obs->frame);
	printf("Eye: %i\n", obs->eye);
	printf("Forehead: %i\n", obs->forehead);
	printf("Mouth: %i\n", obs->mouth);
	printf("\n");
}

void show_all_sets( struct set * ptr ) {
	do {
		print_set( ptr );
		ptr = ptr->next;
	} while ( ptr != NULL );
}