%error-verbose
%debug
%locations

%{
  #include <stdlib.h>
  #include <stdio.h>
  #define SYMBOL_NODE 888
  #define REGULAR_NODE 889
  
  int yylex();
  extern int lin;
  void yyerror(const char* msg) {
    fprintf(stderr, "ERRO na linha %d: %s\n", lin, msg);
  }
  extern FILE *yyin;


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
  const char * stringBasedOnNumber(int number)
  {
    if(number == SYMBOL_NODE){
      return "Symbol Node";
    } else if(number == REGULAR_NODE){
      return "Regular Node";
    }
    return "Not Found";
  }
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
  TIPO ID ';' { printf("var_decl \n"); $$ = ins_node_symbol($1, SYMBOL_NODE,'D', $2); }
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
  ID { printf("termo #1 \n"); $$ = ins_node_symbol($1, 'S','D', $1); }
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

  return 0;
}