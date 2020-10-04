%{
#include <stdio.h>


%}

%token OP_ARITM OP_COMP OP_LOG OP_ASSIGN
%token BOOL
%token TIPO
%token CONDICOES
%token LACOS
%token RETORNO
%token INT FLOAT
%token ID

%start programa

%%
programa: 
  declaracao ';'
| func_decl
;
declaracao:
  declaracao var_decl
| var_decl
| TIPO ',' declaracao
| "tuple" declaracao 
;

var_decl:
  TIPO ID
;

func_decl:
  TIPO ID '(' parm_tipos ')'
| TIPO ID '(' ')'
;

parm_tipos:
  parm_tipos TIPO ID
| parm_tipos TIPO ID '[' ']'
| TIPO ID
| TIPO ID '[' ']'