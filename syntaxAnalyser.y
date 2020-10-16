%error-verbose
%debug
%locations

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include "uthash.h"
  #define SYMBOL_NODE 888
  #define REGULAR_NODE 889
  #define VARIABLE_TYPE 1001
  #define FUNCTION_TYPE 1002
  
  int yylex();
  extern int lin;
  void yyerror(const char* msg) {
    fprintf(stderr, "ERRO na linha %d: %s\n", lin, msg);
  }
  extern FILE *yyin;

  // HELPERS

  char* concat(const char *s1, const char *s2)
  {
      char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
      // in real code you would check for errors in malloc here
      strcpy(result, s1);
      strcat(result, s2);
      return result;
  }

  const char * stringBasedOnNumber(int number)
  {
    if(number == SYMBOL_NODE){
      return "Symbol Node";
    } else if(number == REGULAR_NODE){
      return "Regular Node";
    } else if(number == FUNCTION_TYPE){
      return "Function";
    } else if(number == VARIABLE_TYPE){
      return "Variable";
    }
    return "Not Found";
  }

  // REFERENTE A TABELA DE SIMBOLOS
  typedef struct s_node {
    char* id;
    char* var_type;
    int s_node_type;
    int scope; // 0 = global
    UT_hash_handle hh;
  } s_node;

  struct s_node *s_table = NULL;

  void add_to_s_table(char* id, char* var_type, int s_node_type, int scope){
    s_node *s;
    char scope_string[5];
    sprintf(scope_string, "%d", scope);
    char *auxid = concat("::", scope_string);
    char *identifier = concat(id, auxid);
    HASH_FIND_STR(s_table, identifier, s);
    if(s == NULL){ // variavel ainda nao esta na tabela
      s_node *s = (s_node *)malloc(sizeof *s);
      s->id = identifier;
      s->var_type = var_type;
      s->s_node_type = s_node_type;
      s->scope = scope;
      HASH_ADD_STR(s_table, id, s);
    }
  }

  void print_s_table() {
    s_node *s;

    printf("Tabela de Símbolos:\n");
    // printf("NAME\t\tTYPE\t\tSYMBOL_TYPE\t\tSCOPE SYMBOLS\n");
    for(s=s_table; s != NULL; s=s->hh.next) {
      printf("id: %10s | var_type: %7s | s_node_type: %10s | scope: %d\n", s->id, s->var_type, stringBasedOnNumber(s->s_node_type), s->scope);
    }
  }

  // END REFERENTE A TABELA DE SIMBOLOS

  typedef struct node {
    char* var_type; //
    char node_kind; // 'F' function, 'V' var, 'C' code_block..., 'D' declaration, 'E' expression
    int node_type; // 'S' for symbol | 'R' for regular
    struct node *left;
    struct node *right;
    struct node *middle;
    char *val;

  } node;

  node *parser_tree = NULL;

  /* Definitions 
    node
  */
node* ins_node(char* var_type, int node_type, char node_kind, node *left, node *right, char* node_val){
  node* aux_node = (node*)calloc(1, sizeof(node));

    aux_node->left = left;
    aux_node->right = right;
    aux_node->var_type = var_type;
    aux_node->node_type = node_type;
    aux_node->node_kind = node_kind;
    aux_node->val = node_val;

    return aux_node;
}

node* ins_node_symbol(char* var_type, int node_type, char node_kind, char* id){
  node* aux_node = (node*)calloc(1, sizeof(node));

  // printf("tipo_var: %s", var_type);
  aux_node->left = NULL;
  aux_node->right = NULL;
  aux_node->var_type = var_type;
  aux_node->val = id;
  aux_node->node_type = node_type;
  aux_node->node_kind = node_kind;

  int t = FUNCTION_TYPE;
  if(node_kind == 'V'){
    t = VARIABLE_TYPE;
  }
  add_to_s_table(id, var_type, t, 0);

  return aux_node;
}

  void print_tree(node * tree, int prof) {
    int j;
    for(j=0;j<prof;j++){
      printf("  ");
    }
    if (tree) {
      printf("| var_type: %s | kind: %c | type: %s | val: %s |\n",tree->var_type, tree->node_kind, stringBasedOnNumber(tree->node_type), tree->val);
      print_tree(tree->left, prof+1);
      print_tree(tree->right, prof+1);
    }
  }
%}

%union {
  char *id;
  char *tipo;
  char *operador;

  char *str;
  int itype;
  struct node* nd;
}

%type <nd> programa declaracoes declaracao var_decl func_decl parm_tipos cod_block assign expressao scan print
%type <nd> cod_blocks expressao_logica termo op_expressao declaracao_tupla
%type <str> palavra

%token <operador> OP_ARITM OP_COMP OP_LOG OP_ASSIGN
%token BOOL
%token <tipo> TIPO
%token CONDICOES
%token LACOS
%token <str> RETORNO
%token <str> INT FLOAT TUPLE
%token <id> ID
%token <str> DIGITO LETRA
%token SEPARADOR
%token PRINT SCAN

%left OP_ARITM
%left OP_LOG
%left OP_COMP

%%
programa: 
  declaracoes { parser_tree = $1; printf("tree initialized\n"); }
;

declaracoes:
  declaracoes declaracao { printf("declaracoes \n"); $$ = ins_node("x", REGULAR_NODE,'D', $1, $2, "decl"); }
| declaracao { printf("declaracao \n"); $$ = $1; }
;

declaracao:
  var_decl { printf("var_decl\n"); $$ = $1; }
| TUPLE declaracao_tupla { printf("tuple_decl\n"); $$ = $2; }
| func_decl { printf("func_decl\n"); $$ = $1; }
;

