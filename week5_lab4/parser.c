#include "symbol_table.h"

int program(Analyzer* a);
int identifier_list(Analyzer* a);
int statement_list(Analyzer* a);
int statement(Analyzer* a);
int assign_statement(Analyzer* a);
int expression(Analyzer* a);
int expression_prime(Analyzer* a);
int simple_expression(Analyzer* a);
int simple_expression_prime(Analyzer* a);
int term(Analyzer* a);
int term_prime(Analyzer* a);
int factor(Analyzer* a);

int program(Analyzer* a)
{
    Token token = get_next_token(a);
    
    //check for int funct
    if (token.type == T_KEYWORD)
    {
        printf("%s", token.lexeme);
        token = get_next_token(a);

        if (token.type == T_IDENTIFIER)
        {
            printf(" %s\n", token.lexeme);
            while (1)
            {
                token = get_next_token(a);
                //printf("%s ", token.lexeme);
    
                if (token.type == T_SPECIAL)    //skip past the braces
                    continue;
                else
                {
                    //unread the first token after the braces, continue parsing in identifier_list
                    unread_word(token.lexeme, a);                    
                    break;
                }
            }
        }
        else
        {
            unread_word(token.lexeme, a);
            return 0;
        }        
    }
    else
    {
        unread_word(token.lexeme, a);
        return 0;
    }
    

    int flag = identifier_list(a);
    if (flag)   
    {
        if (statement_list(a))
            return 1;
        else
            return 1;
    }
    else
        return 0;
}

int identifier_list(Analyzer* a)
{
    Token token = get_next_token(a);
    int type = check_DataType(token);

    //if DataType
    if (token.type == T_KEYWORD && type > -1)
    {   
        printf("%s", token.lexeme);
        token = get_next_token(a);

        if (token.type == T_IDENTIFIER)
        {   
            printf(" %s", token.lexeme);
            //identifier|identifier_list
            Token token2 = get_next_token(a);
            if (strcmp(token2.lexeme, ",") == 0)
            {
                printf("%s", token2.lexeme);

                if (identifier_list(a))
                    return 1;
                else
                {   
                    unread_word(token2.lexeme, a);
                    return 0;
                }
            }
            else if (strcmp(token2.lexeme, ";") == 0)
            {
                return 1;
            }
        }
    }
    else if (token.type == T_IDENTIFIER)
    {
        printf(" %s", token.lexeme);
        token = get_next_token(a);
        if (strcmp(token.lexeme, ",") == 0)
        { 
            printf("%s", token.lexeme);
            if (identifier_list(a))
                return 1;
            else
                return 0;
        }
        else if (strcmp(token.lexeme, ";") == 0)
        {
            printf("%s\n", token.lexeme);
            return 1;
        }
        else
        {
            unread_word(token.lexeme, a);
            return 0;
        }
    }
    else if (strcmp(token.lexeme, ";") == 0)
    {
        printf("%s\n", token.lexeme);
        return 1;
    }
    else
    {
        unread_word(token.lexeme, a);
        return 0;
    }    
}

int statement_list(Analyzer* a)
{   
    if (statement(a))
    {
        if (statement_list(a))
            return 1;
        else
        {
            return 1;
        }
    }
    //epsilon
    else
    {
        return 1;
    }
}

int statement(Analyzer* a)
{
    if (assign_statement(a))
    {
        Token token = get_next_token(a);

        if (strcmp(token.lexeme, ";") == 0)
        {
            printf("%s\n", token.lexeme);
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

int assign_statement(Analyzer* a)
{   
    Token token = get_next_token(a);
    if (token.type == T_IDENTIFIER)
    {
        printf("%s", token.lexeme);
        token = get_next_token(a);
        if (strcmp(token.lexeme, "=") == 0)
        {
            printf(" %s", token.lexeme);

            if (expression(a))
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
    else
    {
        unread_word(token.lexeme, a);
        return 0;
    }
}

int expression(Analyzer* a)
{
    if (simple_expression(a))
    {
        if (expression_prime(a))
            return 1;
        else
            return 0;
    }
    else
        return 0;   
}

int expression_prime(Analyzer* a)
{
    Token token = get_next_token(a);
    if (token.type == T_LOGICAL_OPERATOR)
    {
        printf(" %s", token.lexeme);
        if (simple_expression(a))
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

int simple_expression(Analyzer* a)
{
    if (term(a))
    {
        if (simple_expression_prime(a))
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int simple_expression_prime(Analyzer* a)
{
    Token token = get_next_token(a);

    if (token.type == T_ARITHMETIC_OPERATOR)
    {
        printf(" %s", token.lexeme);
        if (term(a))
        {
            if (simple_expression_prime(a))
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }
    else 
    {
        unread_word(token.lexeme, a);
        return 1;
    }
}

int term(Analyzer* a)
{
    if (factor(a))
    {
        if (term_prime(a))
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int term_prime(Analyzer* a)
{
    Token token = get_next_token(a);
    if (token.type == T_ARITHMETIC_OPERATOR)
    {
        printf(" %s", token.lexeme);
        if (factor(a))
        {
            if (term_prime(a))
                return 1;
            else
            {   
                unread_word(token.lexeme, a);
                return 0;
            }
        }
        else
        {   
            unread_word(token.lexeme, a);
            return 0;
        }
    }
    unread_word(token.lexeme, a);
    return 1;
}

int factor(Analyzer* a)
{
    Token token = get_next_token(a);
    if (token.type == T_IDENTIFIER || token.type == T_CONST)
    {   
        printf(" %s", token.lexeme);
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
	int flag = program(&a);

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

 