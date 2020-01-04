#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE* fa; FILE* fb;
	char name[20];

	printf("enter name of the file\n");
	scanf("%s", name);

	fa = fopen(name, "r");
	fb = fopen("q4out.txt", "w");
	
	if(fa == NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}

	char char1, space = ' ';
	char1 = getc(fa);
	int count = 1;
	fprintf(fb, "%d ", count);

	while(char1 != EOF)
	{
		if(char1 == '\n')
		{
			count++;
			fputc(char1, fb);
			fprintf(fb, "%d ", count);
		}
		if(char1 != '\n')
			fputc(char1, fb);
		char1 = getc(fa);
	}

	fclose(fa);
	fclose(fb);

	return 0;
}