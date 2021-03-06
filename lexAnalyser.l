%option nounput
%option noinput

%{
    /* definitions of manifest constants
    LT, LE, EQ, NE, GT, GE,
    IF, THEN, ELSE, ID, NUMBER, RELOP */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "syntaxAnalyser.tab.h"
#define SYM_SIZE 100
#define SYM_TABLE_SIZE 700

int errors = 0;
int error_index = 0;
int col = 1; int lin = 1;
char bad_pattern[200];
int bad_pattern_index = 0;
int bad_pattern_bool = 0;
int symbol_table_index = 1;
int current_scope = 0; // global scope
int scope_stack_index = 0; // global scope

struct lerror {
    char type[50];
    char msg[100];
    char sym[SYM_SIZE];
    int line;
};

struct lerror err[SYM_TABLE_SIZE];

void printErrors(){
    printf("Erros léxicos:\n");
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

%}
%x COMENTARIO
%x INTEIRO
DELIM [ \t]
ENTER [\n]
ESPACO {DELIM}+
SEPARADOR [;{}(),\[\]]
ASPAS ["]
LETRA [A-Za-z]
DIGITO [0-9]
INT "-"?{DIGITO}+
FLOAT "-"?{DIGITO}+("."{DIGITO}+)*
ID {LETRA}({LETRA}|{DIGITO})*
VAR_DECL {ID}['['({DIGITO})+']']
PONTO [.]

/* keywords */
BOOL "true"|"false"
TIPO "float"|"int"|"char"|"bool"|"void"
TUPLE "tuple"
CONDICOES "if"|"else"
LACOS "while"|"for"
RETORNO "return"
/* COD_BLOCK IF'('EXPRESSION')'COD_BLOCK[ else COD_BLOCK] */
OP_ARITM "+"|"-"|"*"|"/"
OP_COMP "=="|"!="|"<="|"<"|">="|">"
OP_LOG "&&"|"||"
OP_ASSIGN "="
SCAN "scan"
PRINT "print"

%%

"/*"            { BEGIN(COMENTARIO); }
<COMENTARIO>"*/" { BEGIN(INITIAL); }
<COMENTARIO>\n   { }
<COMENTARIO>.    { }

{DELIM} {
    
}
{ESPACO} {
    // printf("<ESPACO, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{PONTO} {
    return PONTO;
}
{SEPARADOR} {
    // printf("<SEPARADOR, %s> (tamanho %d)\n", yytext, (int)yyleng);
    if(strcmp(yytext, "{") == 0){
        current_scope++;
        scope_stack_index++;
        return '{';
    } else if(strcmp(yytext, "}") == 0){
        scope_stack_index--;
        return '}';
    } else if(strcmp(yytext, ";") == 0){
        return ';';
    } else if(strcmp(yytext, "(") == 0){
        return '(';
    } else if(strcmp(yytext, ")") == 0){
        return ')';
    } else if(strcmp(yytext, "[") == 0){
        return '[';
    } else if(strcmp(yytext, "]") == 0){
        return ']';
    } else if(strcmp(yytext, ",") == 0){
        return ',';
    };
}
{BOOL} {
    // printf("<BOOL, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.tipo = (char *) strdup(yytext);
    return BOOL;
}
{LACOS} {
    // printf("<LACOS, %s> (tamanho %d)\n", yytext, (int)yyleng);
    return LACOS;
}
{RETORNO} {
    // printf("<RETORNO, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.str = (char *) strdup(yytext);
    return RETORNO;
}
{CONDICOES} {
    // printf("<CONDICOES, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.str = (char *) strdup(yytext);
    if(strcmp(yytext, "if") == 0){
        return IF;
    } else if(strcmp(yytext, "else") == 0){
        return ELSE;
    };
}
{TUPLE} {
    // printf("<TUPLE, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.tipo = (char *) strdup(yytext);
    return TUPLE;
}
{TIPO} {
    // printf("<TIPO, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.tipo = (char *) strdup(yytext);
    return TIPO;
}
{OP_ARITM} {
    // printf("<OP_ARITM, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.operador = (char *) strdup(yytext);
    return OP_ARITM;
}
{OP_COMP} {
    // printf("<OP_COMP, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.operador = (char *) strdup(yytext);
    return OP_COMP;
}
{OP_LOG} {
    // printf("<OP_LOG, %s> (tamanho %d)\n", yytext, (int)yyleng);
    return OP_LOG;
}
{OP_ASSIGN} {
    // printf("<OP_ASSIGN, %s> (tamanho %d)\n", yytext, (int)yyleng);
    return OP_ASSIGN;
}
{SCAN} {
    // printf("<SCAN, %s> (tamanho %d)\n", yytext, (int)yyleng);
    return SCAN;
}
{PRINT} {
    // printf("<PRINT, %s> (tamanho %d)\n", yytext, (int)yyleng);
    return PRINT;
}
{VAR_DECL} {
    // printf("<VAR_DECL, %s> (tamanho %d)\n", yytext, (int)yyleng);
}
{ID} {
    // printf("<ID, %d> (tamanho %d)\n", address, (int)yyleng);
    yylval.id = (char *) strdup(yytext);
    return ID;
}
{INT} {
    // BEGIN(INTEIRO);
    // printf("<INT, %s> (tamanho %d)\n", yytext, (int)yyleng);
    // strcpy(bad_pattern, yytext);
    // bad_pattern_index = (int)yyleng;
    yylval.str = (char *) strdup(yytext);
    return INT;
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
    // printf("<FLOAT, %s> (tamanho %d)\n", yytext, (int)yyleng);
    yylval.str = (char *) strdup(yytext);
    return FLOAT;
}
{ENTER} {
    lin++;
    col = 1;
}
{ASPAS} {
    // printf("<ASPAS, %s> (tamanho %d)\n", yytext, (int)yyleng);
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