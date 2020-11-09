%define parse.error verbose
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
  #define REDECLARATION_ERROR 5001
  #define NO_DECLARATION_ERROR 5002
  #define TYPES_MISSMATCH_ERROR 5003
  // #define DEBUG 993
  #define TRUE 1
  #define FALSE 0

  int has_error = FALSE;
  int yylex();
  extern int lin;
  extern void printErrors();
  void yyerror(const char* msg) {
    fprintf(stderr, "ERRO na linha %d: %s\n", lin, msg);
    has_error = TRUE;
  }
  extern FILE *yyin;

  // HELPERS

  char* concat(const char *s1, const char *s2)
  {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
  }

  const char * plainName(char* composedName){
    char* return_string;
    char aux;
    int i = 0;
    while(aux != ':'){
      aux = composedName[i];
      return_string[i] = aux;
      i++;
    }
    i--;
    return_string[i] = '\0';
    return return_string;
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

  void semantic_error(int error_type, char *msg){
    char err[100];
    if(error_type == REDECLARATION_ERROR){
      sprintf(err, "semantic error, variable redeclaration: %s\n", msg);
    } else if (error_type == NO_DECLARATION_ERROR){
      sprintf(err, "semantic error, no declarion found for: %s\n", msg);
    } else if (error_type == TYPES_MISSMATCH_ERROR){
      sprintf(err, "semantic error, types missmatch: %s\n", msg);
    };

    yyerror(err);
  }

  int types_match(char* t1, char* t2){
    #if defined DEBUG
      printf("t1: %s | t2: %s\n", t1, t2);
    #endif
    if(strcmp(t1, t2) == 0){
      return TRUE;
    } else if(strcmp(t1, "int") == 0){ // se t1 é int, entao se t2 for float esta ok
      if(strcmp(t2, "float") == 0){
        return TRUE;
      } else {
        return FALSE;
      }
    } else if(strcmp(t1, "float") == 0){ // se t1 é float, entao se t2 for int esta ok
      if(strcmp(t2, "int") == 0){
        return TRUE;
      } else {
        return FALSE;
      }
    }

    return FALSE;
  }

  // ESCOPO
  typedef struct scope {
    int level; // 0 pra global, 1 pra funcoes, 2 pra funcao dentro de funcao ...
    char *id; // identificador do escopo
    struct scope *prev; // pra pilha
  } scope;

  int current_scope_level = 0; // global
  scope * s_stack = NULL;
  char scopes_names[50][50];
  int scopes_count = 0;

  void initialize_s_stack(){
    #if defined DEBUG
      printf("initializing scope stack\n");
    #endif
    s_stack = (scope *)malloc(sizeof *s_stack);
    s_stack->level = 0;
    s_stack->id = "global";
    s_stack->prev = NULL;
    strcpy(scopes_names[0], "global");
  };

  scope * s_push(char *s_id){
    scope * s_aux = (scope *)malloc(sizeof *s_aux);
    s_aux->level = current_scope_level;
    s_aux->id = s_id;
    s_aux->prev = s_stack; // empilha
    s_stack = s_aux;
    #if defined DEBUG
      printf("pushed %s to scope stack\n", s_aux->id);
    #endif
    current_scope_level++;

    scopes_count++;
    strcpy(scopes_names[scopes_count],s_id);
    return s_aux;
  };

  scope * s_pop(){
    scope * s_aux = s_stack;
    s_stack = s_aux->prev;
    #if defined DEBUG
      printf("poped %s from scope stack\n", s_aux->id);
    #endif
    current_scope_level--;
    return s_aux;
  };

  // END ESCOPO

  // REFERENTE A TABELA DE SIMBOLOS
  typedef struct s_node {
    char* id;
    char* var_type;
    int s_node_type;
    int scope; // 0 = global
    UT_hash_handle hh;
    int params_count;
    struct s_node* params_list[10];
  } s_node;

  s_node *s_table = NULL;

  s_node* add_to_s_table(char* id, char* var_type, int s_node_type, int scope){
    s_node *s;
    char scope_string[5];
    sprintf(scope_string, "%d", scope);
    char *auxid = concat("::", s_stack->id);
    char *identifier = concat(id, auxid);
    HASH_FIND_STR(s_table, identifier, s);
    if(s == NULL){ // variavel ainda nao esta na tabela
      s_node *s = (s_node *)malloc(sizeof *s);
      s->id = identifier;
      s->var_type = var_type;
      s->s_node_type = s_node_type;
      s->scope = current_scope_level;
      s->params_count = 0;
      HASH_ADD_STR(s_table, id, s);
      return s;
    } else { // variavel ja esta na tabela, levantar erro de redeclaracao
      semantic_error(REDECLARATION_ERROR, identifier);
    };
    return s;
  }

  void print_s_table() {
    s_node *s;

    printf("Tabela de Símbolos:\n");
    // printf("NAME\t\tTYPE\t\tSYMBOL_TYPE\t\tSCOPE SYMBOLS\n");
    for(s=s_table; s != NULL; s=s->hh.next) {
      printf("id: %15s | var_type: %7s | s_node_type: %10s | scope_level: %d | params_count: %d", s->id, s->var_type, stringBasedOnNumber(s->s_node_type), s->scope, s->params_count);
      if(s->s_node_type == FUNCTION_TYPE){
        printf(" | params: ");
        int k;
        for(k=0;k<s->params_count;k++){
          printf("%s | ", plainName(s->params_list[k+1]->id));
        }
        printf("\n");
      } else {
        printf("\n");
      }
    }
  }

  s_node* find_in_s_table(char* id){
    s_node *s;
    int i;
    for(i=0; i<=scopes_count;i++){
      // printf("sn: %s\n", scopes_names[i]);
      char *auxid = concat("::", scopes_names[i]);
      char *identifier = concat(id, auxid);
      // print_s_table();
      // printf("identificador: %s\n", identifier);
      
      HASH_FIND_STR(s_table, identifier, s);
      if(s != NULL){
        return s;
      }
    }

    return s;
  }

  // END REFERENTE A TABELA DE SIMBOLOS

  // REFERENTE A ARVORE SINTATICA
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

  int height(node* node) 
  { 
    if (node==NULL) 
        return 0; 
    else
    { 
        /* compute the height of each subtree */
        int lheight = height(node->left); 
        int rheight = height(node->right); 
  
        /* use the larger one */
        if (lheight > rheight) 
            return(lheight+1); 
        else return(rheight+1); 
    } 
  } 
  void printLevel(node* tree, int level) 
  { 
    if (tree == NULL) 
        return; 
    if (level == 1) 
        printf("| var_type: %s | kind: %c | type: %s | val: %s |\n",tree->var_type, tree->node_kind, stringBasedOnNumber(tree->node_type), tree->val);
    else if (level > 1) 
    { 
        printLevel(tree->left, level-1); 
        printLevel(tree->right, level-1);
    } 
  } 
  void printLevelOrder(node* tree) 
  { 
      int h = height(tree); 
      int i, j; 
      for (i=0; i<=h; i++){
        for(j=0;j<i;j++){
          printf(" ");
        }
        printLevel(tree, i); 
      }
  } 
  void print_tree(node * tree, int h) {
    if (tree) {
      int j;
      for(j=0;j<h;j++){
        printf(" ");
      }
      printf("| val: %s | kind: %c | type: %s | var_type: %s |\n",tree->val, tree->node_kind, stringBasedOnNumber(tree->node_type), tree->var_type);
      print_tree(tree->left, h+1);
      print_tree(tree->right, h+1);
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
%type <nd> cod_blocks expressao_logica termo op_expressao declaracao_tupla tuple_expressao tuple_args
%type <str> palavra variable

%token BOOL
%token <tipo> TIPO
%token <str> IF ELSE
%token LACOS
%token <str> RETORNO
%token <str> INT FLOAT TUPLE
%token <id> ID
%token <str> DIGITO LETRA
%token SEPARADOR
%token PRINT SCAN

%right <operador> OP_ASSIGN
%left <operador> OP_ARITM
%left <operador> OP_LOG
%left <operador> OP_COMP

%%
programa: 
  declaracoes { 
    parser_tree = $1;
    #if defined DEBUG
      printf("tree initialized\n"); 
    #endif 
  }
;

declaracoes:
  declaracoes declaracao { 
    #if defined DEBUG
      printf("declaracoes \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE,'D', $1, $2, "decl"); 
  }
| declaracao { 
    #if defined DEBUG
      printf("declaracao \n"); 
    #endif
    $$ = $1; 
  }
;

declaracao:
  var_decl { 
    #if defined DEBUG
      printf("var_decl\n"); 
    #endif
    $$ = $1;
  }
| TUPLE declaracao_tupla { 
    #if defined DEBUG
      printf("tuple_decl\n"); 
    #endif
    $$ = $2;
  }
| func_decl { 
    #if defined DEBUG
      printf("func_decl\n"); 
    #endif
    $$ = $1; 
  }
;

declaracao_tupla:
  TIPO ID ',' declaracao_tupla { 
    #if defined DEBUG
      printf("declaracao_tupla #1\n"); 
      printf("var: %s | type: %s\n", $4->val, $4->var_type);
      printf("should_type: %s\n", $1);
    #endif
    $$ = $4;
    s_node* s = find_in_s_table($4->val);
    s->var_type = concat($1,$4->var_type);
    // printf("CONCASS: %s\n",concat($1, $4->var_type));
  }
| TIPO ID ID ';'{
    #if defined DEBUG
      printf("declaracao_tupla #2\n"); 
    #endif
    // $$ = $1;
    $$ = ins_node_symbol($1, SYMBOL_NODE,'T', $3);
  }
| ID {
    $$ = ins_node_symbol($1, SYMBOL_NODE,'T', $1);;
  }
;

var_decl:
  TIPO ID ';' { 
    #if defined DEBUG
      printf("var_decl \n"); 
    #endif
    $$ = ins_node_symbol($1, SYMBOL_NODE,'V', $2);
  }
;

func_decl:
  TIPO ID '(' {
    // $<nd>$ = ins_node_symbol($1, SYMBOL_NODE,'F', $2); 
    add_to_s_table($2, $1, FUNCTION_TYPE, 0);
    s_push($2);
  }
  parm_tipos ')' { ; }
  '{' cod_blocks '}' ';' { 
      $<nd>$ = ins_node($1, REGULAR_NODE,'F', $5, $9, $2);
      s_pop(); 
  }
| TIPO ID '(' ')' '{' {
  #if defined DEBUG
    printf("func_decl #4 \n");
  #endif
  add_to_s_table($2, $1, FUNCTION_TYPE, 0); 
  s_push($2);
}
 cod_blocks '}' ';' { $<nd>$ = ins_node($1, REGULAR_NODE,'F', NULL, $7, $2); s_pop(); }
;

parm_tipos:
  parm_tipos TIPO ID { 
    #if defined DEBUG
      printf("parm_tipos #1 \n"); 
    #endif
    s_node* aux = add_to_s_table($3, $2, VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    // printf("aux: %s\n", aux);
    $$ = $1;
  }
| parm_tipos TIPO ID '[' ']' {
    #if defined DEBUG
      printf("parm_tipos #2 \n"); 
    #endif
    s_node* aux = add_to_s_table($3, $2, VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    $$ = $1; 
  }
| TIPO ID ',' {
   #if defined DEBUG
    printf("parm_tipos #3 \n"); 
   #endif
   s_node* aux = add_to_s_table($2, $1, VARIABLE_TYPE, 0);
   s_node* func = find_in_s_table(s_stack->id);
   func->params_count++;
   func->params_list[func->params_count] = aux;
   $$ = NULL; 
  }
| TIPO ID {
   #if defined DEBUG
    printf("parm_tipos #4 \n"); 
   #endif
   s_node* aux = add_to_s_table($2, $1, VARIABLE_TYPE, 0);
   s_node* func = find_in_s_table(s_stack->id);
   func->params_count++; 
   func->params_list[func->params_count] = aux;
   $$ = NULL;
  }
| TIPO ID '[' ']' { 
    #if defined DEBUG
      printf("parm_tipos #5 \n"); 
    #endif
    s_node* aux = add_to_s_table($2, $1, VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    $$ = NULL; 
  }
| TUPLE ID { 
    #if defined DEBUG
      printf("parm_tipos #6\n"); 
    #endif
    // $$ = NULL; 
    $$ = ins_node_symbol($1, 'S','V', $2);
  }
;

cod_blocks:
  cod_blocks cod_block { 
    #if defined DEBUG
      printf("cod_blocks #1\n"); 
    #endif
    $$ = ins_node("-", REGULAR_NODE,'C', $1, $2, "code_block"); 
  }
| cod_block  {
    #if defined DEBUG
      printf("cod_blocks #2\n");
    #endif
    $$ = $1; 
   }
;

cod_block:
  IF '(' expressao_logica ')' '{' cod_blocks '}' {
    #if defined DEBUG
      printf("cod_block #1 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE,'I', $3, $6, "if"); 
  }
| IF '(' expressao_logica ')' '{' cod_blocks '}' ELSE '{' cod_blocks '}' { 
    #if defined DEBUG
      printf("cod_block #2 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE,'I', $3, ins_node("-", REGULAR_NODE,'I', $6, $10, "cb"), "if-else"); 
  }
| LACOS '(' expressao_logica ')' '{' cod_block '}' { 
    #if defined DEBUG
      printf("cod_block #3 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE,'L', $3, $6, "while"); 
  }
| RETORNO ';' { 
    #if defined DEBUG
      printf("cod_block #4 \n");
    #endif
    $$ = NULL; 
  }
| RETORNO termo ';' {
    #if defined DEBUG
      printf("cod_block #4.5 \n");
    #endif
    // printf("CURRENTSCOPE::%s %s\n", s_stack->id, scopes_names[0]);
    s_node* s = find_in_s_table(s_stack->id);
    if(s != NULL){
      // printf("FUNCTYPE : %s\n", s->var_type);
      if(!types_match(s->var_type, $2->var_type)){
        semantic_error(TYPES_MISSMATCH_ERROR, "return type mismatch");
      }
    }
    $$ = ins_node("-", REGULAR_NODE,'R', NULL, $2, "retorno");
  }
| RETORNO '(' expressao ')' ';' {
    #if defined DEBUG
      printf("cod_block #5 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE,'R', NULL, $3, "retorno"); 
  }
| assign ';' { 
    #if defined DEBUG
      printf("cod_block #6 \n"); 
    #endif
    $$ = $1;
  }
| print { 
    #if defined DEBUG
      printf("cod_block #7 \n"); 
    #endif
    $$ = $1;
  }
| variable '(' expressao ')' ';' { 
    #if defined DEBUG
      printf("cod_block #8 \n"); 
    #endif
    $$ = $3;
  }
| variable '(' ')' ';' {
    #if defined DEBUG
      printf("cod_block #9 \n");
    #endif
    $$ = ins_node("-", 'C','R', NULL, NULL, "call");
  }
| scan '(' variable ')' ';' {
    #if defined DEBUG
      printf("cod_block #10 \n"); 
    #endif
  }
| declaracoes { $$ = $1 ;}
;

assign:
  variable OP_ASSIGN expressao { 
    $$ = $3;
    s_node* s = find_in_s_table($1);
    #if defined DEBUG
      printf("assign #1 \n"); 
      printf("TIIIPO: %s | %s\n", $3->var_type, s->var_type);
    #endif
    if(!types_match($3->var_type, s->var_type)){
      char msg[50];
      sprintf(msg, "%s %s\n", $3->var_type, s->var_type);
      semantic_error(TYPES_MISSMATCH_ERROR, msg);
    }
  }
| variable '[' INT ']' OP_ASSIGN expressao { 
    #if defined DEBUG
      printf("assign #2 \n");
    #endif
    $$ = $6;
  }
| variable OP_ASSIGN tuple_expressao { 
    #if defined DEBUG
      printf("assign #3 \n");
    #endif
    // $$ = ins_node("-", REGULAR_NODE, 'T', ins_node("-", REGULAR_NODE, 'E', NULL, NULL, $1), $3, $1);
    $$ = $3;
  }
;

expressao:
  // OP_ARITM op_expressao { printf("expressao #1 \n"); $$ = $2; }
// | expressao OP_ARITM expressao { printf("expressao #4 \n"); $$ = ins_node("-", REGULAR_NODE, 'E', $1, $3, "-"); }
// | op_expressao OP_COMP op_expressao { printf("expressao #5 \n"); $$ = ins_node("-", REGULAR_NODE, 'E', $1, $3, "-"); }
  op_expressao
| '(' expressao ')' {
    #if defined DEBUG
      printf("expressao #6 \n"); 
    #endif
    $$ = $2; 
  }
;

tuple_expressao:
  '(' tuple_args ')' { 
    $$ = $2;
  }
;

tuple_args:
  tuple_args ',' termo {
    // $$ = ins_node("-", REGULAR_NODE, 'T', $1, ins_node("-", REGULAR_NODE, 'E', NULL, NULL, $3->val), $3->val);
    $$ = $1;
  }
| termo {
    $$ = $1; 
  }
;

expressao_logica:
  OP_LOG op_expressao { 
    #if defined DEBUG
      printf("expressao_logica #1 \n"); 
    #endif
    $$ = $2; 
  }
| '!' op_expressao { 
    #if defined DEBUG
      printf("expressao_logica #2 \n"); 
    #endif
    $$ = $2; 
  }
| op_expressao OP_COMP op_expressao { 
    #if defined DEBUG
      printf("expressao_logica #3 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE, 'E', $1, $3, "expressao_logica"); 
  }
| '(' op_expressao ')' { 
    #if defined DEBUG
      printf("expressao_logica #4 \n"); 
    #endif
    $$ = $2; 
  }
| op_expressao { 
    #if defined DEBUG
      printf("expressao_logica #5\n"); 
    #endif
    $$ = $1; 
  }
| BOOL {
    #if defined DEBUG
      printf("expressao_logica #6\n"); 
    #endif
    $$ = NULL;
  }
;

op_expressao:
  op_expressao OP_ARITM termo { 
    #if defined DEBUG
      printf("op_expressao #1\n");
    #endif
    // printf("%s ll %s\n", $1->val, $3->val);
    s_node* s1 = find_in_s_table($1->val);
    s_node* s2 = find_in_s_table($3->val);
    int tm = types_match(s1->var_type, s2->var_type);
    if(tm){
      #if defined DEBUG
        printf("types OK\n");
      #endif
    } else {
      #if defined DEBUG
        printf("types MISSMATCH: %s | %s\n", s1->var_type, s2->var_type);
      #endif
      char msg[50];
      sprintf(msg, "%s %s\n", s1->var_type, s2->var_type);
      semantic_error(TYPES_MISSMATCH_ERROR, msg);
    }
    $$ = ins_node(s1->var_type, REGULAR_NODE, 'E', $1, $3, $2); 
  }
| termo { 
    #if defined DEBUG
      printf("op_expressao #2\n"); 
    #endif
    $$ = $1;
  }
;

termo:
  variable { 
    #if defined DEBUG
      printf("termo #1 \n");
    #endif
    // $$ = ins_node_symbol($1, 'S','V', $1);
    s_node* s = find_in_s_table($1);
    $$ = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, $1);
  }
| INT { 
    #if defined DEBUG
      printf("termo #2 \n");
    #endif
    // $$ = NULL; 
    $$ = ins_node("int", REGULAR_NODE, 'E', NULL, NULL, "int");
  }
| FLOAT { 
    #if defined DEBUG
      printf("termo #3 \n");
    #endif
    $$ = NULL; 
  }
| variable '[' INT ']' { 
    #if defined DEBUG
      printf("termo #4 \n");
    #endif
    s_node* s = find_in_s_table($1);
    $$ = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, $1);
  }
| palavra{
  #if defined DEBUG
    printf("termo #5 \n");
  #endif
  $$ = NULL; 
}
;

scan:
  SCAN '(' variable ')' { 
    #if defined DEBUG
      printf("scan #1 \n"); 
    #endif
  }
;

print:
  PRINT '(' termo ')' ';' { 
    #if defined DEBUG
      printf("print #1 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE, 'P', NULL, $3, "print"); 
  }
| PRINT '(' palavra ')' ';' { 
    #if defined DEBUG
      printf("print #2 \n");
    #endif
    $$ = ins_node("-", REGULAR_NODE, 'P', NULL, NULL, $3); 
  }
;

palavra:
  palavra LETRA { 
    #if defined DEBUG
      printf("palavra #1 \n");
    #endif
  }
| palavra DIGITO { 
    #if defined DEBUG
      printf("palavra #2 \n"); 
    #endif
  }
| LETRA { 
    #if defined DEBUG
      printf("palavra #3 \n"); 
    #endif
  }
| DIGITO { 
    #if defined DEBUG
      printf("palavra #4 \n"); 
    #endif
  }
;

variable:
  ID {
    #if defined DEBUG
      printf("variable #1 \n"); 
    #endif
    s_node* s = find_in_s_table($1);
    if(s == NULL){ // nao declarou a variavel ainda
      semantic_error(NO_DECLARATION_ERROR, $1);
    }
    $$ = $1;
    // $$ = ins_node("-", REGULAR_NODE, 'V', NULL, NULL, $1); 
  }

%%

int main(int argc, char **argv){
  ++argv, --argc;
  if( argc > 0 ){
      yyin = fopen( argv[0], "r" );
  }
  else{
      yyin = stdin;
  }
  initialize_s_stack();
  // system("clear");
  yyparse();
  if(has_error != TRUE){
    printf("\n\nAbstract Syntax Tree:\n");
    print_tree(parser_tree, 0);
    // printLevelOrder(parser_tree);
    printf("\n");

    print_s_table();
  } else {
    printErrors();
  }
  #if defined DEBUG
    printf("Debug Mode...\nEscopos:\n");
    int i;
    for(i=0; i<scopes_count;i++){
      printf("%s\n", scopes_names[i]);
    }
  #endif
  return 0;
}