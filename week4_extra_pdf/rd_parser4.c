#include "symbol_table.h"

int S (Analyzer* a);
int A (Analyzer* a);
int A_prime (Analyzer* a);
int B (Analyzer* b);

int S(Analyzer* a)
{
    Token token = get_next_token(a);
    if (strcmp(token.lexeme, "a") == 0)
    {
        printf("%s", token.lexeme);
        if (A(a))
        {
            token = get_next_token(a);
            if (strcmp(token.lexeme, "c") == 0)
            {
                printf("%s", token.lexeme);
                if (B(a))
                {
                    token = get_next_token(a);
                    if (strcmp(token.lexeme, "e") == 0)
                    {
                        printf("%s", token.lexeme);
                        return 1;
                    }
                    else
                    {
                        unread_word(token.lexeme, a);
                        return 0;
                    }
                }
                else
                    return 0;
            }
            else
            {
                unread_word(token.lexeme, a);
                return 0;
            }
        }
        else
            return 0;
    }
    else
    {
        unread_word(token.lexeme, a);
        return 0;
    }
}

int A(Analyzer* a)
{
    Token token = get_next_token(a);
    if (strcmp(token.lexeme, "b") == 0)
    {
        printf("%s", token.lexeme);
        if (A_prime(a))
            return 1;
        else
            return 0;
    }
    else
    {
        unread_word(token.lexeme, a);
        return 0;
    }
}

int A_prime(Analyzer* a)
{
    Token token = get_next_token(a);
    if (strcmp(token.lexeme, "b") == 0)
    {
        printf("%s", token.lexeme);
        if (A_prime(a))
            return 1;
        else
            return 0;
    }
    else
    {
        unread_word(token.lexeme, a);
        return 1;
    }
}

int B(Analyzer* a)
{
    Token token = get_next_token(a);
    if (strcmp(token.lexeme, "d") == 0)
    {
        printf("%s", token.lexeme);
        return 1;
    }
    else
    {
        unread_word(token.lexeme, a);
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