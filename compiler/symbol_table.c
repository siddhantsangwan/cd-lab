#include "symbol_table.h"

int main(){
	FILE* input_file = fopen("program.c", "r");
	Symbol* table = make_table(input_file);
	print_table(table);
	fclose(input_file);
	return 0;
}
