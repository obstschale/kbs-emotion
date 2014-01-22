#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h> /* for getopt */
#include "dempster/dempster.c"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef VEC_SIZE
#define VEC_SIZE 5
#endif

// int verbose = TRUE;
/* option flags */
struct flags {
	int all;
	int verbose;
	char *file;
};

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

void set_flag_default( struct flags *arguments ) {
	arguments->all     = 0;
	arguments->verbose = 0;
	arguments->file    = NULL;
}

void usage() {
	fprintf(stderr, "usage: ./kbs [-v] [-a] [-h] -f FILE\n\n");
	fprintf(stderr, "\t-v\t\tverbose output\n");
	fprintf(stderr, "\t-a\t\tcalculate all in one run. If -a is not given the user is asked to continue or quit after each calculation.\n");
	fprintf(stderr, "\t-h\t\tthis usage dialog\n");
	fprintf(stderr, "\t-f FILE\t\trelative file path to CSV file\n");

	fprintf(stderr, "\n");
}

void print_set( struct set * obs) {
	printf("%-5s | %-5s | %-7s | %-7s\n", "Set #", "Eye", "Forehead", "Mouth");
	printf("%5i | ", obs->frame);
	printf("%4.2f | ", obs->eye);
	printf("%7.2f  | ", obs->forehead);
	printf("%4.2f\n", obs->mouth);
	printf("\n");
}

void print_set_emotion( struct set * obs, double max_emotion, char emotion[] ) {
	printf("\n");
	printf("%-5s | %-5s | %-7s | %-7s | %-7s | %-7s\n", "Set #", "Eye", "Forehead", "Mouth", "highest Pl(x)", "Emotion");
	printf("%5i | ", obs->frame);
	printf("%4.2f | ", obs->eye);
	printf("%7.2f  | ", obs->forehead);
	printf("%4.2f   | ", obs->mouth);
	printf("%4.2f          | ", max_emotion);
	printf("%-7s\n", emotion);
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
void calculate_extreme_values( struct exva * exva, struct set * start, struct flags *arguments ) {
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

	if ( arguments->verbose ) {
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

struct set* find_frame( struct set * ptr, int search ) {

	while( ptr->frame != search ) {
		ptr = ptr->next;
	}

	return ptr;
}

void reset_vector( int *vector, int size ) {
	int i;

	for (i = 0; i < size; ++i) {
		vector[i] = 0;
	}
}

void calculate_evidence( struct exva * exva, struct set * set_ptr, struct flags *arguments ) {
	int i;
	float eye_value, mouth_value, forehead_value;
	char emotions[5][10] = {"fear", "surprise", "contempt", "disgust", "fury"};
	float pl[5];
	double max_emotion = pl[0];
	int index = 0;
	int eye_vector[VEC_SIZE];
	int forehead_vector[VEC_SIZE];
	int mouth_vector[VEC_SIZE];
	basicMeasure m1, m2, m3, *res1, *res2;
	set *p1, *p2, *p3;

	printf("*** Hit <Enter> after each calculation to continue\n");
	while ( set_ptr != NULL ) {
	// set_ptr = find_frame( set_ptr, 50 );

		/* initilize or reset bit vectors for calc */
		reset_vector( eye_vector, VEC_SIZE );
		reset_vector( forehead_vector, VEC_SIZE );
		reset_vector( mouth_vector, VEC_SIZE );
		max_emotion = 0;
		index = -1;

		if ( arguments->verbose ) {
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

		if ( arguments->verbose ) {
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

		// calculate bitvector
		if ( eye_per > 0.5 ) {
			eye_vector[0] = 1;
			eye_vector[1] = 1;
		} else if ( eye_per <= 0.5 ) {
			eye_vector[2] = 1;
			eye_vector[3] = 1;
		} else{
			eye_vector[0] = 1;
			eye_vector[1] = 1;
			eye_vector[2] = 1;
			eye_vector[3] = 1;
		}

		if ( forehead_per > 0.5 ) {
			forehead_vector[0] = 1;
			forehead_vector[1] = 1;
		} else if ( forehead_per <= 0.5 ) {
			forehead_vector[4] = 1;
		} else {
			forehead_vector[0] = 1;
			forehead_vector[1] = 1;
			forehead_vector[4] = 1;
		}

		if ( mouth_per > 0.5 ) {
			mouth_vector[1] = 1;
		} else if ( mouth_per <= 0.5 ) {
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
		
		if ( arguments->verbose ) printf(  " Nr : Pl(x)  |  B(x)   |  Z(x) \n");

		for (i=0;i<5;i++) {
			/* print all evidences for all alternatives */
			if ( arguments->verbose ) printf("[%d] : %5.3f  |  %5.3f  | %5.3f \n", i, plausibility(res2,i), singleBelief(res2,i), singleDoubt(res2,i));
			pl[i] = plausibility(res2,i);
		}

		for ( i = 1; i < 5; ++i) {
			max_emotion = (max_emotion > pl[i]) ? max_emotion : pl[i];
			index = (max_emotion > pl[i]) ? index : i;
		}

		if ( arguments->verbose ) {
			/* print set and emotion */
			print_set_emotion( set_ptr, max_emotion, emotions[index] );
		} else {
			/* just print emotion */
			printf("#%2d: Probability of %5.3f%% that the emotion is %s\n", set_ptr->frame, max_emotion*100, emotions[index]);
		}

		if ( arguments->all != 1 ) {
			printf("*** <ENTER> continute | Quit (q)");
			if ( getc( stdin ) == 'q' ) {
				exit(1);
			}
		}

		set_ptr = set_ptr->next;
	} // END while ( set_ptr->next != NULL ) 
}

