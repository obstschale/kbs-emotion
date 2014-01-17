#include "kbs.h"


int main(int argc, char const *argv[])
{
	FILE *fp;
	char buffer[80];
	struct set * start;
	struct set * observation;
	struct set * temp;
	char * pch;

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
			// save data in sets
			pch = strtok( buffer, ";" );
			observation->frame = atoi( pch );
			// printf( "%i\n", observation->frame );
			pch = strtok( NULL, ";" );
			observation->eye = atoi( pch );
			pch = strtok( NULL, ";" );
			observation->forehead = atoi( pch );
			pch = strtok( NULL, ";" );
			observation->mouth = atoi( pch );
			// print_set(observation);

			// allocate new memory for next entry
			temp = (struct set*) malloc( sizeof(struct set) );
			observation->next = temp;
			temp->prev = observation;
			temp->next = NULL;
			observation = temp;
		}
	}

	// show_all_sets( start );
	return 0;
}