#include "lib/kbs.h"
#include "lib/dempster/dempster.c"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

int main(int argc, char const *argv[])
{
	FILE *fp;
	int i;
	char buffer[80];
	struct set * start = NULL;
	struct set * observation = NULL;
	struct set * temp = NULL;
	char * pch = NULL;
	int firstPtr = TRUE;
	float eye_max, mouth_max, forehead_max;
	float eye_min, mouth_min, forehead_min;
	float eye_max_adjust, mouth_max_adjust, forehead_max_adjust;
	float eye_value, mouth_value, forehead_value;
	float pl[5];
	char emotions[5][10] = {"fear", "surprise", "contempt", "disgust", "fury"};

	// open read only
	fp = fopen("Files/E_013_Testdatei.csv","r");
	if (fp == NULL) {
		perror( "Error opening file" );
		printf("~~~Hit <Enter> to end the program~~~\n");
		getc( stdin );
		return 0;
	} else {
		start = (struct set*) malloc( sizeof(struct set) );
		if (start == NULL) exit (1);
		observation = start;
		observation->prev = NULL;
		observation->next = NULL;

		// Read first line to skip titles
		// loop start with first line of numbers
		fscanf(fp, "%s", buffer);

		// loop thru file til EOF
		while ( fscanf(fp, "%s", buffer) != EOF ) {
			// use this condition to check if this is the
			// first iteration. Otherwise allocate new space
			// for next data
			// This guarantees that only so much space is allocated
			// as needed.
			if ( !firstPtr ) {
				// allocate new memory for next entry
				temp = (struct set*) malloc( sizeof(struct set) );
				observation->next = temp;
				temp->prev = observation;
				temp->next = NULL;
				observation = temp;
			}

			// @TODO: replace , with . as decimal delimiter
			// save data in sets
			pch = strtok( buffer, ";" );
			observation->frame = atof( pch );
			pch = strtok( NULL, ";" );
			observation->eye = atof( pch );
			pch = strtok( NULL, ";" );
			observation->forehead = atof( pch );
			pch = strtok( NULL, ";" );
			observation->mouth = atof( pch );

			// First data is saved
			// so next data is not the
			// first anymore
			firstPtr = FALSE;
		}
	}

	observation = start;

	while ( observation->next != NULL ) {
		print_set( observation );
		// show_all_sets( start );

		// find min and max values of eye, mouth, and forehead
		eye_max = find_max_value( start, "eye" );
		eye_min = find_min_value( start, "eye" );
		eye_max_adjust = eye_max - eye_min;

		forehead_max = find_max_value( start, "forehead" );
		forehead_min = find_min_value( start, "forehead" );
		forehead_max_adjust = forehead_max - forehead_min;

		mouth_max = find_max_value( start, "mouth" );
		mouth_min = find_min_value( start, "mouth" );
		mouth_max_adjust = mouth_max - mouth_min;

		printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", eye_max, eye_min, eye_max_adjust);
		printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", forehead_max, forehead_min, forehead_max_adjust);
		printf("max: %5.3f\tmin: %5.3f\tadjusted: %5.3f\n", mouth_max, mouth_min, mouth_max_adjust);

		// @TODO: calculate entry values for each pair
		eye_value = observation->eye - eye_min;
		forehead_value = observation->forehead - forehead_min;
		mouth_value = observation->mouth - mouth_min;

		// percentage values
		float eye_per = eye_value / eye_max_adjust;
		float forehead_per = forehead_value / forehead_max_adjust;
		float mouth_per = mouth_value / mouth_max_adjust;
		// inverse percentage values
		float eye_per_inv = 1 - eye_per;
		float forehead_per_inv = 1 - forehead_per;
		float mouth_per_inv = 1 - mouth_per;

		printf("%5.3f %5.3f %5.3f\n", eye_value, forehead_value, mouth_value);
		printf("%5.3f %5.3f %5.3f\n", eye_per, forehead_per, mouth_per);
		printf("%5.3f %5.3f %5.3f\n", eye_per_inv, forehead_per_inv, mouth_per_inv);

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
		printf(  " Nr : Pl(x)  |  B(x)   |  Z(x) \n");

		for (i=0;i<5;i++) {
			// printf("[%d] : %5.3f  |  %5.3f  | %5.3f \n", i, plausibility(res2,i), singleBelief(res2,i), singleDoubt(res2,i));
			pl[i] = plausibility(res2,i);
		}

		double max_emotion = pl[0];
		int index = 0;
		for ( i = 1; i < 5; ++i) {
			max_emotion = (max_emotion > pl[i]) ? max_emotion : pl[i];
			index = (max_emotion > pl[i]) ? index : i;
		}

		printf("[%i] %5.3f %s\n", index, max_emotion, emotions[index]);
		
		printf("~~~Hit <Enter> to continue~~~\n");
		getc( stdin );
		observation = observation->next;
	}

	return 0;
}
