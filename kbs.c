#include "lib/kbs.h"

int main(int argc, char const *argv[])
{
	FILE *fp;
	char buffer[80];
	struct set * start = NULL;
	struct set * observation = NULL;
	char * pch = NULL;
	int firstPtr = TRUE;
	struct exva exva;
	
	// open read only
	fp = fopen("Files/E_013_Testdatei.csv","r");
	if (fp == NULL) {
		perror( "Error opening file" );
		printf("*** Hit <Enter> to end the program\n");
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
			if ( !firstPtr ) {
				// use this condition to check if this is the
				// first iteration. Otherwise allocate new space
				// for next data
				// This guarantees that only so much space is allocated
				// as needed.
				// allocate new memory for next entry
				observation->next = (struct set*) malloc( sizeof(struct set) );
				observation->next->prev = observation;
				observation->next->next = NULL;
				observation = observation->next;
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

	calculate_extreme_values( &exva, start );
	calculate_evidence( &exva, start );

	return 0;
}
