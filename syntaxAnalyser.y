%{
  #include <stdio.h>

  void yyerror(const char* msg) {
    fprintf(stderr, "%s\n", msg);
  }
  int yylex();

%}

%token OP_ARITM OP_COMP OP_LOG OP_ASSIGN
%token BOOL
%token TIPO
%token CONDICOES
%token LACOS
%token RETORNO
%token INT FLOAT
%token ID
%token DIGITO LETRA

%start programa

%%
programa: 
  declaracao ';' { printf("%d", $1);}
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
| TIPO ID '(' parm_tipos ')' '{' cod_block '}'
| TIPO ID '(' ')' '{' cod_block '}'
;

parm_tipos:
  parm_tipos TIPO ID
| parm_tipos TIPO ID '[' ']'
| TIPO ID
| TIPO ID '[' ']'
;

cod_block:
  "if" '(' expressao ')' '{' cod_block '}'
| "if" '(' expressao ')' '{' cod_block '}' "else" '{' cod_block '}'
| "while" '(' expressao ')' '{' cod_block '}'
| RETORNO ';'
| RETORNO expressao ';'
| assign ';'
| ID '(' expressao ')' ';'
| ID '(' ')' ';'
| scan
| print
;

assign:
  ID '=' expressao
| ID '[' INT ']' OP_ASSIGN expressao
;

expressao:
  OP_ARITM expressao
| OP_LOG expressao
| '!' expressao
| expressao OP_ARITM expressao
| expressao OP_COMP expressao
| '(' expressao ')'
;

scan:
  "scan" '(' ID ')'
;

print:
  "print" '(' ID ')'
| "print" '(' palavra ')'
;

palavra:
  palavra LETRA
| palavra DIGITO
| LETRA
| DIGITO
;

%%

int _main() {
  yyparse();

  return 0;
}