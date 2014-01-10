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

void printSet( struct set * obs);

int main(int argc, char const *argv[])
{
	FILE *fp;
	char buffer[80];
	struct set * observation;
	struct set * temp;
	char * pch;

	// open read only
	fp = fopen("Beispieldatei/E_013.csv","r");
	if (fp == NULL) {
		perror( "Error opening file" );
	} else {
		observation = (struct set*) malloc( sizeof(struct set) );
		if (observation == NULL) exit (1);
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
			// printSet(observation);

			// allocate new memory for next entry
			temp = (struct set*) malloc( sizeof(struct set) );
			observation->next = temp;
			temp->prev = observation;
			temp->next = NULL;
			observation = temp;
		}
	}

	do {
		observation = observation->prev;
	} while ( observation->prev != NULL );
	while ( observation->next != NULL ) {
		printSet( observation );
		observation = observation->next;
	}

	return 0;
}

void printSet( struct set * obs) {
	printf("---Set #%i---\n", obs->frame);
	printf("Eye: %i\n", obs->eye);
	printf("Forehead: %i\n", obs->forehead);
	printf("Mouth: %i\n", obs->mouth);
	printf("\n");
}