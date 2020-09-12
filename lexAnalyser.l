%{
    /* definitions of manifest constants
    LT, LE, EQ, NE, GT, GE,
    IF, THEN, ELSE, ID, NUMBER, RELOP */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define SYM_SIZE 100
#define SYM_TABLE_SIZE 700

int errors = 0;
int error_index = 0;
int col = 1; int lin = 1;
char bad_pattern[200];
int bad_pattern_index = 0;
int bad_pattern_bool = 0;
int symbol_table_index = 1;

struct lerror {
    char type[50];
    char msg[100];
    char sym[SYM_SIZE];
    int line;
};

struct lerror err[SYM_TABLE_SIZE];

struct symbol_item {
    char sym[SYM_SIZE];
    int position;
};

struct symbol_item symbol_table[SYM_TABLE_SIZE];

void printErrors(){
    int i;
    for(i=0; i<errors; i++){
        printf("\nErro número %d\n", i+1);
        printf("Simbolo: %s\n", err[i].sym);
        printf("%s\n", err[i].type);
        printf("%s\n", err[i].msg);
        printf("Linha: %d\n", err[i].line);
        printf("\n");
    }
};

int pushSymTable(char symbol[]){
    strcpy(symbol_table[symbol_table_index].sym,symbol);
    symbol_table[symbol_table_index].position = symbol_table_index;
    symbol_table_index++;
    printf("%s adicionado à tabela\n", symbol);
    return symbol_table_index;
};

void printSymTable(){
    printf("\n** -- Tabela de Símbolos: -- **\n");
    int i;
    for(i=0; i<symbol_table_index; i++){
        printf("%d | %s \n", symbol_table[i].position, symbol_table[i].sym);
    }
};

%}
%x COMENTARIO
%x INTEIRO
DELIM [ \t]
ENTER [\n]
ESPACO {DELIM}+
SEPARADOR [;{}(),\[\]]
LETRA [A-Za-z]
DIGITO [0-9]
INT "-"?{DIGITO}+
FLOAT "-"?{DIGITO}+("."{DIGITO}+)*
ID {LETRA}({LETRA}|{DIGITO})*
VAR_DECL {ID}['['({DIGITO})+']']

/* keywords */
BOOL "true"|"false"
TIPO "float"|"int"|"char"|"bool"|"void"|"tuple"
CONDICOES "if"|"else"
LACOS "while"|"for"
RETORNO "return"
PARAM_TIPOS "void"|{TIPO}" "{ID}['['']']" "(','{TIPO}" "{ID}['['']'])*

/* tuple sendo declarada a partir da palavra reservada */
DECLARACAO {TIPO} {VAR_DECL}(','{VAR_DECL})*|"tuple" {TIPO}(','{TIPO})* VAR_DECL

FUNC_DECL {TIPO}" "{ID}'('" "{PARAM_TIPOS}" "')'
/* COD_BLOCK IF'('EXPRESSION')'COD_BLOCK[ else COD_BLOCK] */
OP_ARITM "+"|"-"|"*"|"/"
OP_COMP "=="|"!="|"<="|"<"|">="|">"
OP_LOG "&&"|"||"
OP_ASSIGN "="
SCAN "scan"'('{DIGITO}','{VAR_DECL}')'|"scan"'('{LETRA}','{VAR_DECL}')'
PRINT "print"'('{VAR_DECL}')'|"print"'('({LETRA}{DIGITO})*')'

%%

"/*"            { BEGIN(COMENTARIO); }
<COMENTARIO>"*/" { BEGIN(INITIAL); }
<COMENTARIO>\n   { }
<COMENTARIO>.    { }

{DELIM} {
    
}
{ESPACO} {
    printf("<ESPACO, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{SEPARADOR} {
    printf("<SEPARADOR, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{BOOL} {
    printf("<BOOL, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{TIPO} {
    printf("<TIPO, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{PARAM_TIPOS} {
    printf("<PARAM_TIPOS, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{FUNC_DECL} {
    printf("<FUNC_DECL, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{OP_ARITM} {
    printf("<OP_ARITM, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{OP_COMP} {
    printf("<OP_COMP, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{OP_LOG} {
    printf("<OP_LOG, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{OP_ASSIGN} {
    printf("<OP_ASSIGN, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{SCAN} {
    printf("<SCAN, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{PRINT} {
    printf("<PRINT, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{VAR_DECL} {
    printf("<VAR_DECL, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{ID} {
    int address = pushSymTable(yytext);
    printf("<ID, %d> (tamanho %d)\n", address, (int)yyleng);
}
{INT} {
    BEGIN(INTEIRO);
    printf("<INT, %s> (tamanho %d)\n", yytext, (int)yyleng);
    strcpy(bad_pattern, yytext);
    bad_pattern_index = (int)yyleng;
}
<INTEIRO>{DIGITO} { }
<INTEIRO>{DELIM} { 
    BEGIN(INITIAL);
    if(bad_pattern_bool){
        errors++;
        strcpy(err[error_index].type, "fpat");
        strcpy(err[error_index].msg, "Padrão não pertence à gramática");
        strcpy(err[error_index].sym, bad_pattern);
        err[error_index].line = lin;
        error_index++;
    }
    bad_pattern_bool = 0;
    strcpy(bad_pattern, "");
    bad_pattern_index = 0;
}
<INTEIRO>{ENTER} { 
    BEGIN(INITIAL);
    if(bad_pattern_bool){
        errors++;
        strcpy(err[error_index].type, "fpat");
        strcpy(err[error_index].msg, "Padrão não pertence à gramática");
        strcpy(err[error_index].sym, bad_pattern);
        err[error_index].line = lin;
        error_index++;
    }
    bad_pattern_bool = 0;
    strcpy(bad_pattern, "");
    bad_pattern_index = 0;
}
<INTEIRO>{SEPARADOR} { 
    BEGIN(INITIAL);
    if(bad_pattern_bool){
        errors++;
        strcpy(err[error_index].type, "fpat");
        strcpy(err[error_index].msg, "Padrão não pertence à gramática");
        strcpy(err[error_index].sym, bad_pattern);
        err[error_index].line = lin;
        error_index++;
    }
    bad_pattern_bool = 0;
    strcpy(bad_pattern, "");
    bad_pattern_index = 0;
}
<INTEIRO>. { 
    bad_pattern[bad_pattern_index++] = *yytext;
    bad_pattern_bool = 1;
 }
{FLOAT} {
    printf("<FLOAT, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{ENTER} {
    lin++;
    col = 1;
}
. {
    errors++;
    strcpy(err[error_index].type, "nsym");
    strcpy(err[error_index].msg, "Simbolo não pertence à gramática");
    strcpy(err[error_index].sym, yytext);
    err[error_index].line = lin;
    error_index++;
}

%%

int main( int argc, char **argv ) {
    ++argv, --argc;
    if( argc > 0 ){
        yyin = fopen( argv[0], "r" );
    }
    else{
        yyin = stdin;
    }

    yylex();
    printErrors();
    printSymTable();
}