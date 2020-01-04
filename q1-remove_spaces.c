#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	FILE* fa; FILE* fb;
	char name[20];

	printf("enter name of the file\n");
	scanf("%s", name);

	fa = fopen(name, "r");
	fb = fopen("q1out.txt", "w");
	
	if(fa == NULL)
	{
		printf("Cannot open file\n");
		exit(0);
	}

	char char1, char2;
	char1 = getc(fa);

	while(char1 != EOF)
	{
		if(char1 == ' ' || char1 == '\t')
		{
			while(char1 == ' ' || char1 == '\t')
				char1 = getc(fa);
			putc(' ', fb);
		}

		if(char1 != EOF)
			putc(char1, fb);
		char1 = getc(fa);
	}

	fclose(fa);
	fclose(fb);

	return 0;
}