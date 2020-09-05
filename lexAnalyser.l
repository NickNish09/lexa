%{
    /* definitions of manifest constants
    LT, LE, EQ, NE, GT, GE,
    IF, THEN, ELSE, ID, NUMBER, RELOP */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

%}

DELIM [ \t\n]
ESPACO {DELIM}+
SEPARADOR [;{}()]
LETRA [A-Za-z]
DIGITO [0-9]
INT "-"?{DIGITO}+
FLOAT "-"?{DIGITO}+("."{DIGITO}+)*
ID {LETRA}({LETRA}|{DIGITO})*
VAR_DECL {ID}

/* keywords */
BOOL "true"|"false"
TIPO "float"|"int"|"char"|"bool"|"void"
CONDICOES "if"|"else"
LACOS "while"|"for"
RETORNO "return"
PARAM_TIPOS "void"|{TIPO}" "{ID}['['']']" "(','{TIPO}" "{ID}['['']'])*

/* tuple sendo declarada a partir da palavra reservada */
DECLARACAO {TIPO} {VAR_DECL}(','{VAR_DECL})*|"tuple" {TIPO}(','{TIPO})* VAR_DECL

FUNC_DECL {TIPO} {ID}'(' {PARAM_TIPOS} ')'
/* COD_BLOCK IF'('EXPRESSION')'COD_BLOCK[ else COD_BLOCK] */
OP_ARITM "+"|"-"|"*"|"/"
OP_COMP "=="|"!="|"<="|"<"|">="|">"
OP_LOG "&&"|"||"
OP_ASSIGN "="
SCAN "scan"'('{DIGITO}','{VAR_DECL}')' | "scan"'('{LETRA}','{VAR_DECL}')'
PRINT "print"'('{VAR_DECL}')' | "print"'('({LETRA}{DIGITO})*')'

%%

{DELIM} {
    printf("DELIM %s (tamanho %d)\n", yytext, yyleng);
}
{ESPACO} {
    printf("ESPACO %s (tamanho %d)\n", yytext, yyleng);
}
{SEPARADOR} {
    printf("SEPARADOR %s (tamanho %d)\n", yytext, yyleng);
}
{LETRA} {
    printf("LETRA %s (tamanho %d)\n", yytext, yyleng);
}
{DIGITO} {
    printf("DIGITO %s (tamanho %d)\n", yytext, yyleng);
}
{INT} {
    printf("INT %s (tamanho %d)\n", yytext, yyleng);
}
{FLOAT} {
    printf("FLOAT %s (tamanho %d)\n", yytext, yyleng);
}
{BOOL} {
    printf("BOOL %s (tamanho %d)\n", yytext, yyleng);
}
{TIPO} {
    printf("TIPO %s (tamanho %d)\n", yytext, yyleng);
}
{PARAM_TIPOS} {
    printf("PARAM_TIPOS %s (tamanho %d)\n", yytext, yyleng);
}
{ID} {
    printf("ID %s (tamanho %d)\n", yytext, yyleng);
}
{VAR_DECL} {
    printf("VAR_DECL %s (tamanho %d)\n", yytext, yyleng);
}

. {
    printf("Erro \n");
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
}