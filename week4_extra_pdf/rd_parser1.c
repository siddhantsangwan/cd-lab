#include "symbol_table.h"

int S(Analyzer* a);
int T(Analyzer* a);

int S(Analyzer* a)
{
	Token token = get_next_token(a);

	if (token.type == T_IDENTIFIER || token.type == T_LOGICAL_OPERATOR)
	{
		printf("%s ", token.lexeme);
		return 1;
	}
	else if (strcmp(token.lexeme, "(") == 0)
	{
		printf("%s ", token.lexeme);
		int flag = T(a);
		
		if (flag)
		{
			token = get_next_token(a);

			if (strcmp(token.lexeme, ")") == 0)
			{
				printf("%s ", token.lexeme);
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;
}

int T(Analyzer* a)
{
	Token token;

	if (S(a))
	{
		token = get_next_token(a);
		if (strcmp(token.lexeme, ",") == 0)
		{
			printf("%s ", token.lexeme);
			if (T(a))
				return 1;
		}
		else
		{
			unreadc(token.lexeme[0], a);
			return 1;
		}
	}	
	else
	{
		return 0;
	}
	
}

int parse(FILE* inp_file)
{
	Analyzer a = make_analyzer(inp_file);

	printf("Parse results:\n");
	int flag = S(&a);

	return flag;
}

int main()
{
	char* file_name;
	printf("Enter name of the input file:\n");
	scanf("%s", file_name);
	FILE* inp_file = fopen(file_name, "r");
	// Analyzer a = make_analyzer(inp_file);
	// Token lex = get_next_token(&a);

	int message = parse(inp_file);
	if(message)
		printf("\nSuccessfully parsed.\n");
	else
		printf("\nUnsuccessful.\n");

	return 0;
}