#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// List of tokens
const char *keywords[32] = {
	"auto",
	"double",
	"int",
	"struct",
	"break",
	"else",
	"long",
	"switch",
	"case",
	"enum",
	"register",
	"typedef",
	"char",
	"extern",
	"return",
	"union",
	"continue",
	"for",
	"signed",
	"void",
	"do",
	"if",
	"static",
	"while",
	"default",
	"goto",
	"sizeof",
	"volatile",
	"const",
	"float",
	"short",
	"unsigned"
};

// list of delimiters
const char delimiters[] = " .,;:!-()\n\t";

int isKeyword (char *word) {
	int i;
	for (i = 0; i < 32; ++i) {
		if (strcmp(word, keywords[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void printUpperCase (char *word) {
	int l = strlen(word);
	char z;
	int i;
	for (i = 0; i < l; ++i) {
		z = word[i];
		printf("%c", z > 96 ? z - 32 : z);
	}
	printf("\n");
}

int main (int argc, const char * argv []) {

	char name[128];
	printf("Enter input filename: ");
	scanf(" %s", name);

	FILE *f1 = fopen(name, "r");

	char buffer[1024];
	int k;

	while (fgets(buffer, 1024, f1) > 0) {

		// Make a temp copy of the string
		char *temp = (char *)malloc(1024 * sizeof(char));
		strcpy(temp, buffer);
			
		char *token = (char *)malloc(256 * sizeof(char));

		do {
			int count = 1;
			token = strsep(&temp, delimiters);
			
			// if(strcmp(token, "\n") == 0)
			// 	count++;

			if (token != NULL) {
				if (isKeyword(token)) {
					printUpperCase(token);
				}
			}

		} while (token != NULL);
	}

	fclose(f1);

	return 0;

}