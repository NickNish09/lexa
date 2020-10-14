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

  // yylex();
  // printErrors();
  // printSymTable();
  yyparse();

  return 0;
}