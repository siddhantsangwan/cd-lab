#include <stdio.h>
#include <stdlib.h>

int main (int argc, const char * argv []) {

	char name[128];
	printf("Enter input filename: ");
	scanf(" %s", name);

	FILE *input = fopen(name, "r");

	FILE *output = fopen("q2out.txt", "w");

	char ch;
	do {
		ch = getc(input); // Get character from input file.
		if (ch == '#') { // If a #if found, loop till you find the next '\n'
			while (ch != '\n') { 
				ch = getc(input);
			}
					}
		if(ch != EOF)
			putc(ch, output);
	} while (ch != EOF);

	fclose(input);
	fclose(output);

	return 0;

}