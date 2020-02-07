#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "types.h"

typedef struct token Token;
typedef struct analyzer Analyzer;
typedef enum tokenType TokenType;

struct token{
    char* lexeme;
    TokenType type;
    int row, col;
};

struct analyzer{
    FILE* fp;
    int row, col;
};

Analyzer make_analyzer(FILE* ip){
    Analyzer a = {ip, 1, 1};
    return a;
}

Token make_token(char* lexeme, int size, TokenType type, int row, int col){
    Token t = {NULL, type, row, col};
    t.lexeme = malloc(sizeof(char)*size);
    strcpy(t.lexeme, lexeme);
    return t;
}

int is_special_character(char ch){
    return strchr("(){}[];:,", ch) != NULL;
}

int is_arithmetic_operator(char ch){
    return strchr("+-*/%", ch) != NULL;
}

int is_logical_operator(char ch){
    return strchr("<>=!", ch) != NULL;
}

int is_keyword(char* word){
    char keywords[][10] = {"int", "float", "char", "void", "double", "long", "unsigned", "short", "if", "else", "while", "for", "do", "return", "sizeof",
    "typedef", "enum", "union", "asm", "break", "class"};
    int num_keywords = 22;
    for(int i=0;i<num_keywords; ++i) if(!strcmp(keywords[i], word)) return 1;
    return 0;
}

char readc(Analyzer* a){
    char ch = getc(a->fp);
    if(ch == '\n'){
        a->row++; a->col = 1;
    }
    else if(ch == '\t') a->col+=4;
    else if(ch != EOF) a->col++;
    return ch;
}

void unreadc(char ch, Analyzer* a){
    ungetc(ch, a->fp);
    if(ch == '\n') a->row--;
    else if(ch == '\t') a->col-=4;
    else a->col--;
}

void read_line(Analyzer* a){
    while( readc(a) != '\n' );
}

void read_comment(Analyzer* a){
    int state = 0; 
    char ch;
    while( (ch = readc(a)) != EOF ){
        if(ch == '*'){
            state = 1;
        }
        else if(state == 1 && ch == '/'){
            return;
        }
        else state = 0;
    }
}

void read_const(Analyzer* a, char* word, int* len){
    char ch = word[0];
    word[1] = 0;
    *len = 1;
    if( ch == '0' ){
        ch = readc(a);
        if(ch == 'x') word[(*len)++] = ch;
        else if(!isdigit(ch)) return;
    }
    while( (ch = readc(a)) != EOF){
        if(isdigit(ch)) word[(*len)++] = ch;
        else break;
    }
    unreadc(ch, a);
    word[*len] = 0;
}

void read_word(Analyzer* a, char* word, int* len){
    *len = 1;
    while((word[(*len)++] = readc(a))!=EOF) if(!isalnum(word[(*len)-1]) && word[*len-1] != '_') break;
    unreadc(word[--(*len)], a);
    word[*len] = 0;
}

void read_string(Analyzer* a){
    char ch;
    int state = 0;
    while( (ch = readc(a)) != EOF){
        if(ch == '"') return;
        else if(ch == '\\') ch = readc(a);
    }
}

Token get_next_token(Analyzer* a){
    char ch;
    char word[100];
    int len;
    while( (ch = readc(a)) != EOF ){
        if(is_special_character(ch)){
            word[0] = ch; word[1] = 0;
            return make_token(word, 2, T_SPECIAL, a->row, a->col-1);
        }
        else if(ch == '/'){
            ch = readc(a);
            if(ch == '/') read_line(a);
            else if(ch == '*') read_comment(a);
            else {
                unreadc(ch, a);
                return make_token("/", 2, T_ARITHMETIC_OPERATOR, a->row, a->col-1);
            }
        }
        else if(ch == '#') read_line(a);
        else if(ch == '='){
            word[0] = ch;
            ch = readc(a);
            if(ch == '='){
                word[1] = ch;
                word[2] = 0;
                return make_token(word, 3, T_LOGICAL_OPERATOR, a->row, a->col-2);
            }
            else {
                unreadc(ch, a);
                word[1] = 0;
                return make_token(word, 2, T_ASSIGNMENT, a->row, a->col-1);
            }
        }
        else if(is_logical_operator(ch)){
            word[0] = ch; word[1] = 0; word[2] = 0;
            ch = readc(a);
            if(is_logical_operator(ch)) word[1] = ch;
            else unreadc(ch, a);
            return make_token(word, 3, T_LOGICAL_OPERATOR, a->row, a->col-1);
        }
        else if(is_arithmetic_operator(ch)){
            word[0] = ch; word[1] = 0;
            return make_token(word, 2, T_ARITHMETIC_OPERATOR, a->row, a->col-1);
        }
        else if(ch == '"') read_string(a);
        else if(isdigit(ch)){
            word[0] = ch;
            read_const(a, word, &len);
            return make_token(word, len+1, T_CONST, a->row, a->col-len);
        }
        else if(isalpha(ch)){
            word[0] = ch;
            read_word(a, word, &len);
            if(is_keyword(word)) return make_token(word, len+1, T_KEYWORD, a->row, a->col-len);
            else return make_token(word, len+1, T_IDENTIFIER, a->row, a->col-len);
        }
    }
    return make_token("EOF", 4, T_EOF, a->row, a->col);
}
