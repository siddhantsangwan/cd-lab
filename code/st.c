#include "st.h"

int main(){
	FILE* input_file = fopen("test", "r");
	Symbol* table = make_table(input_file);
	print_table(table);
	fclose(input_file);
	return 0;
}
