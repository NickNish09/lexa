%error-verbose
%debug
%locations

%{
  #include <stdlib.h>
  #include <stdio.h>
  
  int yylex();
  void yyerror(const char* msg) {
    fprintf(stderr, "%s\n", msg);
  }
  extern FILE *yyin;

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
%token SEPARADOR

%start programa

%%
programa: 
  declaracao { $$ = $1; printf("kkk %d", $1); }
| func_decl
;
declaracao:
  declaracao var_decl
| var_decl { printf("var_decl\n"); }
| TIPO ',' declaracao { printf("TIPO\n"); }
| "tuple" declaracao  { printf("tuple\n"); }
;

var_decl:
  TIPO ID ';'
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
| RETORNO SEPARADOR
| RETORNO expressao SEPARADOR
| assign SEPARADOR
| ID '(' expressao ')' SEPARADOR
| ID '(' ')' SEPARADOR
| scan
| print
;

assign:
  ID OP_ASSIGN expressao
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

int main(int argc, char **argv){
  ++argv, --argc;
  if( argc > 0 ){
      yyin = fopen( argv[0], "r" );
  }
  else{
      yyin = stdin;
  }

  yylex();
  // printErrors();
  // printSymTable();
  yyparse();

  return 0;
}