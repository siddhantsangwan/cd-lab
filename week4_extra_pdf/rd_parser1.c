#include "symbol_table.h"

int S(Analyzer* a);
int T(Analyzer* a);

int S(Analyzer* a)
{
	Token token = get_next_token(a);

	if (token.type == T_IDENTIFIER)
	{
		printf("%s ", token.lexeme);
		return 1;
	}
	else if (token.type == T_LOGICAL_OPERATOR)	
	{
		printf("%s\n", token.lexeme);
		return 1;
	}
	else if (token.type == T_SPECIAL)
	{
		printf("%s\n", token.lexeme);
		int flag = T(a);
		
		if (flag)
		{
			token = get_next_token(a);
			if (token.type == T_SPECIAL)
				return 1;
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
	// Token token = get_next_token(a);
	// printf("Debug: In T: Got next token\n");

	// int flag = 0;

	// //recursively check for T,S
	// if (T(a))
	// {
	// 	printf("Debug: Checking T\n");
	// 	token = get_next_token(a);
	// 	if (token.type == T_SPECIAL)
	// 	{
	// 		printf("Debug: Comma encountered\n");
	// 		if (S(a))
	// 			return 1;
	// 		else
	// 			return 0;
	// 	}
	// 	else
	// 		return 0;
	// }
	// else if (S(a))
	// {
	// 	printf("Debug: Checking S\n");
	// 	return 1;
	// }
	// else
	// 	return 0;
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