declaracao_tupla:
  TIPO ',' declaracao_tupla { printf("declaracao_tupla #1"); $$ = $3; }
| var_decl { printf("declaracao_tupla #2"); $$ = $1;}
;

var_decl:
  TIPO ID ';' { printf("var_decl \n"); $$ = ins_node_symbol($1, SYMBOL_NODE,'V', $2); }
;

func_decl:
  TIPO ID '(' parm_tipos ')' ';'{ printf("func_decl #1 \n"); $$ = ins_node_symbol($1, SYMBOL_NODE,'F', $2); }
| TIPO ID '(' ')' ';' { printf("func_decl #2 \n"); $$ = ins_node_symbol($1, SYMBOL_NODE,'F', $2); }
| TIPO ID '(' parm_tipos ')' '{' cod_blocks '}' { printf("func_decl #3 \n"); $$ = $7; }
| TIPO ID '(' ')' '{' cod_blocks '}' { printf("func_decl #4 \n"); $$ = $6}
;

parm_tipos:
  parm_tipos TIPO ID { printf("parm_tipos #1 \n"); }
| parm_tipos TIPO ID '[' ']' { printf("parm_tipos #2 \n"); }
| TIPO ID { printf("parm_tipos #3 \n"); }
| TIPO ID '[' ']' { printf("parm_tipos #4 \n"); }
;

cod_blocks:
  cod_blocks cod_block { printf("cod_blocks #1\n"); $$ = ins_node("x", REGULAR_NODE,'C', $1, $2, "cb"); }
| cod_block  { printf("cod_blocks #2\n"); $$ = $1; }
;

cod_block:
  "if" '(' expressao_logica ')' '{' cod_blocks '}' { printf("cod_block #1 \n"); }
| "if" '(' expressao_logica ')' '{' cod_blocks '}' "else" '{' cod_blocks '}' { printf("cod_block #2 \n"); }
| LACOS '(' expressao_logica ')' '{' cod_block '}' { printf("cod_block #3 \n"); $$ = ins_node("x", REGULAR_NODE,'L', $3, $6, "while") }
| RETORNO ';' { printf("cod_block #4 \n"); $$ = NULL }
| RETORNO termo ';' { printf("cod_block #4.5 \n"); $$ = ins_node("x", REGULAR_NODE,'R', NULL, $2, "retorno")}
| RETORNO '(' expressao ')' ';' { printf("cod_block #5 \n"); $$ = ins_node("x", REGULAR_NODE,'R', NULL, $3, "retorno") }
| assign ';' { printf("cod_block #6 \n"); }
| print { printf("cod_block #7 \n"); $$ = $1}
| ID '(' expressao ')' ';' { printf("cod_block #8 \n"); $$ = $3}
| ID '(' ')' ';' { printf("cod_block #9 \n"); $$ = ins_node("x", 'C','R', NULL, NULL, "call")}
| scan '(' ID ')' ';' { printf("cod_block #10 \n"); }
| declaracoes { $$ = $1 }
;

assign:
  ID OP_ASSIGN expressao { printf("assign #1 \n"); }
| ID '[' INT ']' OP_ASSIGN expressao { printf("assign #2 \n"); }
;

expressao:
  // OP_ARITM op_expressao { printf("expressao #1 \n"); $$ = $2; }
// | expressao OP_ARITM expressao { printf("expressao #4 \n"); $$ = ins_node("x", REGULAR_NODE, 'E', $1, $3, "-"); }
// | op_expressao OP_COMP op_expressao { printf("expressao #5 \n"); $$ = ins_node("x", REGULAR_NODE, 'E', $1, $3, "-"); }
  op_expressao
| '(' expressao ')' { printf("expressao #6 \n"); $$ = $2 }
;

expressao_logica:
  OP_LOG op_expressao { printf("expressao_logica #1 \n"); $$ = $2; }
| '!' op_expressao { printf("expressao_logica #2 \n"); $$ = $2; }
| op_expressao OP_COMP op_expressao { printf("expressao_logica #3 \n"); $$ = ins_node("x", REGULAR_NODE, 'E', $1, $3, "expressao_logica"); }
| '(' op_expressao ')' { printf("expressao_logica #4 \n"); $$ = $2 }
| op_expressao { printf("expressao_logica #5\n"); $$ = $1; }
;

op_expressao:
  op_expressao OP_ARITM termo { printf("op_expressao #1\n"); $$ = ins_node("x", REGULAR_NODE, 'E', $1, $3, $2); }
| termo { printf("op_expressao #2\n"); $$ = $1; }
;

termo:
  ID { printf("termo #1 \n"); $$ = ins_node_symbol($1, 'S','V', $1); }
| INT { printf("termo #2 \n"); $$ = NULL }
| FLOAT { printf("termo #3 \n"); $$ = NULL }
;

scan:
  SCAN '(' ID ')' { printf("scan #1 \n"); }
;

print:
  PRINT '(' termo ')' ';' { printf("print #1 \n"); $$ = ins_node("x", REGULAR_NODE, 'P', NULL, $3, "print"); }
| PRINT '(' palavra ')' ';' { printf("print #2 \n"); $$ = ins_node("x", REGULAR_NODE, 'P', NULL, NULL, $3); }
;

palavra:
  palavra LETRA { printf("palavra #1 \n"); }
| palavra DIGITO { printf("palavra #2 \n"); }
| LETRA { printf("palavra #3 \n"); }
| DIGITO { printf("palavra #4 \n"); }
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

  // yylex();
  // printErrors();
  // printSymTable();
  yyparse();
  printf("Abstract Syntax Tree:\n");
  print_tree(parser_tree, 0);
  printf("\n");

  print_s_table();
  return 0;
}