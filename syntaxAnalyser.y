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
    int type; //int, float, char, tuple
    char kind; // function, var, code_block...
    struct node *left;
    struct node *right;
    struct node *middle;
    char *val;

  } node;

  node *parser_tree = NULL;

  void insert_node(node ** tree, int node_type, char node_kind, char *node_val, int is_left) {
  node *aux = NULL;
  if(!(*tree)) {
    aux = (node *)malloc(sizeof(node));
    aux->left = aux->right = NULL;
    aux->type = node_type;
    aux->kind = node_kind;
    aux->val = node_val;
    *tree = aux;
    return;
  }
 
  if(is_left) {
       insert_node(&(*tree)->left, node_type, node_kind, node_val, is_left);
    } else {
      insert_node(&(*tree)->right, node_type, node_kind, node_val, is_left);
    }
  }

  void create_node(int node_type, char node_kind, char *node_val){
    node *aux;

    // set the aux parameters
    aux->type = node_type;
    aux->kind = node_kind;
    aux->val = node_val;

    if(parser_tree == NULL){
      parser_tree = aux;
    };
  }

  void print_tree(node * tree) {
    if (tree) {
      print_tree(tree->left);
      printf("kind:%c type: %d\n",tree->kind, tree->type);
      print_tree(tree->right);
    }
  }
%}

%union {
  char *id;
  char *tipo;
  char *operador;

  char *str;
}

%type <str> programa declaracoes declaracao var_decl func_decl parm_tipos cod_block assign expressao scan print

%token OP_ARITM OP_COMP OP_LOG OP_ASSIGN
%token BOOL
%token <tipo> TIPO
%token CONDICOES
%token LACOS
%token RETORNO
%token <str> INT FLOAT
%token <id> ID
%token <str> DIGITO LETRA
%token SEPARADOR

%%
programa: 
  declaracoes { $$ = $1; printf("tree initialized\n"); }
;

declaracoes:
  declaracoes declaracao { printf("declaracoes \n"); }
| declaracao { printf("declaracao \n"); }
;

declaracao:
  var_decl { printf("\nvar_decl\n"); }
| "tuple" declaracao  { printf("tuple\n"); }
| func_decl { printf("\nfunc_decl\n"); }
;

var_decl:
  TIPO ID ';' { printf("TIPO ID %d %d", $1, $2); }
;

func_decl:
  TIPO ID '(' parm_tipos ')' { printf("func_decl #1 \n"); }
| TIPO ID '(' ')' { printf("func_decl #2 \n"); }
| TIPO ID '(' parm_tipos ')' '{' cod_block '}' { printf("func_decl #3 \n"); }
| TIPO ID '(' ')' '{' cod_block '}' { printf("func_decl #4 \n"); }
;

parm_tipos:
  parm_tipos TIPO ID { printf("parm_tipos #1 \n"); }
| parm_tipos TIPO ID '[' ']' { printf("parm_tipos #2 \n"); }
| TIPO ID { printf("parm_tipos #3 \n"); }
| TIPO ID '[' ']' { printf("parm_tipos #4 \n"); }
;

cod_block:
  "if" '(' expressao ')' '{' cod_block '}' { printf("cod_block #1 \n"); }
| "if" '(' expressao ')' '{' cod_block '}' "else" '{' cod_block '}' { printf("cod_block #2 \n"); }
| "while" '(' expressao ')' '{' cod_block '}' { printf("cod_block #3 \n"); }
| RETORNO ';' { printf("cod_block #4 \n"); }
| RETORNO expressao ';' { printf("cod_block #5 \n"); }
| assign ';' { printf("cod_block #6 \n"); }
| ID '(' expressao ')' ';' { printf("cod_block #7 \n"); }
| ID '(' ')' ';' { printf("cod_block #8 \n"); }
| scan { printf("cod_block #9 \n"); }
| print { printf("cod_block #10 \n"); }
;

assign:
  ID OP_ASSIGN expressao { printf("assign #1 \n"); }
| ID '[' INT ']' OP_ASSIGN expressao { printf("assign #2 \n"); }
;

expressao:
  OP_ARITM expressao { printf("expressao #1 \n"); }
| OP_LOG expressao { printf("expressao #2 \n"); }
| '!' expressao { printf("expressao #3 \n"); }
| expressao OP_ARITM expressao { printf("expressao #4 \n"); }
| expressao OP_COMP expressao { printf("expressao #5 \n"); }
| '(' expressao ')' { printf("expressao #6 \n"); }
;

scan:
  "scan" '(' ID ')' { printf("scan #1 \n"); }
;

print:
  "print" '(' ID ')' { printf("print #1 \n"); }
| "print" '(' palavra ')' { printf("print #2 \n"); }
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

  return 0;
}