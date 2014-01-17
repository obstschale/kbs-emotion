#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct set
{
	int frame;
	float eye;
	float forehead;
	float mouth;
	struct set * prev;
	struct set * next;
};

void print_set( struct set * obs) {
	printf("---Set #%i---\n", obs->frame);
	printf("Eye: %5.3f\n", obs->eye);
	printf("Forehead: %5.3f\n", obs->forehead);
	printf("Mouth: %5.3f\n", obs->mouth);
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
 * thru all sets and compares the values using fmaxf()
 * @param  ptr    pointer of current check position
 * @param  string comparision string
 * @return float    max value or -1 if invalid
 */
float find_max_value( struct set * ptr, char* string ) {
	if( ptr->next != NULL ) {
		if ( strcmp( "eye", string ) == 0 ) {
			return fmaxf( ptr->eye, find_max_value( ptr->next, "eye" ) );
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return fmaxf( ptr->mouth, find_max_value( ptr->next, "mouth" ) );
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return fmaxf( ptr->forehead, find_max_value( ptr->next, "forehead" ) );
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
 * thru all sets and compares the values using fminf()
 * @param  ptr    pointer of current check position
 * @param  string comparision string
 * @return float    min value or -1 if invalid
 */
float find_min_value( struct set * ptr, char* string ) {
	if( ptr->next != NULL ) {
		if ( strcmp( "eye", string ) == 0 ) {
			return fminf( ptr->eye, find_min_value( ptr->next, "eye" ) );
		} else if ( strcmp( "mouth", string ) == 0 ) {
			return fminf( ptr->mouth, find_min_value( ptr->next, "mouth" ) );
		} else if ( strcmp( "forehead", string ) == 0 ) {
			return fminf( ptr->forehead, find_min_value( ptr->next, "forehead" ) );
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