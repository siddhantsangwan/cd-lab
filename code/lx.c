#include "lx.h"

void print_token(Token t){
	printf("lexeme: %s\trow: %d\tcol: %d\ttype: %s\n", t.lexeme, t.row, t.col, token_names[t.type]);
}

int main(){
	FILE *inp = fopen("test", "r");
	Analyzer a = make_analyzer(inp);
	Token lex = get_next_token(&a);
	while(lex.type != T_EOF){
		print_token(lex);
		lex = get_next_token(&a);
	}
	fclose(inp);
	return 0;
}
