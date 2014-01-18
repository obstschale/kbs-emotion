#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dempster/dempster.c"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

int verbose = TRUE;

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
	printf("%-5s | %-5s | %-7s | %-7s\n", "Set #", "Eye", "Forehead", "Mouth");
	printf("%5i | ", obs->frame);
	printf("%4.2f | ", obs->eye);
	printf("%6.2f   | ", obs->forehead);
	printf("%4.2f\n", obs->mouth);
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
void calculate_extreme_values( struct exva * exva, struct set * start ) {
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

	if ( verbose ) {
		printf("%9s %-7s | %-7s | %-7s\n", "", "Eye", "Forehead", "Mouth");
		printf("%9s ", "max:");
		printf("%07.2f | %07.2f  | %07.2f\n", exva->eye_max, exva->forehead_max, exva->mouth_max);

		printf("%9s ", "min:");
		printf("%07.2f | %07.2f  | %07.2f\n", exva->eye_min, exva->forehead_min, exva->mouth_min);

		printf("%9s ", "adjusted:");
		printf("%07.2f | %07.2f  | %07.2f\n", exva->eye_max_adjust, exva->forehead_max_adjust, exva->mouth_max_adjust);

		printf("\n");
	}
}

void calculate_evidence( struct exva * exva, struct set * set_ptr ) {
	int i;
	float eye_value, mouth_value, forehead_value;
	char emotions[5][10] = {"fear", "surprise", "contempt", "disgust", "fury"};
	float pl[5];

	// while ( set_ptr->next != NULL ) {
		if ( verbose ) {
			/* print set before calculating evidences */
			print_set( set_ptr );
		}

		// @TODO: calculate entry values for each pair
		eye_value = set_ptr->eye - exva->eye_min;
		forehead_value = set_ptr->forehead - exva->forehead_min;
		mouth_value = set_ptr->mouth - exva->mouth_min;

		// percentage values
		float eye_per = eye_value / exva->eye_max_adjust;
		float forehead_per = forehead_value / exva->forehead_max_adjust;
		float mouth_per = mouth_value / exva->mouth_max_adjust;
		// inverse percentage values
		float eye_per_inv = 1 - eye_per;
		float forehead_per_inv = 1 - forehead_per;
		float mouth_per_inv = 1 - mouth_per;

		if ( verbose ) {
			/* print observation values, percentages and inverse */
			printf("%9s %-6s | %7s | %-7s\n", "", "Eye", "Forehead", "Mouth");

			printf("%-10s", "Values: ");
			printf("%06.2f | %06.2f   | %06.2f\n", eye_value, forehead_value, mouth_value);
			printf("%-10s", "Percent: ");
			printf("%06.2f | %06.2f   | %06.2f\n", eye_per, forehead_per, mouth_per);
			printf("%-10s", "Inverse: ");
			printf("%06.2f | %06.2f   | %06.2f\n", eye_per_inv, forehead_per_inv, mouth_per_inv);
			printf("\n");
		}

		int eye_vector[5] = {0,0,0,0,0};
		int forehead_vector[5] = {0,0,0,0,0};
		int mouth_vector[5] = {0,0,0,0,0};
		// calculate bitvector
		if ( eye_per > 0.6 ) {
			eye_vector[0] = 1;
			eye_vector[1] = 1;
		} else if ( eye_per < 0.4 ) {
			eye_vector[2] = 1;
			eye_vector[3] = 1;
		} else{
			eye_vector[0] = 1;
			eye_vector[1] = 1;
			eye_vector[2] = 1;
			eye_vector[3] = 1;
		}

		if ( forehead_per > 0.6 ) {
			forehead_vector[0] = 1;
			forehead_vector[1] = 1;
		} else if ( forehead_per < 0.4 ) {
			forehead_vector[4] = 1;
		} else {
			forehead_vector[0] = 1;
			forehead_vector[1] = 1;
			forehead_vector[4] = 1;
		}

		if ( mouth_per > 0.6 ) {
			mouth_vector[1] = 1;
		} else if ( mouth_per < 0.4 ) {
			mouth_vector[3] = 1;
			mouth_vector[4] = 1;
		} else {
			mouth_vector[1] = 1;
			mouth_vector[3] = 1;
			mouth_vector[4] = 1;
		}

		// {Angst, Ueberraschung, Verachtung, Ekel, Wut}
		// const int entry1[5] = {0,0,1,1,0}; // Augen
		// const int entry2[5] = {0,0,0,0,1}; // Stirn
		// const int entry3[5] = {0,0,0,1,1}; // Mund

		basicMeasure m1, m2, m3, *res1, *res2;
		set *p1, *p2, *p3;
		p1 = createAlternatives((int *) eye_vector, 5);
		p2 = createAlternatives((int *) forehead_vector, 5);
		p3 = createAlternatives((int *) mouth_vector, 5);
		createBasicMeasure(&m1,5);
		addMeasureEntry(&m1,*p1, (eye_per > eye_per_inv) ? eye_per : eye_per_inv);
		// printBasicMeasure(&m1);

		createBasicMeasure(&m2,5);
		addMeasureEntry(&m2,*p2, (forehead_per > forehead_per_inv) ? forehead_per : forehead_per_inv);
		// printBasicMeasure(&m2);
		// printf("accumulate ...\n");

		res1 = getAccumulatedMeasure(&m1,&m2);
		// printBasicMeasure(res1);

		createBasicMeasure(&m3,5);
		addMeasureEntry(&m3,*p3, (mouth_per > mouth_per_inv) ? mouth_per : mouth_per_inv);
		// printBasicMeasure(&m3);
		// printf("accumulate ...\n");

		res2 = getAccumulatedMeasure(res1,&m3);
		// printBasicMeasure(res2);
		
		if ( verbose ) printf(  " Nr : Pl(x)  |  B(x)   |  Z(x) \n");

		for (i=0;i<5;i++) {
			/* print all evidences for all alternatives */
			if ( verbose ) printf("[%d] : %5.3f  |  %5.3f  | %5.3f \n", i, plausibility(res2,i), singleBelief(res2,i), singleDoubt(res2,i));
			pl[i] = plausibility(res2,i);
		}

		double max_emotion = pl[0];
		int index = 0;
		for ( i = 1; i < 5; ++i) {
			max_emotion = (max_emotion > pl[i]) ? max_emotion : pl[i];
			index = (max_emotion > pl[i]) ? index : i;
		}

		printf("[%i] %5.3f %s\n", index, max_emotion, emotions[index]);
		
		printf("*** Hit <Enter> to continue\n");
		// getc( stdin );
		set_ptr = set_ptr->next;
	// } // END while ( set_ptr->next != NULL ) 
}

