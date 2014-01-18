#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* a set is one observation */
struct set {
	int frame;
	float eye;
	float forehead;
	float mouth;
	struct set * prev;
	struct set * next;
};

/* extreme values */
struct exva {
	float eye_max;
	float eye_min;
	float eye_max_adjust;
	float forehead_max;
	float forehead_min;
	float forehead_max_adjust;
	float mouth_max;
	float mouth_min;
	float mouth_max_adjust;
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

/**
 * calculate all extreme values which are needed for further calculations
 * @param exva  struct exva pointer which store all values
 * @param start first set
 */
void calculate_extreme_values( struct exva *exva, struct set * start ) {
	// find min and max values of eye, mouth, and forehead
	exva->eye_max = find_max_value( start, "eye" );
	exva->eye_min = find_min_value( start, "eye" );
	exva->eye_max_adjust = exva->eye_max - exva->eye_min;

	exva->forehead_max = find_max_value( start, "forehead" );
	exva->forehead_min = find_min_value( start, "forehead" );
	exva->forehead_max_adjust = exva->forehead_max - exva->forehead_min;

	exva->mouth_max = find_max_value( start, "mouth" );
	exva->mouth_min = find_min_value( start, "mouth" );
	exva->mouth_max_adjust = exva->mouth_max - exva->mouth_min;

	printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", exva->eye_max, exva->eye_min, exva->eye_max_adjust);
	printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", exva->forehead_max, exva->forehead_min, exva->forehead_max_adjust);
	printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", exva->mouth_max, exva->mouth_min, exva->mouth_max_adjust);
}