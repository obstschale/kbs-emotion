#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/**
 * Find max value in all sets of a specific parameter
 * (eye | mouth | forehead). This function goes recursively
 * thru all sets and compares the values using fmax()
 * @param  ptr    pointer of current check position
 * @param  string comparision string
 * @return int    max value or -1 if invalid
 */
int find_max_value( struct set * ptr, char* string ) {
	if( ptr->next != NULL ) {
		if ( strcmp( "eye", string ) == 0 ) {
			return fmax( ptr->eye, find_max_value( ptr->next, "eye" ) );
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return fmax( ptr->mouth, find_max_value( ptr->next, "mouth" ) );
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return fmax( ptr->forehead, find_max_value( ptr->next, "forehead" ) );
		} else {
			printf("Error: Invalid string value\n");
			return -1;
		}
	} else {
		if ( strcmp( "eye", string ) == 0 ) {
			return ptr->eye;
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return ptr->mouth;
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return ptr->forehead;
		} else {
			printf("Error: Invalid string value\n");
			return -1;
		}
	}
}

/**
 * Find min value in all sets of a specific parameter
 * (eye | mouth | forehead). This function goes recursively
 * thru all sets and compares the values using fmin()
 * @param  ptr    pointer of current check position
 * @param  string comparision string
 * @return int    min value or -1 if invalid
 */
int find_min_value( struct set * ptr, char* string ) {
	if( ptr->next != NULL ) {
		if ( strcmp( "eye", string ) == 0 ) {
			return fmin( ptr->eye, find_min_value( ptr->next, "eye" ) );
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return fmin( ptr->mouth, find_min_value( ptr->next, "mouth" ) );
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return fmin( ptr->forehead, find_min_value( ptr->next, "forehead" ) );
		} else {
			printf("Error: Invalid string value\n");
			return -1;
		}
	} else {
		if ( strcmp( "eye", string ) == 0 ) {
			return ptr->eye;
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return ptr->mouth;
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return ptr->forehead;
		} else {
			printf("Error: Invalid string value\n");
			return -1;
		}
	}
}