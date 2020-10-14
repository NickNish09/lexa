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
    int var_type; // 0 int, 1 float, 2 char, 3 tuple
    char node_kind; // 'F' function, 'V' var, 'C' code_block...
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
  node* ins_node(int var_type, char node_type, char node_kind, node *left, node *right){
    node* aux_node = (node*)calloc(1, sizeof(node));

    aux_node->left = left;
    aux_node->right = right;
    aux_node->var_type = var_type;
    aux_node->node_type = node_type;
    aux_node->node_kind = node_kind;

  return aux_node;
}

  void print_tree(node * tree) {
    if (tree) {
      print_tree(tree->left);
      printf("var_type: %d kind:%c type: %c\n",tree->var_type, tree->node_kind, tree->node_type);
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
  declaracoes { parser_tree = $1; printf("tree initialized\n"); }
;

declaracoes:
  declaracoes declaracao { printf("declaracoes \n"); $$ = ins_node(27, 'R','D', $1, $2); }
| declaracao { printf("declaracao \n"); $$ = $1; }
;

declaracao:
  var_decl { printf("var_decl\n"); $$ = $1; }
| "tuple" declaracao  { printf("tuple\n"); }
| func_decl { printf("func_decl\n"); $$ = $1; }
;

var_decl:
  TIPO ID ';' { printf("var_decl \n"); $$ = NULL; }
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
| ID { printf("expressao #7 \n"); }
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
  printf("Abstract Syntax Tree:\n");
  print_tree(parser_tree);

  return 0;
}