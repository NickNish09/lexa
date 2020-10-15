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


  typedef struct node {
    char* var_type; //
    char node_kind; // 'F' function, 'V' var, 'C' code_block..., 'D' declaration, 'E' expression
    char node_type; // 'S' for symbol | 'R' for regular
    struct node *left;
    struct node *right;
    struct node *middle;
    char *val;

  } node;

  node *parser_tree = NULL;

  /* Definitions 
    node
  */
node* ins_node(char* var_type, char node_type, char node_kind, node *left, node *right, char* node_val){
  node* aux_node = (node*)calloc(1, sizeof(node));

    aux_node->left = left;
    aux_node->right = right;
    aux_node->var_type = var_type;
    aux_node->node_type = node_type;
    aux_node->node_kind = node_kind;
    aux_node->val = node_val;

    return aux_node;
}

node* ins_node_symbol(char* var_type, char node_type, char node_kind, char* id){
  node* aux_node = (node*)calloc(1, sizeof(node));

  // printf("tipo_var: %s", var_type);
  aux_node->left = NULL;
  aux_node->right = NULL;
  aux_node->var_type = var_type;
  aux_node->val = id;
  aux_node->node_type = node_type;
  aux_node->node_kind = node_kind;

  return aux_node;
}

  void print_tree(node * tree) {
    if (tree) {
      printf("\nvar_type: %s\n kind:%c\n type: %c\n val: %s\n",tree->var_type, tree->node_kind, tree->node_type, tree->val);
      print_tree(tree->left);
      print_tree(tree->right);
    }
  }
%}

%union {
  char *id;
  char *tipo;
  char *operador;

  char *str;
  struct node* nd;
}

%type <nd> programa declaracoes declaracao var_decl func_decl parm_tipos cod_block assign expressao scan print
%type <nd> cod_blocks expressao_logica

%token OP_ARITM OP_COMP OP_LOG OP_ASSIGN
%token BOOL
%token <tipo> TIPO
%token CONDICOES
%token LACOS
%token <str> RETORNO
%token <str> INT FLOAT
%token <id> ID
%token <str> DIGITO LETRA
%token SEPARADOR
%token PRINT SCAN

%%
programa: 
  declaracoes { parser_tree = $1; printf("tree initialized\n"); }
;

declaracoes:
  declaracoes declaracao { printf("declaracoes \n"); $$ = ins_node("x", 'R','D', $1, $2, "-"); }
| declaracao { printf("declaracao \n"); $$ = $1; }
;

declaracao:
  var_decl { printf("var_decl\n"); $$ = $1; }
| "tuple" declaracao  { printf("tuple\n"); $$ = $2; }
| func_decl { printf("func_decl\n"); $$ = $1; }
;

var_decl:
  TIPO ID ';' { printf("var_decl \n"); $$ = ins_node_symbol($1, 'S','D', $2); }
;

func_decl:
  TIPO ID '(' parm_tipos ')' ';'{ printf("func_decl #1 \n"); $$ = ins_node_symbol($1, 'S','F', $2); }
| TIPO ID '(' ')' ';' { printf("func_decl #2 \n"); $$ = ins_node_symbol($1, 'S','F', $2); }
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
  cod_blocks cod_block { printf("cod_blocks #1\n"); $$ = ins_node("x", 'R','C', $1, $2, "cb"); }
| cod_block  { printf("cod_blocks #2\n"); $$ = $1; }
;

cod_block:
  "if" '(' expressao ')' '{' cod_blocks '}' { printf("cod_block #1 \n"); }
| "if" '(' expressao ')' '{' cod_blocks '}' "else" '{' cod_blocks '}' { printf("cod_block #2 \n"); }
| "while" '(' expressao_logica ')' '{' cod_block '}' { printf("cod_block #3 \n"); }
| RETORNO ';' { printf("cod_block #4 \n"); }
| RETORNO '(' expressao ')' ';' { printf("cod_block #5 \n"); $$ = ins_node("x", 'R','R', NULL, $3, "retorno") }
| assign ';' { printf("cod_block #6 \n"); }
| print '(' ID ')' ';' { printf("cod_block #7 \n"); $$ = $1}
| print '(' palavra ')' ';' { printf("cod_block #8 \n"); $$ = $1}
| ID '(' expressao ')' ';' { printf("cod_block #9 \n"); $$ = $3}
| ID '(' ')' ';' { printf("cod_block #10 \n"); $$ = ins_node("x", 'C','R', NULL, NULL, "call")}
| scan '(' ID ')' ';' { printf("cod_block #11 \n"); }
;

assign:
  ID OP_ASSIGN expressao { printf("assign #1 \n"); }
| ID '[' INT ']' OP_ASSIGN expressao { printf("assign #2 \n"); }
;

expressao:
  OP_ARITM expressao { printf("expressao #1 \n"); $$ = $2; }
| OP_LOG expressao { printf("expressao #2 \n"); $$ = $2; }
| '!' expressao { printf("expressao #3 \n"); $$ = $2; }
| expressao OP_ARITM expressao { printf("expressao #4 \n"); $$ = ins_node("x", 'E', 'R', $1, $3, "-"); }
| expressao OP_COMP expressao { printf("expressao #5 \n"); $$ = ins_node("x", 'E', 'R', $1, $3, "-"); }
| '(' expressao ')' { printf("expressao #6 \n"); $$ = $2 }
| ID { printf("expressao #7 \n"); $$ = NULL }
;

expressao_logica:
  OP_LOG expressao_logica { printf("expressao_logica #2 \n"); $$ = $2; }
| '!' expressao_logica { printf("expressao_logica #3 \n"); $$ = $2; }
| expressao_logica OP_COMP expressao_logica { printf("expressao_logica #5 \n"); $$ = ins_node("x", 'E', 'R', $1, $3, "-"); }
| '(' expressao_logica ')' { printf("expressao_logica #6 \n"); $$ = $2 }
| ID { printf("expressao_logica #7 \n"); $$ = NULL }
| INT { printf("expressao_logica #7 \n"); $$ = NULL }
| FLOAT { printf("expressao_logica #7 \n"); $$ = NULL }
;

scan:
  SCAN '(' ID ')' { printf("scan #1 \n"); }
;

print:
  PRINT '(' ID ')' { printf("print #1 \n"); $$ = ins_node("x", 'P', 'R', NULL, NULL, "print"); }
| PRINT '(' palavra ')' { printf("print #2 \n"); $$ = ins_node("x", 'P', 'R', NULL, NULL, "print"); }
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
  print_tree(parser_tree);

  return 0;
}