#include <stdio.h>

int main(int argc, char const *argv[])
{
	FILE *fp;
	char buffer[80];
	int index;

	// open read only
	fp = fopen("Beispieldatei/E_013.csv","r");
	if (fp == NULL) {
		perror( "Error opening file" );
	} else {
		// loop thru file til EOF 
		while ( fscanf(fp, "%s", buffer) != EOF ) {
			printf("%s\n", buffer);
		}
	}

	return 0;
}