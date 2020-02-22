#include "lexical_analyzer.h"

typedef struct symbol Symbol;
typedef struct tableEntry TableEntry;
typedef enum dataType DataType;
typedef enum scope Scope;

enum dataType{ TYPE_CHAR, TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, TYPE_VOID, TYPE_FUNC};
enum scope{ SCOPE_LOCAL, SCOPE_GLOBAL };

char type_strings[][10] = {"CHAR", "INT", "FLOAT", "DOUBLE", "VOID", "FUNC"};
char scope_strings[][10] = {"LOCAL", "GLOBAL"};
int type_size[] = {1, 4, 8, 16, 0, 0};

struct symbol{
	char* lexeme;
	Scope scope;
	DataType type, ret_type;
	int num_args, idx;
	TableEntry* args;
	Symbol *next, *prev;
};

struct tableEntry{
	int type, idx;
    TableEntry* next;
};

Symbol* make_symbol(int idx, char* lexeme, Scope scope, DataType type, DataType ret_type){
	Symbol* s = malloc(sizeof(Symbol));
	s->lexeme = malloc(30);
	strcpy(s->lexeme, lexeme);
	s->scope = scope; s->type = type; s->ret_type = ret_type;
	s->num_args = 0;
    s->args = NULL;
    s->idx = idx;
	s->next = s->prev = s->args = NULL;
}

TableEntry make_tableEntry(int type, int idx){
    TableEntry t = {type, idx, NULL};
    return t;
}

void insert_symbol(Symbol** table, Symbol** tail, Symbol* sym){
	sym->prev = *tail;
	if(*tail)
		(*tail)->next = sym;
	if(!*table) *table = sym;
	*tail = sym;
}

void add_args(Symbol* sym, TableEntry* arg){
    if(sym == NULL) return;
    TableEntry* tmp = sym->args;
    sym->num_args++;
    if(sym->args == NULL) sym->args = arg;
    else{
        while(tmp->next) tmp = tmp->next;
        tmp->next = arg;

    }
}

Symbol* make_table(FILE* input_file){
	int row=1, col=1, idx=1;
	int found_kword = 0, found_identifier=0, in_header=0;
	Token token;
	Symbol* table = NULL;
	Symbol* tail = table;
	DataType type, ret_type;
	Token identifier;
	Scope scope = SCOPE_GLOBAL;
    Analyzer a = make_analyzer(input_file);
    TableEntry* t = NULL;
	while(!feof(input_file)){
		token = get_next_token(&a);
		if(token.type == T_KEYWORD){
			found_kword = 1;
			if(!strcmp(token.lexeme, "int")){
				type = TYPE_INT;
			}
			else if(!strcmp(token.lexeme, "char")){
				type = TYPE_CHAR;
			}
			else if(!strcmp(token.lexeme, "float")){
				type = TYPE_FLOAT;
			}
			else if(!strcmp(token.lexeme, "double")){
				type = TYPE_DOUBLE;
			}
			else if(!strcmp(token.lexeme, "void")){
				type = TYPE_VOID;
			}
			else found_kword = 0;
		}
		else if(token.type == T_IDENTIFIER){
			identifier = token;
			found_identifier = 1;
		}
		else if(token.type == T_SPECIAL){
			if(token.lexeme[0] == '(' && found_kword && found_identifier){
				ret_type = type;
				type = TYPE_FUNC;
				Symbol* sym = make_symbol(idx++, identifier.lexeme, SCOPE_GLOBAL, type, ret_type);
				insert_symbol(&table, &tail, sym);
				scope = SCOPE_LOCAL;
				found_kword = found_identifier = 0;
                		in_header = 1;
			}
			else if((token.lexeme[0] == ';' || token.lexeme[0] == ',' || token.lexeme[0] == ')' || 
						token.lexeme[0] == '{' || token.lexeme[0] == '}') && found_kword && found_identifier){
				Symbol* sym = make_symbol(idx++, identifier.lexeme, scope, type, ret_type);
				insert_symbol(&table, &tail, sym);
				found_identifier = 0;
				if(token.lexeme[0] == ';' || token.lexeme[0] == ')' ) found_kword = 0;
                		if(in_header){
                		   //make table entry and add to table;
                		    t = malloc(sizeof(TableEntry));
                		    *t = make_tableEntry(sym->type, sym->idx);
                		    Symbol* tmp = tail;
                		    while(tmp && tmp->type != TYPE_FUNC){ tmp = tmp->prev; }
                		    add_args(tmp, t);
                		}
            			if(token.lexeme[0] == ')') in_header = 0;
			}
			else if(token.lexeme[0] == ')') in_header = 0;
		}
		else if(token.type == T_ASSIGNMENT && found_kword && found_identifier){
			Symbol* sym = make_symbol(idx++, identifier.lexeme, scope, type, ret_type);
			insert_symbol(&table, &tail, sym);
			found_kword = found_identifier = 0;
		}

	}
	return table;
}

void print_args(Symbol* sym){
    TableEntry* t = sym->args;
    while(t){
        printf("<%d,%s> ", t->idx, type_strings[t->type]);
        t = t->next;
    }
    printf("\n");
}

void print_table(Symbol* table){
	printf("Name\tType\tSize\tScope\tNum_args ret type\targs\n");
	while(table){
		if(table->type == TYPE_FUNC){
			printf("%s\t %s\t \t %s\t %d\t\t%s\t", table->lexeme, type_strings[table->type], scope_strings[table->scope], table->num_args, type_strings[table->ret_type]);
            print_args(table);
        }
		else
			printf("%s\t %s\t %d\t %s\t\n", table->lexeme, type_strings[table->type], type_size[table->type], scope_strings[table->scope]);
		table = table->next;
	}
}
