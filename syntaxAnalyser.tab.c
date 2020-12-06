/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "syntaxAnalyser.y"

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <libgen.h>
  #include "uthash.h"
  #define SYMBOL_NODE 888
  #define REGULAR_NODE 889
  #define VARIABLE_TYPE 1001
  #define FUNCTION_TYPE 1002
  #define REDECLARATION_ERROR 5001
  #define NO_DECLARATION_ERROR 5002
  #define TYPES_MISSMATCH_ERROR 5003
  #define WRONG_NUMBER_OF_ARGUMENTS_ERROR 5004
  #define TAC_PATH "tests/tac/"
  #define SCOPE_SEPARATOR "__"
  #define FUNCTION_CHAR 'Z'
  // #define DEBUG 993
  #define TRUE 1
  #define FALSE 0

  #define BADKEY -1
#define CODE_ASSIGN 440
#define CODE_PRINT 441
#define CODE_RETURN 442

typedef struct { char *key; int val; } t_symstruct;

static t_symstruct lookuptable[] = {
  { "assign", CODE_ASSIGN }, 
  { "print", CODE_PRINT },
  { "retorno", CODE_RETURN },
};

#define NKEYS (sizeof(lookuptable)/sizeof(t_symstruct))

int keyfromstring(char *key)
{
    int i;
    for (i=0; i < NKEYS; i++) {
        t_symstruct *sym = &lookuptable[i];
        if (strcmp(sym->key, key) == 0)
            return sym->val;
    }
    return BADKEY;
}

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
    } else if (error_type == WRONG_NUMBER_OF_ARGUMENTS_ERROR){
      sprintf(err, "semantic error, wrong number of arguments: %s\n", msg);
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
    char *auxid = concat(SCOPE_SEPARATOR, s_stack->id);
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
          printf("%s | ", s->params_list[k+1]->var_type);
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
      char *auxid = concat(SCOPE_SEPARATOR, scopes_names[i]);
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

  s_node* find_in_s_table_plain(char* id){
    s_node *s;
    HASH_FIND_STR(s_table, id, s);
    if(s != NULL){
      return s;
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
    char *func_name;

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

node* ins_node_func_call(char* var_type, int node_type, char node_kind, node *left, node *right, char* node_val, char* func_name){
  node* aux_node = (node*)calloc(1, sizeof(node));

    aux_node->left = left;
    aux_node->right = right;
    aux_node->var_type = var_type;
    aux_node->node_type = node_type;
    aux_node->node_kind = node_kind;
    aux_node->val = node_val;
    aux_node->func_name = func_name;

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
  
  void free_tree(node * tree) {
    if (tree) {
      free_tree(tree->left);
      free_tree(tree->right);
      free(tree);
    }
  }

  int paramsc = 0;

  void check_specific_param(node *nd, char *func_name){
    if(nd != NULL){
      if(strcmp(nd->val, "func_args") != 0){
        // printf("arg: %s \n", nd->var_type);
        s_node *aux = find_in_s_table(func_name);
        // printf("should arg: %s\n", aux->params_list[paramsc]->var_type);
        if(paramsc > 0){
         if(strcmp(nd->var_type, aux->params_list[paramsc]->var_type) != 0){
            char msg[50];
            sprintf(msg, "args call differ from declaration in function %s", func_name);
            semantic_error(TYPES_MISSMATCH_ERROR, msg);
          }
          paramsc--; 
        } else {
          char msg[50];
          sprintf(msg, "function %s", func_name);
          semantic_error(WRONG_NUMBER_OF_ARGUMENTS_ERROR, msg);
        }
      }
      check_specific_param(nd->right, func_name);
      check_specific_param(nd->left, func_name);
    }
  }

  void check_params(node *nd, char* func_name){
    s_node* aux = find_in_s_table(func_name);
    paramsc = aux->params_count;
    check_specific_param(nd->right, func_name);
    paramsc = 0;
    // printf("func: %d\n", aux->params_count);
    // printf("v: %s\n", nd->right->left->right->val);
    // printf("v: %s\n", nd->right->left->left->val);
  }

  char *remove_extension(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
  }

  void generateTableInTac(FILE *tac_file){
    s_node *s;
    char aux[100];
    fputs(".table\n", tac_file);
    for(s=s_table; s != NULL; s=s->hh.next) {
      if(s->s_node_type != FUNCTION_TYPE){
        strcpy(aux, s->var_type);
        strcat(aux, " ");
        strcat(aux, s->id);
        strcat(aux, "\n");
        fputs(aux, tac_file);
      }
    }
  }

  char* generate_instruction(char *instruction, char* arg1, char* arg2, char* arg3){
    char *aux = (char*)malloc(50* sizeof(char));
    strcpy(aux, instruction);
    if(arg1 != NULL){
      strcat(aux, " ");
      strcat(aux, arg1);
      if(arg2 != NULL){
        strcat(aux, ", ");
        strcat(aux, arg2);
        if(arg3 != NULL){
          strcat(aux, ", ");
          strcat(aux, arg3);
        }
      }
    }
    strcat(aux, "\n");
    return aux;
  }

  int is_expression(char* sym){
    int is_exp = ((strcmp(sym, "+") == 0) || (strcmp(sym, "-") == 0) || (strcmp(sym, "*") == 0) || (strcmp(sym, "/") == 0));
    return is_exp;
  }

  char * generate_aritm_instruction(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    if(is_expression(sub_tree->left->val)){
      strcpy(aux, generate_aritm_instruction(sub_tree->left));
      if(strcmp(sub_tree->val, "+") == 0){
        strcat(aux, "add ");
      } else if(strcmp(sub_tree->val, "-") == 0){
        strcat(aux, "sub ");
      } else if(strcmp(sub_tree->val, "*") == 0){
        strcat(aux, "mul ");
      } else if(strcmp(sub_tree->val, "/") == 0){
        strcat(aux, "div ");
      }
      strcat(aux, "$0, $0, ");
      strcat(aux, sub_tree->right->val);
      strcat(aux, "\n");
    } else {
      if(strcmp(sub_tree->val, "+") == 0){
        strcpy(aux, "add ");
      } else if(strcmp(sub_tree->val, "-") == 0){
        strcpy(aux, "sub ");
      } else if(strcmp(sub_tree->val, "*") == 0){
        strcpy(aux, "mul ");
      } else if(strcmp(sub_tree->val, "/") == 0){
        strcpy(aux, "div ");
      }
      strcat(aux, "$0, ");
      strcat(aux, concat(sub_tree->right->val, ", "));
      strcat(aux, sub_tree->left->val);
      strcat(aux, "\n");
    }

    return aux;
  }

  int func_counter;
  char * generate_func_call_instruction(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    // printf("aa: %s\n", sub_tree->left->val);
    if(strcmp(sub_tree->left->val, "func_args") == 0){
      strcpy(aux, generate_func_call_instruction(sub_tree->left));
      strcat(aux, "param ");
      strcat(aux, sub_tree->right->val);
      strcat(aux, "\n");
      func_counter++;
    } else {
      if(sub_tree->left != NULL){
        strcat(aux, "param ");
        strcat(aux, concat(sub_tree->left->val, "\n"));
        func_counter++;
      }
      if(sub_tree->right != NULL){
        strcat(aux, "param ");
        strcat(aux, concat(sub_tree->right->val, "\n"));
        func_counter++;
      }
    }

    return aux;
  }

  void resolveNode(FILE *tac_file, node *tree){
    if(tree){
      char *aux = NULL;
      switch(keyfromstring(tree->val)){
        case CODE_ASSIGN:
          if(is_expression(tree->right->val)){
            aux = generate_aritm_instruction(tree->right);
            strcat(aux, generate_instruction("mov", tree->left->val, "$0", NULL));
          } else if(strcmp(tree->right->val, "func_call") == 0){
            func_counter = 0;
            char func_counter_string[3];
            aux = generate_func_call_instruction(tree->right->right);
            sprintf(func_counter_string, "%d", func_counter);
            strcat(aux, generate_instruction("call", concat(SCOPE_SEPARATOR, tree->right->func_name), func_counter_string, NULL));
            strcat(aux, generate_instruction("pop", tree->left->val, NULL, NULL));
          } else {
            aux = generate_instruction("mov", tree->left->val, tree->right->val, NULL);
          }
          break;

        case CODE_PRINT:
          aux = generate_instruction("println", tree->right->val, NULL, NULL);
          break;

        case CODE_RETURN:
          if(tree->right != NULL){
            if(strcmp(tree->right->val, "0") == 0){
            } else {
              aux = generate_instruction("return", tree->right->val, NULL, NULL);
            }
          } else {
            aux = generate_instruction("return", NULL, NULL, NULL);
          }
          break;
        default:
          switch(tree->node_kind){
            case FUNCTION_CHAR:
              aux = generate_instruction(concat(SCOPE_SEPARATOR, concat(tree->val, ":")), NULL, NULL, NULL);
              s_node* func_s_node = find_in_s_table(tree->val);
              int i;
              for(i=func_s_node->params_count; i> 0; i--){
                // printf("LL: %s\n", func_s_node->params_list[i]->id);
                strcat(aux, generate_instruction("pop", func_s_node->params_list[i]->id, NULL, NULL));
              }
              break;

            default:
              break;
          }
          break;
      }
      if(aux != NULL){
        fputs(aux, tac_file);
      }
      resolveNode(tac_file, tree->left);
      resolveNode(tac_file, tree->right);
    }
  }

  void generateCodeInTac(FILE *tac_file, node* tree){
    fputs(".code\njump main\n", tac_file);
    resolveNode(tac_file, tree);
  }

  void generateTacFile(node * tree, char* file_name){
    FILE *tac_file;
    char *file_name_with_path = concat(TAC_PATH, file_name);
    tac_file = fopen(concat(file_name_with_path, ".tac"), "w");

    if(tac_file == NULL)
    {
        printf("Falha ao criar tac file.\n");
        exit(EXIT_FAILURE);
    }

    generateTableInTac(tac_file);
    generateCodeInTac(tac_file, parser_tree);

    fclose(tac_file);
    printf("Arquivo .tac gerado em %s\n", file_name_with_path);
  }

#line 696 "syntaxAnalyser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "syntaxAnalyser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BOOL = 3,                       /* BOOL  */
  YYSYMBOL_TIPO = 4,                       /* TIPO  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_LACOS = 7,                      /* LACOS  */
  YYSYMBOL_RETORNO = 8,                    /* RETORNO  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_TUPLE = 11,                     /* TUPLE  */
  YYSYMBOL_ID = 12,                        /* ID  */
  YYSYMBOL_DIGITO = 13,                    /* DIGITO  */
  YYSYMBOL_LETRA = 14,                     /* LETRA  */
  YYSYMBOL_SEPARADOR = 15,                 /* SEPARADOR  */
  YYSYMBOL_PONTO = 16,                     /* PONTO  */
  YYSYMBOL_PRINT = 17,                     /* PRINT  */
  YYSYMBOL_SCAN = 18,                      /* SCAN  */
  YYSYMBOL_OP_ASSIGN = 19,                 /* OP_ASSIGN  */
  YYSYMBOL_OP_ARITM = 20,                  /* OP_ARITM  */
  YYSYMBOL_OP_LOG = 21,                    /* OP_LOG  */
  YYSYMBOL_OP_COMP = 22,                   /* OP_COMP  */
  YYSYMBOL_23_ = 23,                       /* ','  */
  YYSYMBOL_24_ = 24,                       /* ';'  */
  YYSYMBOL_25_ = 25,                       /* '('  */
  YYSYMBOL_26_ = 26,                       /* ')'  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* '['  */
  YYSYMBOL_30_ = 30,                       /* ']'  */
  YYSYMBOL_31_ = 31,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_programa = 33,                  /* programa  */
  YYSYMBOL_declaracoes = 34,               /* declaracoes  */
  YYSYMBOL_declaracao = 35,                /* declaracao  */
  YYSYMBOL_declaracao_tupla = 36,          /* declaracao_tupla  */
  YYSYMBOL_var_decl = 37,                  /* var_decl  */
  YYSYMBOL_func_decl = 38,                 /* func_decl  */
  YYSYMBOL_39_1 = 39,                      /* $@1  */
  YYSYMBOL_40_2 = 40,                      /* $@2  */
  YYSYMBOL_41_3 = 41,                      /* $@3  */
  YYSYMBOL_parm_tipos = 42,                /* parm_tipos  */
  YYSYMBOL_cod_blocks = 43,                /* cod_blocks  */
  YYSYMBOL_cod_block = 44,                 /* cod_block  */
  YYSYMBOL_assign = 45,                    /* assign  */
  YYSYMBOL_expressao = 46,                 /* expressao  */
  YYSYMBOL_expressao_logica = 47,          /* expressao_logica  */
  YYSYMBOL_op_expressao = 48,              /* op_expressao  */
  YYSYMBOL_termo = 49,                     /* termo  */
  YYSYMBOL_scan = 50,                      /* scan  */
  YYSYMBOL_print = 51,                     /* print  */
  YYSYMBOL_func_call = 52,                 /* func_call  */
  YYSYMBOL_func_args = 53,                 /* func_args  */
  YYSYMBOL_func_arg = 54,                  /* func_arg  */
  YYSYMBOL_palavra = 55,                   /* palavra  */
  YYSYMBOL_variable = 56                   /* variable  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   204

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   277


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,     2,     2,     2,
      25,    26,     2,     2,    23,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    24,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,    30,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    28,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   662,   662,   671,   677,   686,   692,   698,   707,   722,
     729,   735,   744,   749,   744,   754,   754,   765,   778,   790,
     802,   814,   826,   836,   842,   851,   857,   863,   869,   875,
     896,   902,   908,   914,   932,   937,   941,   958,   975,   981,
     982,  1005,  1011,  1017,  1023,  1029,  1035,  1044,  1067,  1073,
    1079,  1092,  1099,  1105,  1113,  1122,  1130,  1139,  1145,  1154,
    1157,  1163,  1173,  1184,  1185,  1186,  1190,  1195,  1200,  1205,
    1213,  1227
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "BOOL", "TIPO", "IF",
  "ELSE", "LACOS", "RETORNO", "INT", "FLOAT", "TUPLE", "ID", "DIGITO",
  "LETRA", "SEPARADOR", "PONTO", "PRINT", "SCAN", "OP_ASSIGN", "OP_ARITM",
  "OP_LOG", "OP_COMP", "','", "';'", "'('", "')'", "'{'", "'}'", "'['",
  "']'", "'!'", "$accept", "programa", "declaracoes", "declaracao",
  "declaracao_tupla", "var_decl", "func_decl", "$@1", "$@2", "$@3",
  "parm_tipos", "cod_blocks", "cod_block", "assign", "expressao",
  "expressao_logica", "op_expressao", "termo", "scan", "print",
  "func_call", "func_args", "func_arg", "palavra", "variable", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    44,    59,    40,    41,   123,   125,    91,
      93,    33
};
#endif

#define YYPACT_NINF (-74)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      11,     0,    12,    26,    11,   -74,   -74,   -74,    39,     9,
     -74,   -74,   -74,   -74,   -74,    14,    -4,    15,    48,    22,
      12,   -74,    36,    38,    21,   -74,   -74,   163,    29,   -74,
      53,    66,   -74,    67,    69,   148,    -5,    72,    79,   -74,
      49,   -74,    50,    83,   -74,   -74,   -12,    51,    80,   105,
      91,    20,    20,   -74,   -74,   107,   -74,   -74,   -74,    86,
     100,    93,   103,   117,    77,   174,   121,   -74,   -74,   -74,
     121,    86,   128,   -74,   109,   -74,   163,   -74,   174,   174,
     174,   112,    56,   -74,   118,    86,   122,    65,   -74,   -74,
     -74,   -74,   143,   -74,   -74,   -74,   124,   -74,    46,   -74,
      93,   133,   137,   139,   -74,   134,   -74,   108,    65,   156,
      65,   120,   174,   174,   174,   127,   151,   145,   155,   166,
     180,   -74,   167,   -74,   171,   177,   -74,   -74,   163,   -74,
      65,   -74,   163,   -74,   -74,   -74,   168,   -74,   -74,   -74,
      86,   123,   169,   -74,   -74,   193,   -74,   173,   163,   138,
     -74
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     2,     4,     5,     7,     0,     0,
      10,     6,     1,     3,    11,    12,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     9,     8,     0,    19,    22,
       0,     0,    13,     0,     0,     0,    70,     0,     0,    35,
       0,    24,     0,     0,    32,    33,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    70,    69,    68,    28,     0,
       0,    54,    50,     0,     0,     0,     0,    16,    23,    31,
       0,     0,     0,    21,     0,    17,     0,    46,     0,     0,
       0,     0,    45,    48,     0,     0,     0,    39,    40,    29,
      67,    66,     0,    71,    63,    64,    61,    58,     0,    60,
      65,     0,     0,     0,    36,     0,    18,     0,    41,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,    55,     0,     0,    14,    44,     0,    47,
      43,    49,     0,    38,    30,    53,     0,    59,    56,    34,
       0,     0,     0,    62,    37,    25,    27,     0,     0,     0,
      26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -74,   -74,   -74,    27,   181,   -74,   -74,   -74,   -74,   -74,
     -74,   -73,   -39,   -74,   -67,   150,   -42,   -30,   -74,   -74,
     -57,   -74,    84,   -58,   -27
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    39,    11,     6,     7,    18,    50,    27,
      24,    40,    41,    42,    86,    81,    87,    83,    43,    44,
      45,    98,    99,    61,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    68,    88,   107,   104,    60,   100,    71,    19,    82,
      82,    63,     8,    46,    88,     1,     9,    72,   116,    20,
      64,    16,     2,    77,    10,    30,    12,     5,    88,    53,
      54,    13,    55,    56,    57,   101,   108,   109,   110,   102,
      17,    78,    21,   103,    31,    79,    25,    32,    28,    46,
      29,    80,    22,     1,    33,   141,    34,    35,    47,    23,
       2,    36,   100,    14,    15,    48,    37,    38,    68,   120,
      49,   130,   121,   144,    69,   149,   112,    67,   113,   114,
      46,    73,   129,    88,   131,   112,    94,    95,   114,    96,
      56,    57,    51,   142,    52,    53,    54,    65,    36,    56,
      57,    46,    68,    97,    66,    46,    90,    91,    70,    74,
      68,    85,     1,    33,    46,    34,    35,    75,    76,     2,
      36,    46,    46,    63,    89,    37,    38,     1,    33,    93,
      34,    35,    92,    55,     2,    36,   126,   105,   111,   106,
      37,    38,     1,    33,   115,    34,    35,   128,   117,     2,
      36,   145,   118,   119,   132,    37,    38,    53,    54,   122,
      55,    56,    57,   123,   125,   124,   150,     1,    33,   134,
      34,    35,    58,    59,     2,    36,   112,   133,   136,   114,
      37,    38,   127,    53,    54,   135,    55,    56,    57,    94,
      95,   138,    96,    56,    57,   139,   140,   146,   143,   147,
     148,    26,    84,     0,   137
};

static const yytype_int16 yycheck[] =
{
      27,    40,    59,    76,    71,    35,    64,    19,    12,    51,
      52,    16,    12,    40,    71,     4,     4,    29,    85,    23,
      25,    12,    11,     3,    12,     4,     0,     0,    85,     9,
      10,     4,    12,    13,    14,    65,    78,    79,    80,    66,
      26,    21,    27,    70,    23,    25,    24,    26,    12,    76,
      12,    31,     4,     4,     5,   128,     7,     8,    29,    11,
      11,    12,   120,    24,    25,    12,    17,    18,   107,    23,
       4,   113,    26,   140,    24,   148,    20,    28,    22,    23,
     107,    30,   112,   140,   114,    20,     9,    10,    23,    12,
      13,    14,    25,   132,    25,     9,    10,    25,    12,    13,
      14,   128,   141,    26,    25,   132,    13,    14,    25,    29,
     149,    25,     4,     5,   141,     7,     8,    12,    27,    11,
      12,   148,   149,    16,    24,    17,    18,     4,     5,    12,
       7,     8,    29,    12,    11,    12,    28,     9,    26,    30,
      17,    18,     4,     5,    26,     7,     8,    27,    26,    11,
      12,    28,     9,    29,    27,    17,    18,     9,    10,    26,
      12,    13,    14,    26,    30,    26,    28,     4,     5,    24,
       7,     8,    24,    25,    11,    12,    20,    26,    12,    23,
      17,    18,    26,     9,    10,    30,    12,    13,    14,     9,
      10,    24,    12,    13,    14,    24,    19,    28,    30,     6,
      27,    20,    52,    -1,   120
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    11,    33,    34,    35,    37,    38,    12,     4,
      12,    36,     0,    35,    24,    25,    12,    26,    39,    12,
      23,    27,     4,    11,    42,    24,    36,    41,    12,    12,
       4,    23,    26,     5,     7,     8,    12,    17,    18,    35,
      43,    44,    45,    50,    51,    52,    56,    29,    12,     4,
      40,    25,    25,     9,    10,    12,    13,    14,    24,    25,
      49,    55,    56,    16,    25,    25,    25,    28,    44,    24,
      25,    19,    29,    30,    29,    12,    27,     3,    21,    25,
      31,    47,    48,    49,    47,    25,    46,    48,    52,    24,
      13,    14,    29,    12,     9,    10,    12,    26,    53,    54,
      55,    49,    56,    56,    46,     9,    30,    43,    48,    48,
      48,    26,    20,    22,    23,    26,    46,    26,     9,    29,
      23,    26,    26,    26,    26,    30,    28,    26,    27,    49,
      48,    49,    27,    26,    24,    30,    12,    54,    24,    24,
      19,    43,    44,    30,    46,    28,    28,     6,    27,    43,
      28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    36,    36,
      36,    37,    39,    40,    38,    41,    38,    42,    42,    42,
      42,    42,    42,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    45,    45,    46,    46,
      46,    47,    47,    47,    47,    47,    47,    48,    48,    48,
      49,    49,    49,    49,    49,    50,    51,    52,    52,    53,
      53,    54,    54,    54,    54,    54,    55,    55,    55,    55,
      56,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     1,     4,     4,
       1,     3,     0,     0,    10,     0,     8,     4,     5,     2,
       2,     4,     2,     2,     1,     7,    11,     7,     2,     3,
       5,     2,     1,     1,     5,     1,     3,     6,     3,     1,
       1,     2,     2,     3,     3,     1,     1,     3,     1,     3,
       1,     1,     1,     4,     1,     4,     5,     4,     3,     3,
       1,     1,     4,     1,     1,     1,     2,     2,     1,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: declaracoes  */
#line 662 "syntaxAnalyser.y"
              { 
    parser_tree = (yyvsp[0].nd);
    #if defined DEBUG
      printf("tree initialized\n"); 
    #endif 
  }
#line 2216 "syntaxAnalyser.tab.c"
    break;

  case 3: /* declaracoes: declaracoes declaracao  */
#line 671 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("declaracoes \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'D', (yyvsp[-1].nd), (yyvsp[0].nd), "decl"); 
  }
#line 2227 "syntaxAnalyser.tab.c"
    break;

  case 4: /* declaracoes: declaracao  */
#line 677 "syntaxAnalyser.y"
             { 
    #if defined DEBUG
      printf("declaracao \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2238 "syntaxAnalyser.tab.c"
    break;

  case 5: /* declaracao: var_decl  */
#line 686 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("var_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2249 "syntaxAnalyser.tab.c"
    break;

  case 6: /* declaracao: TUPLE declaracao_tupla  */
#line 692 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("tuple_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2260 "syntaxAnalyser.tab.c"
    break;

  case 7: /* declaracao: func_decl  */
#line 698 "syntaxAnalyser.y"
            { 
    #if defined DEBUG
      printf("func_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2271 "syntaxAnalyser.tab.c"
    break;

  case 8: /* declaracao_tupla: TIPO ID ',' declaracao_tupla  */
#line 707 "syntaxAnalyser.y"
                               { 
    #if defined DEBUG
      printf("declaracao_tupla #1\n"); 
      printf("var: %s | type: %s\n", (yyvsp[0].nd)->val, (yyvsp[0].nd)->var_type);
      printf("should_type: %s\n", (yyvsp[-3].tipo));
    #endif
    s_node* s = find_in_s_table((yyvsp[0].nd)->val);
    s->var_type = concat((yyvsp[-3].tipo),(yyvsp[0].nd)->var_type);
    (yyvsp[0].nd)->var_type = concat((yyvsp[-3].tipo), (yyvsp[0].nd)->var_type);
    // printf("vt: %s\n",$4->var_type);
    (yyval.nd) = (yyvsp[0].nd);
    // $$ = ins_node(concat($4->var_type, $1), REGULAR_NODE,'F', NULL, $4, $2);
    // printf("CONCASS: %s\n",concat($1, $4->var_type));
    // free(s);
  }
#line 2291 "syntaxAnalyser.tab.c"
    break;

  case 9: /* declaracao_tupla: TIPO ID ID ';'  */
#line 722 "syntaxAnalyser.y"
                {
    #if defined DEBUG
      printf("declaracao_tupla #2\n"); 
    #endif
    // $$ = $1;
    (yyval.nd) = ins_node_symbol((yyvsp[-3].tipo), SYMBOL_NODE,'T', (yyvsp[-1].id));
  }
#line 2303 "syntaxAnalyser.tab.c"
    break;

  case 10: /* declaracao_tupla: ID  */
#line 729 "syntaxAnalyser.y"
     {
    (yyval.nd) = ins_node_symbol((yyvsp[0].id), SYMBOL_NODE,'T', (yyvsp[0].id));;
  }
#line 2311 "syntaxAnalyser.tab.c"
    break;

  case 11: /* var_decl: TIPO ID ';'  */
#line 735 "syntaxAnalyser.y"
              { 
    #if defined DEBUG
      printf("var_decl \n"); 
    #endif
    (yyval.nd) = ins_node_symbol((yyvsp[-2].tipo), SYMBOL_NODE,'V', (yyvsp[-1].id));
  }
#line 2322 "syntaxAnalyser.tab.c"
    break;

  case 12: /* $@1: %empty  */
#line 744 "syntaxAnalyser.y"
              {
    // $<nd>$ = ins_node_symbol($1, SYMBOL_NODE,'F', $2); 
    add_to_s_table((yyvsp[-1].id), (yyvsp[-2].tipo), FUNCTION_TYPE, 0);
    s_push((yyvsp[-1].id));
  }
#line 2332 "syntaxAnalyser.tab.c"
    break;

  case 13: /* $@2: %empty  */
#line 749 "syntaxAnalyser.y"
                 { ; }
#line 2338 "syntaxAnalyser.tab.c"
    break;

  case 14: /* func_decl: TIPO ID '(' $@1 parm_tipos ')' $@2 '{' cod_blocks '}'  */
#line 750 "syntaxAnalyser.y"
                     { 
      (yyval.nd) = ins_node((yyvsp[-9].tipo), REGULAR_NODE, FUNCTION_CHAR, (yyvsp[-5].nd), (yyvsp[-1].nd), (yyvsp[-8].id));
      s_pop(); 
  }
#line 2347 "syntaxAnalyser.tab.c"
    break;

  case 15: /* $@3: %empty  */
#line 754 "syntaxAnalyser.y"
                      {
  #if defined DEBUG
    printf("func_decl #4 \n");
  #endif
  add_to_s_table((yyvsp[-3].id), (yyvsp[-4].tipo), FUNCTION_TYPE, 0); 
  s_push((yyvsp[-3].id));
}
#line 2359 "syntaxAnalyser.tab.c"
    break;

  case 16: /* func_decl: TIPO ID '(' ')' '{' $@3 cod_blocks '}'  */
#line 761 "syntaxAnalyser.y"
                { (yyval.nd) = ins_node((yyvsp[-7].tipo), REGULAR_NODE, FUNCTION_CHAR, NULL, (yyvsp[-1].nd), (yyvsp[-6].id)); s_pop(); }
#line 2365 "syntaxAnalyser.tab.c"
    break;

  case 17: /* parm_tipos: parm_tipos ',' TIPO ID  */
#line 765 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("parm_tipos #1 \n"); 
    #endif
    s_node* aux = add_to_s_table((yyvsp[0].id), (yyvsp[-1].tipo), VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    // printf("aux: %s\n", aux);
    (yyval.nd) = (yyvsp[-3].nd);
    // free(aux);
    // free(func);
  }
#line 2383 "syntaxAnalyser.tab.c"
    break;

  case 18: /* parm_tipos: parm_tipos TIPO ID '[' ']'  */
#line 778 "syntaxAnalyser.y"
                             {
    #if defined DEBUG
      printf("parm_tipos #2 \n"); 
    #endif
    s_node* aux = add_to_s_table((yyvsp[-2].id), (yyvsp[-3].tipo), VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    (yyval.nd) = (yyvsp[-4].nd); 
    // free(aux);
    // free(func);
  }
#line 2400 "syntaxAnalyser.tab.c"
    break;

  case 19: /* parm_tipos: TIPO ID  */
#line 790 "syntaxAnalyser.y"
          {
   #if defined DEBUG
    printf("parm_tipos #3 \n"); 
   #endif
   s_node* aux = add_to_s_table((yyvsp[0].id), (yyvsp[-1].tipo), VARIABLE_TYPE, 0);
   s_node* func = find_in_s_table(s_stack->id);
   func->params_count++;
   func->params_list[func->params_count] = aux;
   (yyval.nd) = NULL; 
  //  free(aux);
  //  free(func);
  }
#line 2417 "syntaxAnalyser.tab.c"
    break;

  case 20: /* parm_tipos: TIPO ID  */
#line 802 "syntaxAnalyser.y"
          {
   #if defined DEBUG
    printf("parm_tipos #4 \n"); 
   #endif
   s_node* aux = add_to_s_table((yyvsp[0].id), (yyvsp[-1].tipo), VARIABLE_TYPE, 0);
   s_node* func = find_in_s_table(s_stack->id);
   func->params_count++; 
   func->params_list[func->params_count] = aux;
   (yyval.nd) = NULL;
  //  free(aux);
  //  free(func);
  }
#line 2434 "syntaxAnalyser.tab.c"
    break;

  case 21: /* parm_tipos: TIPO ID '[' ']'  */
#line 814 "syntaxAnalyser.y"
                  { 
    #if defined DEBUG
      printf("parm_tipos #5 \n"); 
    #endif
    s_node* aux = add_to_s_table((yyvsp[-2].id), (yyvsp[-3].tipo), VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    (yyval.nd) = NULL; 
    // free(aux);
    // free(func);
  }
#line 2451 "syntaxAnalyser.tab.c"
    break;

  case 22: /* parm_tipos: TUPLE ID  */
#line 826 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("parm_tipos #6\n"); 
    #endif
    // $$ = NULL; 
    (yyval.nd) = ins_node_symbol((yyvsp[-1].str), 'S','V', (yyvsp[0].id));
  }
#line 2463 "syntaxAnalyser.tab.c"
    break;

  case 23: /* cod_blocks: cod_blocks cod_block  */
#line 836 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("cod_blocks #1\n"); 
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'C', (yyvsp[-1].nd), (yyvsp[0].nd), "code_block"); 
  }
#line 2474 "syntaxAnalyser.tab.c"
    break;

  case 24: /* cod_blocks: cod_block  */
#line 842 "syntaxAnalyser.y"
             {
    #if defined DEBUG
      printf("cod_blocks #2\n");
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
   }
#line 2485 "syntaxAnalyser.tab.c"
    break;

  case 25: /* cod_block: IF '(' expressao_logica ')' '{' cod_blocks '}'  */
#line 851 "syntaxAnalyser.y"
                                                 {
    #if defined DEBUG
      printf("cod_block #1 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'I', (yyvsp[-4].nd), (yyvsp[-1].nd), "if"); 
  }
#line 2496 "syntaxAnalyser.tab.c"
    break;

  case 26: /* cod_block: IF '(' expressao_logica ')' '{' cod_blocks '}' ELSE '{' cod_blocks '}'  */
#line 857 "syntaxAnalyser.y"
                                                                         { 
    #if defined DEBUG
      printf("cod_block #2 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'I', (yyvsp[-8].nd), ins_node("-", REGULAR_NODE,'I', (yyvsp[-5].nd), (yyvsp[-1].nd), "cb"), "if-else"); 
  }
#line 2507 "syntaxAnalyser.tab.c"
    break;

  case 27: /* cod_block: LACOS '(' expressao_logica ')' '{' cod_block '}'  */
#line 863 "syntaxAnalyser.y"
                                                   { 
    #if defined DEBUG
      printf("cod_block #3 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'L', (yyvsp[-4].nd), (yyvsp[-1].nd), "while"); 
  }
#line 2518 "syntaxAnalyser.tab.c"
    break;

  case 28: /* cod_block: RETORNO ';'  */
#line 869 "syntaxAnalyser.y"
              { 
    #if defined DEBUG
      printf("cod_block #4 \n");
    #endif
    (yyval.nd) = NULL; 
  }
#line 2529 "syntaxAnalyser.tab.c"
    break;

  case 29: /* cod_block: RETORNO termo ';'  */
#line 875 "syntaxAnalyser.y"
                    {
    #if defined DEBUG
      printf("cod_block #4.5 \n");
    #endif
    // printf("CURRENTSCOPSCOPE_SEPARATORs %s\n", s_stack->id, scopes_names[0]);
    s_node* s = find_in_s_table(s_stack->id);
    s_node* ss = find_in_s_table_plain((yyvsp[-1].nd)->val);
    if(s != NULL){
      if(ss != NULL){
        if(!types_match(s->var_type, ss->var_type)){
          semantic_error(TYPES_MISSMATCH_ERROR, "return type mismatch");
        }
      } else {
        if(!types_match(s->var_type, (yyvsp[-1].nd)->var_type)){
          semantic_error(TYPES_MISSMATCH_ERROR, "return type mismatch");
        }
      }
    }
    (yyval.nd) = ins_node("-", REGULAR_NODE,'R', NULL, (yyvsp[-1].nd), "retorno");
    // free(s);
  }
#line 2555 "syntaxAnalyser.tab.c"
    break;

  case 30: /* cod_block: RETORNO '(' expressao ')' ';'  */
#line 896 "syntaxAnalyser.y"
                                {
    #if defined DEBUG
      printf("cod_block #5 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'R', NULL, (yyvsp[-2].nd), "retorno"); 
  }
#line 2566 "syntaxAnalyser.tab.c"
    break;

  case 31: /* cod_block: assign ';'  */
#line 902 "syntaxAnalyser.y"
             { 
    #if defined DEBUG
      printf("cod_block #6 \n"); 
    #endif
    (yyval.nd) = (yyvsp[-1].nd);
  }
#line 2577 "syntaxAnalyser.tab.c"
    break;

  case 32: /* cod_block: print  */
#line 908 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("cod_block #7 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2588 "syntaxAnalyser.tab.c"
    break;

  case 33: /* cod_block: func_call  */
#line 914 "syntaxAnalyser.y"
            { 
  #if defined DEBUG
    printf("cod_block #7.5 \n"); 
  #endif
  (yyval.nd) = (yyvsp[0].nd);
  }
#line 2599 "syntaxAnalyser.tab.c"
    break;

  case 34: /* cod_block: scan '(' variable ')' ';'  */
#line 932 "syntaxAnalyser.y"
                            {
    #if defined DEBUG
      printf("cod_block #10 \n"); 
    #endif
  }
#line 2609 "syntaxAnalyser.tab.c"
    break;

  case 35: /* cod_block: declaracao  */
#line 937 "syntaxAnalyser.y"
             { (yyval.nd) = (yyvsp[0].nd) ;}
#line 2615 "syntaxAnalyser.tab.c"
    break;

  case 36: /* assign: variable OP_ASSIGN expressao  */
#line 941 "syntaxAnalyser.y"
                               { 
    // $$ = $3;
    (yyval.nd) = ins_node("-", 'C','R', ins_node("-", 'C','R', NULL, NULL, (yyvsp[-2].str)), (yyvsp[0].nd), "assign");
    s_node* s = find_in_s_table((yyvsp[-2].str));
    #if defined DEBUG
      printf("assign #1 \n");
      printf("TIIIPO: %s | %s\n", (yyvsp[0].nd)->var_type, s->var_type);
    #endif
    if(s != NULL){
      if(!types_match((yyvsp[0].nd)->var_type, s->var_type)){
        char msg[50];
        sprintf(msg, "%s %s\n", (yyvsp[0].nd)->var_type, s->var_type);
        semantic_error(TYPES_MISSMATCH_ERROR, msg);
      }
      // free(s);
    }
  }
#line 2637 "syntaxAnalyser.tab.c"
    break;

  case 37: /* assign: variable '[' INT ']' OP_ASSIGN expressao  */
#line 958 "syntaxAnalyser.y"
                                           { 
    #if defined DEBUG
      printf("assign #2 \n");
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2648 "syntaxAnalyser.tab.c"
    break;

  case 38: /* expressao: '(' expressao ')'  */
#line 975 "syntaxAnalyser.y"
                    {
    #if defined DEBUG
      printf("expressao #6 \n"); 
    #endif
    (yyval.nd) = (yyvsp[-1].nd); 
  }
#line 2659 "syntaxAnalyser.tab.c"
    break;

  case 40: /* expressao: func_call  */
#line 982 "syntaxAnalyser.y"
              {
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2667 "syntaxAnalyser.tab.c"
    break;

  case 41: /* expressao_logica: OP_LOG op_expressao  */
#line 1005 "syntaxAnalyser.y"
                      { 
    #if defined DEBUG
      printf("expressao_logica #1 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2678 "syntaxAnalyser.tab.c"
    break;

  case 42: /* expressao_logica: '!' op_expressao  */
#line 1011 "syntaxAnalyser.y"
                   { 
    #if defined DEBUG
      printf("expressao_logica #2 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2689 "syntaxAnalyser.tab.c"
    break;

  case 43: /* expressao_logica: op_expressao OP_COMP op_expressao  */
#line 1017 "syntaxAnalyser.y"
                                    { 
    #if defined DEBUG
      printf("expressao_logica #3 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE, 'E', (yyvsp[-2].nd), (yyvsp[0].nd), "expressao_logica"); 
  }
#line 2700 "syntaxAnalyser.tab.c"
    break;

  case 44: /* expressao_logica: '(' op_expressao ')'  */
#line 1023 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("expressao_logica #4 \n"); 
    #endif
    (yyval.nd) = (yyvsp[-1].nd); 
  }
#line 2711 "syntaxAnalyser.tab.c"
    break;

  case 45: /* expressao_logica: op_expressao  */
#line 1029 "syntaxAnalyser.y"
               { 
    #if defined DEBUG
      printf("expressao_logica #5\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2722 "syntaxAnalyser.tab.c"
    break;

  case 46: /* expressao_logica: BOOL  */
#line 1035 "syntaxAnalyser.y"
       {
    #if defined DEBUG
      printf("expressao_logica #6\n"); 
    #endif
    (yyval.nd) = NULL;
  }
#line 2733 "syntaxAnalyser.tab.c"
    break;

  case 47: /* op_expressao: op_expressao OP_ARITM termo  */
#line 1044 "syntaxAnalyser.y"
                              { 
    #if defined DEBUG
      printf("op_expressao #1\n");
    #endif
    // printf("%s ll %s\n", $1->val, $3->val);
    // s_node* s1 = find_in_s_table($1->val);
    // s_node* s2 = find_in_s_table($3->val);
    int tm = types_match((yyvsp[-2].nd)->var_type, (yyvsp[0].nd)->var_type);
    if(tm){
      #if defined DEBUG
        printf("types OK\n");
      #endif
    } else {
      #if defined DEBUG
        printf("types MISSMATCH: %s | %s\n", (yyvsp[-2].nd)->var_type, (yyvsp[0].nd)->var_type);
      #endif
      char msg[50];
      sprintf(msg, "%s %s\n", (yyvsp[-2].nd)->var_type, (yyvsp[0].nd)->var_type);
      semantic_error(TYPES_MISSMATCH_ERROR, msg);
    }  
    (yyval.nd) = ins_node((yyvsp[-2].nd)->var_type, REGULAR_NODE, 'E', (yyvsp[-2].nd), (yyvsp[0].nd), (yyvsp[-1].operador)); 
  
  }
#line 2761 "syntaxAnalyser.tab.c"
    break;

  case 48: /* op_expressao: termo  */
#line 1067 "syntaxAnalyser.y"
          { 
    #if defined DEBUG
      printf("op_expressao #2\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2772 "syntaxAnalyser.tab.c"
    break;

  case 49: /* op_expressao: op_expressao ',' termo  */
#line 1073 "syntaxAnalyser.y"
                           {
      (yyval.nd) = ins_node(concat((yyvsp[-2].nd)->var_type, (yyvsp[0].nd)->var_type), REGULAR_NODE, 'T', (yyvsp[0].nd), (yyvsp[-2].nd), "tuple_args");
  }
#line 2780 "syntaxAnalyser.tab.c"
    break;

  case 50: /* termo: variable  */
#line 1079 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("termo #1 \n");
    #endif
    // $$ = ins_node_symbol($1, 'S','V', $1);
    s_node* s = find_in_s_table((yyvsp[0].str));
    if(s != NULL){
      (yyval.nd) = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
    } else {
      (yyval.nd) = ins_node("-", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
    }
    // free(s);
  }
#line 2798 "syntaxAnalyser.tab.c"
    break;

  case 51: /* termo: INT  */
#line 1092 "syntaxAnalyser.y"
      { 
    #if defined DEBUG
      printf("termo #2 \n");
    #endif
    // $$ = NULL; 
    (yyval.nd) = ins_node("int", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
  }
#line 2810 "syntaxAnalyser.tab.c"
    break;

  case 52: /* termo: FLOAT  */
#line 1099 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("termo #3 \n");
    #endif
    (yyval.nd) = ins_node("float", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
  }
#line 2821 "syntaxAnalyser.tab.c"
    break;

  case 53: /* termo: variable '[' INT ']'  */
#line 1105 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("termo #4 \n");
    #endif
    s_node* s = find_in_s_table((yyvsp[-3].str));
    (yyval.nd) = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, (yyvsp[-3].str));
    // free(s);
  }
#line 2834 "syntaxAnalyser.tab.c"
    break;

  case 54: /* termo: palavra  */
#line 1113 "syntaxAnalyser.y"
         {
  #if defined DEBUG
    printf("termo #5 \n");
  #endif
  (yyval.nd) = ins_node("char", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
}
#line 2845 "syntaxAnalyser.tab.c"
    break;

  case 55: /* scan: SCAN '(' variable ')'  */
#line 1122 "syntaxAnalyser.y"
                        { 
    #if defined DEBUG
      printf("scan #1 \n"); 
    #endif
  }
#line 2855 "syntaxAnalyser.tab.c"
    break;

  case 56: /* print: PRINT '(' termo ')' ';'  */
#line 1130 "syntaxAnalyser.y"
                          { 
    #if defined DEBUG
      printf("print #1 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE, 'P', NULL, (yyvsp[-2].nd), "print"); 
  }
#line 2866 "syntaxAnalyser.tab.c"
    break;

  case 57: /* func_call: ID '(' func_args ')'  */
#line 1139 "syntaxAnalyser.y"
                      {
    s_node* aux = find_in_s_table((yyvsp[-3].id));
    (yyval.nd) = ins_node_func_call(aux->var_type, REGULAR_NODE,'F', NULL, (yyvsp[-1].nd), "func_call", (yyvsp[-3].id)); 
    check_params((yyval.nd), (yyvsp[-3].id));
    // free(aux);
  }
#line 2877 "syntaxAnalyser.tab.c"
    break;

  case 58: /* func_call: ID '(' ')'  */
#line 1145 "syntaxAnalyser.y"
              {
    s_node* aux = find_in_s_table((yyvsp[-2].id));
    (yyval.nd) = ins_node_func_call(aux->var_type, REGULAR_NODE,'F', NULL, NULL, "func_call", (yyvsp[-2].id)); 
    check_params((yyval.nd), (yyvsp[-2].id));
    // free(aux);
  }
#line 2888 "syntaxAnalyser.tab.c"
    break;

  case 59: /* func_args: func_args ',' func_arg  */
#line 1154 "syntaxAnalyser.y"
                         {
    (yyval.nd) = ins_node("-", REGULAR_NODE,'F', (yyvsp[-2].nd), (yyvsp[0].nd), "func_args"); 
  }
#line 2896 "syntaxAnalyser.tab.c"
    break;

  case 60: /* func_args: func_arg  */
#line 1157 "syntaxAnalyser.y"
             {
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2904 "syntaxAnalyser.tab.c"
    break;

  case 61: /* func_arg: ID  */
#line 1163 "syntaxAnalyser.y"
     { 
    s_node* s = find_in_s_table((yyvsp[0].id));
    if(s == NULL){ // nao declarou a variavel ainda
      semantic_error(NO_DECLARATION_ERROR, (yyvsp[0].id));
      (yyval.nd) = ins_node("-", REGULAR_NODE,'A', NULL, NULL, "-");  
    } else {
      (yyval.nd) = ins_node(s->var_type, REGULAR_NODE,'A', NULL, NULL, concat((yyvsp[0].id), concat(SCOPE_SEPARATOR, s_stack->id))); 
    }
    // free(s);
  }
#line 2919 "syntaxAnalyser.tab.c"
    break;

  case 62: /* func_arg: ID '[' ID ']'  */
#line 1173 "syntaxAnalyser.y"
                  { 
    s_node* s = find_in_s_table((yyvsp[-3].id));
    if(s == NULL){ // nao declarou a variavel ainda
      semantic_error(NO_DECLARATION_ERROR, (yyvsp[-3].id));
      // $$ = ins_node("-", REGULAR_NODE,'A', NULL, NULL, "-");  
    } else {
      // $$ = ins_node(s->var_type, REGULAR_NODE,'A', NULL, NULL, $1); 
    }
    (yyval.nd) = ins_node("-", REGULAR_NODE,'A', NULL, NULL, (yyvsp[-3].id));
    // free(s);
  }
#line 2935 "syntaxAnalyser.tab.c"
    break;

  case 63: /* func_arg: INT  */
#line 1184 "syntaxAnalyser.y"
        { (yyval.nd) = ins_node("int", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 2941 "syntaxAnalyser.tab.c"
    break;

  case 64: /* func_arg: FLOAT  */
#line 1185 "syntaxAnalyser.y"
          { (yyval.nd) = ins_node("float", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 2947 "syntaxAnalyser.tab.c"
    break;

  case 65: /* func_arg: palavra  */
#line 1186 "syntaxAnalyser.y"
            { (yyval.nd) = ins_node("char", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 2953 "syntaxAnalyser.tab.c"
    break;

  case 66: /* palavra: palavra LETRA  */
#line 1190 "syntaxAnalyser.y"
                { 
    #if defined DEBUG
      printf("palavra #1 \n");
    #endif
  }
#line 2963 "syntaxAnalyser.tab.c"
    break;

  case 67: /* palavra: palavra DIGITO  */
#line 1195 "syntaxAnalyser.y"
                 { 
    #if defined DEBUG
      printf("palavra #2 \n"); 
    #endif
  }
#line 2973 "syntaxAnalyser.tab.c"
    break;

  case 68: /* palavra: LETRA  */
#line 1200 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("palavra #3 \n"); 
    #endif
  }
#line 2983 "syntaxAnalyser.tab.c"
    break;

  case 69: /* palavra: DIGITO  */
#line 1205 "syntaxAnalyser.y"
         { 
    #if defined DEBUG
      printf("palavra #4 \n"); 
    #endif
  }
#line 2993 "syntaxAnalyser.tab.c"
    break;

  case 70: /* variable: ID  */
#line 1213 "syntaxAnalyser.y"
     {
    #if defined DEBUG
      printf("variable #1 \n"); 
    #endif
    s_node* s = find_in_s_table((yyvsp[0].id));
    if(s == NULL){ // nao declarou a variavel ainda
      semantic_error(NO_DECLARATION_ERROR, (yyvsp[0].id));
    }
    char *auxid = concat(SCOPE_SEPARATOR, s_stack->id);
    char *identifier = concat((yyvsp[0].id), auxid);
    (yyval.str) = identifier;
    // $$ = ins_node("-", REGULAR_NODE, 'V', NULL, NULL, $1); 
    // free(s);
  }
#line 3012 "syntaxAnalyser.tab.c"
    break;

  case 71: /* variable: ID PONTO ID  */
#line 1227 "syntaxAnalyser.y"
                {
    #if defined DEBUG
      printf("variable #1 \n"); 
    #endif
    s_node* s = find_in_s_table((yyvsp[-2].id));
    if(s == NULL){ // nao declarou a variavel ainda
      semantic_error(NO_DECLARATION_ERROR, (yyvsp[-2].id));
    }
    (yyval.str) = (yyvsp[-2].id);
    // $$ = ins_node("-", REGULAR_NODE, 'V', NULL, NULL, $1); 
    // free(s);
  }
#line 3029 "syntaxAnalyser.tab.c"
    break;


#line 3033 "syntaxAnalyser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1240 "syntaxAnalyser.y"


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
    // printf("\n\nAbstract Syntax Tree:\n");
    // print_tree(parser_tree, 0);
    // printLevelOrder(parser_tree);
    printf("\n");

    print_s_table();
  }
  // free_tree(parser_tree);
  #if defined DEBUG
    printf("Debug Mode...\nEscopos:\n");
    int i;
    for(i=0; i<scopes_count;i++){
      printf("%s\n", scopes_names[i]);
    }
  #endif

  char *pure_file_name = remove_extension(basename(argv[0]));
  generateTacFile(parser_tree, pure_file_name);
  return 0;
}
