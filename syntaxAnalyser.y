%error-verbose
%debug
%locations

%{
  #include <stdlib.h>
  #include <stdio.h>
  
  int yylex();
  extern int lin;
  void yyerror(const char* msg) {
    fprintf(stderr, "ERRO na linha %d: %s\n", lin, msg);
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
  declaracoes { $$ = $1; printf("kkk %d\n", $1); }
;

declaracoes:
  declaracoes declaracao
| declaracao
;

declaracao:
  var_decl { printf("\nvar_decl\n"); }
| "tuple" declaracao  { printf("tuple\n"); }
| func_decl
;

var_decl:
  TIPO ID ';' { printf("TIPO ID %s %s", $1, $2); }
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