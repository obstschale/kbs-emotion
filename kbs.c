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
	struct set * start;
	struct set * observation;
	struct set * temp;
	char * pch;
	int firstPtr = TRUE;
	float eye_max, mouth_max, forehead_max;
	float eye_min, mouth_min, forehead_min;

	// open read only
	fp = fopen("Files/E_013_Beispieldatei.csv","r");
	if (fp == NULL) {
		perror( "Error opening file" );
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

			// save data in sets
			pch = strtok( buffer, ";" );
			observation->frame = atof( pch );
			pch = strtok( NULL, ";" );
			observation->eye = atof( pch );
			pch = strtok( NULL, ";" );
			observation->forehead = atof( pch );
			pch = strtok( NULL, ";" );
			observation->mouth = atof( pch );
			// print_set(observation);

			// First data is saved
			// so next data is not the
			// first anymore
			firstPtr = FALSE;
		}
	}

	// show_all_sets( start );

	// find min and max values of eye, mouth, and forehead
	eye_max = find_max_value( start, "eye" );
	eye_min = find_min_value( start, "eye" );

	mouth_max = find_max_value( start, "mouth" );
	mouth_min = find_min_value( start, "mouth" );

	forehead_max = find_max_value( start, "forehead" );
	forehead_min = find_min_value( start, "forehead" );

	// printf("max: %5.3f\nmin: %5.3f\n", eye_max, eye_min);
	// printf("max: %5.3f\nmin: %5.3f\n", mouth_max, mouth_min);
	// printf("max: %5.3f\nmin: %5.3f\n", forehead_max, forehead_min);

	// @TODO: calculate entry values for each pair

	// {Angst, Ueberraschung, Verachtung, Ekel, Wut}
	const int entry1[5] = {0,0,1,1,0}; // Augen
	const int entry2[5] = {0,0,0,0,1}; // Stirn
	const int entry3[5] = {0,0,0,1,1}; // Mund

	basicMeasure m1, m2, m3, *res1, *res2;
	set *p1, *p2, *p3;
	p1 = createAlternatives((int *) entry1, 5);
	p2 = createAlternatives((int *) entry2, 5);
	p3 = createAlternatives((int *) entry3, 5);
	createBasicMeasure(&m1,5);
	addMeasureEntry(&m1,*p1,0.714);
	printBasicMeasure(&m1);

	createBasicMeasure(&m2,5);
	addMeasureEntry(&m2,*p2,0.904);
	printBasicMeasure(&m2);
	printf("accumulate ...\n");

	res1 = getAccumulatedMeasure(&m1,&m2);
	printBasicMeasure(res1);

	createBasicMeasure(&m3,5);
	addMeasureEntry(&m3,*p3,0.866);
	printBasicMeasure(&m3);
	printf("accumulate ...\n");

	res2 = getAccumulatedMeasure(res1,&m3);
	printBasicMeasure(res2);
	printf(  " Nr : Pl(x)  |  B(x)   |  Z(x) \n");
  
  for (i=0;i<5;i++)
  {
    printf("[%d] : %5.3f  |  %5.3f  | %5.3f \n", i, plausibility(res2,i), singleBelief(res2,i), singleDoubt(res2,i));
  }
	return 0;
}