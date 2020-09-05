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

/* keywords */
BOOL "true"|"false"
TIPO "float"|"int"|"char"|"bool"|"void"|"tuple"
CONDICOES "if"|"else"
LACOS "while"|"for"
RETORNO "return"
PARAM_TIPOS "void"|{TIPO}" "{ID}['['']']" "(','{TIPO}" "{ID}['['']'])*

/* tuple sendo declarada a partir da palavra reservada */
DECLARACAO {TIPO} {ID}(','{ID})*|"tuple" {TIPO}(','{TIPO})* ID

FUNC_DECL {TIPO}" "{ID}'('" "{PARAM_TIPOS}" "')'
/* COD_BLOCK IF'('EXPRESSION')'COD_BLOCK[ else COD_BLOCK] */
OP_ARITM "+"|"-"|"*"|"/"
OP_COMP "=="|"!="|"<="|"<"|">="|">"
OP_LOG "&&"|"||"
OP_ASSIGN "="
SCAN "scan"'('{DIGITO}','{ID}')'|"scan"'('{LETRA}','{ID}')'
PRINT "print"'('{ID}')'|"print"'('({LETRA}{DIGITO})*')'

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
{FUNC_DECL} {
    printf("FUNC_DECL %s (tamanho %d)\n", yytext, yyleng);
}
{OP_ARITM} {
    printf("OP_ARITM %s (tamanho %d)\n", yytext, yyleng);
}
{OP_COMP} {
    printf("OP_COMP %s (tamanho %d)\n", yytext, yyleng);
}
{OP_LOG} {
    printf("OP_LOG %s (tamanho %d)\n", yytext, yyleng);
}
{OP_ASSIGN} {
    printf("OP_ASSIGN %s (tamanho %d)\n", yytext, yyleng);
}
{SCAN} {
    printf("SCAN %s (tamanho %d)\n", yytext, yyleng);
}
{PRINT} {
    printf("PRINT %s (tamanho %d)\n", yytext, yyleng);
}
{ID} {
    printf("ID %s (tamanho %d)\n", yytext, yyleng);
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