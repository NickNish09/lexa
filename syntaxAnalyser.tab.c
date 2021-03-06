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
  #define TUPLE_TYPE 1003
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
  #define CODE_TUPLEARGS 443
  #define CODE_IF 444
  #define CODE_ELSE 445
  #define CODE_WHILE 446

typedef struct { char *key; int val; } t_symstruct;

static t_symstruct lookuptable[] = {
  { "assign", CODE_ASSIGN }, 
  { "print", CODE_PRINT },
  { "retorno", CODE_RETURN },
  { "tuple_args", CODE_TUPLEARGS },
  { "if", CODE_IF },
  { "else", CODE_ELSE },
  { "while", CODE_WHILE },
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
    char* return_string = NULL;
    char aux = ' ';
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
    } else if(number == TUPLE_TYPE){
      return "Tuple";
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
    for(s=s_table; s != NULL; s=s->hh.next) {
      printf("id: %15s | var_type: %7s | s_node_type: %10s | scope_level: %d | params_count: %d", s->id, s->var_type, stringBasedOnNumber(s->s_node_type), s->scope, s->params_count);
      if(s->s_node_type == FUNCTION_TYPE || s->s_node_type == TUPLE_TYPE){
        printf(" | params: ");
        int k;
        for(k=0;k<s->params_count;k++){
          printf("%s::%s | ", s->params_list[k+1]->var_type, s->params_list[k+1]->id);
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
      char *auxid = concat(SCOPE_SEPARATOR, scopes_names[i]);
      char *identifier = concat(id, auxid);
      
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

  aux_node->left = NULL;
  aux_node->right = NULL;
  aux_node->var_type = var_type;
  aux_node->val = id;
  aux_node->node_type = node_type;
  aux_node->node_kind = node_kind;

  int t = FUNCTION_TYPE;
  if(node_kind == 'V'){
    t = VARIABLE_TYPE;
  } else if(node_kind == 'T'){
    t = TUPLE_TYPE;
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
        s_node *aux = find_in_s_table(func_name);
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
    int shouldGenerateBools = FALSE;
    fputs(".table\n", tac_file);
    for(s=s_table; s != NULL; s=s->hh.next) {
      if(s->s_node_type != FUNCTION_TYPE){
        if(s->s_node_type == TUPLE_TYPE){
          int i;
          for(i=1; i<=s->params_count;i++){
            strcpy(aux, s->params_list[i]->var_type);
            strcat(aux, " ");
            strcat(aux, concat(s->id, concat(SCOPE_SEPARATOR, s->params_list[i]->id)));
            strcat(aux, "\n");
            fputs(aux, tac_file);
          }
        } else {
          if(strcmp(s->var_type, "bool") == 0){
            strcpy(aux, "int");
            shouldGenerateBools = TRUE;
          } else {
            strcpy(aux, s->var_type);
          }
          strcat(aux, " ");
          strcat(aux, s->id);
          strcat(aux, "\n");
          fputs(aux, tac_file);
        }
      }
    }
    if(shouldGenerateBools){
      strcpy(aux, "int true = 1\n");
      strcat(aux, "int false = 0\n");
      fputs(aux, tac_file);
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
      strcat(aux, concat(sub_tree->left->val, ", "));
      strcat(aux, sub_tree->right->val);
      strcat(aux, "\n");
    }

    return aux;
  }

  char * generate_tuple_print(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    s_node *s = find_in_s_table_plain(sub_tree->val);
    if(s != NULL){
      strcpy(aux, "print ");
      strcat(aux, concat(sub_tree->val, concat(SCOPE_SEPARATOR, s->params_list[1]->id)));
      strcat(aux, "\n");
      strcat(aux, "print ' '\nprintln ");
      strcat(aux, concat(sub_tree->val, concat(SCOPE_SEPARATOR, s->params_list[2]->id)));
      strcat(aux, "\n");
    }

    return aux;
  }

  char * generate_tuple_operation(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    s_node *s = find_in_s_table_plain(sub_tree->left->val);
    s_node *sr = find_in_s_table_plain(sub_tree->right->right->val);
    s_node *sl = find_in_s_table_plain(sub_tree->right->left->val);
    if(s != NULL){
      if(strcmp(sub_tree->right->val, "+") == 0){
        strcpy(aux, "add ");
      } else if(strcmp(sub_tree->right->val, "-") == 0){
        strcpy(aux, "sub ");
      } else if(strcmp(sub_tree->right->val, "*") == 0){
        strcpy(aux, "mul ");
      } else if(strcmp(sub_tree->right->val, "/") == 0){
        strcpy(aux, "div ");
      }
      strcat(aux, concat(sub_tree->left->val, concat(SCOPE_SEPARATOR, s->params_list[1]->id)));
      strcat(aux, concat(", ", concat(sub_tree->right->right->val, concat(SCOPE_SEPARATOR, sr->params_list[1]->id))));
      strcat(aux, concat(", ", concat(sub_tree->right->left->val, concat(SCOPE_SEPARATOR, sl->params_list[1]->id))));
      strcat(aux, "\n");

      if(strcmp(sub_tree->right->val, "+") == 0){
        strcat(aux, "add ");
      } else if(strcmp(sub_tree->right->val, "-") == 0){
        strcat(aux, "sub ");
      } else if(strcmp(sub_tree->right->val, "*") == 0){
        strcat(aux, "mul ");
      } else if(strcmp(sub_tree->right->val, "/") == 0){
        strcat(aux, "div ");
      }
      strcat(aux, concat(sub_tree->left->val, concat(SCOPE_SEPARATOR, s->params_list[2]->id)));
      strcat(aux, concat(", ", concat(sub_tree->right->right->val, concat(SCOPE_SEPARATOR, sr->params_list[2]->id))));
      strcat(aux, concat(", ", concat(sub_tree->right->left->val, concat(SCOPE_SEPARATOR, sl->params_list[2]->id))));
      strcat(aux, "\n");
    }

    return aux;
  }

  char * generate_tuple_instruction(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    s_node *s = find_in_s_table_plain(sub_tree->left->val);
    if(s != NULL){
      strcpy(aux, "mov ");
      strcat(aux, concat(sub_tree->left->val, concat(SCOPE_SEPARATOR, s->params_list[1]->id)));
      strcat(aux, concat(", ", sub_tree->right->right->val));
      strcat(aux, "\n");

      strcat(aux, "mov ");
      strcat(aux, concat(sub_tree->left->val, concat(SCOPE_SEPARATOR, s->params_list[2]->id)));
      strcat(aux, concat(", ", sub_tree->right->left->val));
      strcat(aux, "\n");
    }

    return aux;
  }

  int func_counter;
  char * generate_func_call_instruction(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));
    if(sub_tree->left != NULL){
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
      } else {
        strcat(aux, "param ");
        strcat(aux, concat(sub_tree->val, "\n"));
        func_counter++;
      }
    }

    return aux;
  }

  char * int_to_label(char* label, int i){
    char *aux = (char*)malloc(50* sizeof(char));
    char labelChar[12];
    sprintf(labelChar, "%d", i);
    strcat(aux, concat(label, labelChar));
    strcat(aux, "");
    return aux;
  }

  int whileLabelCounter = 1;
  int whileInstructionFound = FALSE;

  char * generate_loop_expression(node *tree){
    char *aux = (char*)malloc(50* sizeof(char));
    whileInstructionFound = TRUE;
    strcpy(aux, int_to_label("W", whileLabelCounter));
    strcat(aux, ":\n");
    if(strcmp(tree->val, ">") == 0){
      strcat(aux, "slt $2, ");
      strcat(aux, tree->right->val);
      strcat(aux, ", ");
      strcat(aux, tree->left->val);
      strcat(aux, "\n");
    } else if(strcmp(tree->val, ">=") == 0){
      strcat(aux, "sleq $2, ");
      strcat(aux, tree->right->val);
      strcat(aux, ", ");
      strcat(aux, tree->left->val);
      strcat(aux, "\n");
    } else if(strcmp(tree->val, "<") == 0){
      strcat(aux, "slt $2, ");
      strcat(aux, tree->left->val);
      strcat(aux, ", ");
      strcat(aux, tree->right->val);
      strcat(aux, "\n");
    } else if(strcmp(tree->val, "<=") == 0){
      strcat(aux, "sleq $2, ");
      strcat(aux, tree->left->val);
      strcat(aux, ", ");
      strcat(aux, tree->right->val);
      strcat(aux, "\n");
    } else if(strcmp(tree->val, "==") == 0){
      strcat(aux, "seq $2, ");
      strcat(aux, tree->right->val);
      strcat(aux, ", ");
      strcat(aux, tree->left->val);
      strcat(aux, "\n");
    }

    strcat(aux, concat("brz EXIT_", int_to_label("W", whileLabelCounter)));
    strcat(aux, ", $2\n");
    return aux;
  }

  int globalLabelCounter = 0;
  int nextInstructionShouldHaveLabel = FALSE;

  char * generate_conditional_instruction(node *sub_tree){
    char *aux = (char*)malloc(50* sizeof(char));

    if(strcmp(sub_tree->left->val, ">") == 0){
      strcpy(aux, "slt $0, ");
      strcat(aux, sub_tree->left->right->val);
      strcat(aux, ", ");
      strcat(aux, sub_tree->left->left->val);
    } else if(strcmp(sub_tree->left->val, "<") == 0){
      strcpy(aux, "slt $0, ");
      strcat(aux, sub_tree->left->left->val);
      strcat(aux, ", ");
      strcat(aux, sub_tree->left->right->val);
    } else if(strcmp(sub_tree->left->val, ">=") == 0){
      strcpy(aux, "sleq $0, ");
      strcat(aux, sub_tree->left->right->val);
      strcat(aux, ", ");
      strcat(aux, sub_tree->left->left->val);
    } else if(strcmp(sub_tree->left->val, "<=") == 0){
      strcpy(aux, "sleq $0, ");
      strcat(aux, sub_tree->left->left->val);
      strcat(aux, ", ");
      strcat(aux, sub_tree->left->right->val);
    } else {
      strcpy(aux, "mov $0, ");
      strcat(aux, sub_tree->left->val);
    }
    strcat(aux, "\nbrz ");
    char labelChar[12];
    sprintf(labelChar, "%d", globalLabelCounter);
    strcat(aux, concat("L", labelChar));
    strcat(aux, ", $0\n");
    nextInstructionShouldHaveLabel = TRUE;

    return aux;
  }

  void resolveNode(FILE *tac_file, node *tree){
    if(tree){
      char *aux = NULL;
      switch(keyfromstring(tree->val)){
        case CODE_ASSIGN:
          if(is_expression(tree->right->val)){
            if(strlen(tree->right->right->var_type) > 6){
              aux = generate_tuple_operation(tree);
            } else {
              aux = generate_aritm_instruction(tree->right);
              strcat(aux, generate_instruction("mov", tree->left->val, "$0", NULL));
            }
          } else if(strcmp(tree->right->val, "func_call") == 0){
            func_counter = 0;
            char func_counter_string[3];
            aux = generate_func_call_instruction(tree->right->right);
            sprintf(func_counter_string, "%d", func_counter);
            strcat(aux, generate_instruction("call", concat(SCOPE_SEPARATOR, tree->right->func_name), func_counter_string, NULL));
            strcat(aux, generate_instruction("pop", tree->left->val, NULL, NULL));
          } else if(strcmp(tree->right->val, "tuple_args") == 0){
            aux = generate_tuple_instruction(tree);
          } else {
            aux = generate_instruction("mov", tree->left->val, tree->right->val, NULL);
          }
          break;

        case CODE_PRINT:
          if(strlen(tree->right->var_type) > 6){
            aux = generate_tuple_print(tree->right);
          } else {
            aux = generate_instruction("println", tree->right->val, NULL, NULL);
          }
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
        case CODE_IF:
          aux = generate_conditional_instruction(tree);
          break;
        case CODE_WHILE:
          aux = generate_loop_expression(tree->left);
          break;
        default:
          switch(tree->node_kind){
            case FUNCTION_CHAR:
              aux = generate_instruction(concat(SCOPE_SEPARATOR, concat(tree->val, ":")), NULL, NULL, NULL);
              s_node* func_s_node = find_in_s_table(tree->val);
              int i;
              for(i=func_s_node->params_count; i> 0; i--){
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
      if(strcmp(tree->val, "while") == 0 || strcmp(tree->val, "if") == 0){
        resolveNode(tac_file, tree->right);
        if(aux != NULL){
          if(nextInstructionShouldHaveLabel){
            char labelChar[12];
            sprintf(labelChar, "%d", globalLabelCounter);
            strcpy(aux, concat(concat("L", labelChar), ":\nnop"));
            strcat(aux, "\n");
            fputs(aux, tac_file);
            nextInstructionShouldHaveLabel = FALSE;
            globalLabelCounter++;
          }
          if(whileInstructionFound){
            strcpy(aux, concat("jump ", int_to_label("W", whileLabelCounter)));
            strcat(aux, "\n");
            strcat(aux, int_to_label("EXIT_W",whileLabelCounter));
            strcat(aux, ":\n");
            strcat(aux, "nop\n");
            fputs(aux, tac_file);
            whileInstructionFound = FALSE;
            whileLabelCounter++;
          }
        }
      } else {
        resolveNode(tac_file, tree->left);
        resolveNode(tac_file, tree->right);
      }
    }
  }

  void generateCodeInTac(FILE *tac_file, node* tree){
    fputs(".code\njump __main\n", tac_file);
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

  char *tuple_gamb;
  char *tuple_gamb_kind;

#line 946 "syntaxAnalyser.tab.c"

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
#define YYLAST   231

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  156

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
       0,   912,   912,   921,   927,   936,   942,   948,   957,   978,
     987,   993,  1002,  1007,  1002,  1012,  1012,  1023,  1033,  1045,
    1057,  1069,  1079,  1085,  1094,  1100,  1106,  1112,  1118,  1138,
    1144,  1150,  1156,  1162,  1167,  1171,  1197,  1206,  1207,  1208,
    1231,  1237,  1243,  1249,  1255,  1264,  1284,  1290,  1293,  1315,
    1328,  1335,  1341,  1353,  1359,  1368,  1376,  1385,  1391,  1400,
    1403,  1409,  1420,  1431,  1432,  1433,  1437,  1442,  1447,  1452,
    1460,  1474
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

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      20,    -2,    22,    28,    20,   -72,   -72,   -72,    44,    45,
     -72,   -72,   -72,   -72,   -72,    38,     0,    51,    49,    61,
      22,   -72,    80,    85,    15,   -72,   -72,   201,    69,   -72,
      94,   108,   -72,    88,    89,    42,    -9,    90,    91,   -72,
      83,   -72,    96,    98,   -72,   -72,    -4,   100,   102,   120,
     106,     8,     8,   -72,   -72,   -72,   122,   -72,   -72,   -72,
     173,   118,    34,   112,   134,    63,   211,   135,   -72,   -72,
     -72,   135,   173,   139,   -72,   123,   -72,   201,   190,   190,
     190,   130,    82,   -72,   131,   190,   136,     7,   -72,   -72,
     -72,   -72,   152,   -72,   -72,   -72,   142,   -72,    60,   -72,
      34,   137,   146,   151,   -72,   138,   -72,   117,   190,     7,
      23,     7,   161,   211,   190,   211,   170,    73,   165,   171,
     195,   182,   -72,   186,   -72,   187,   197,   -72,   158,   202,
     201,   -72,     7,   -72,   201,   202,   -72,   -72,   196,   -72,
     -72,   -72,   173,   202,   211,   132,   147,   -72,   -72,   -72,
     221,   -72,   203,   201,   162,   -72
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     2,     4,     5,     7,     0,     0,
      10,     6,     1,     3,    11,    12,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     9,     8,     0,    19,    21,
       0,     0,    13,     0,     0,     0,    70,     0,     0,    34,
       0,    23,     0,     0,    31,    32,     0,     0,     0,     0,
       0,     0,     0,    54,    50,    51,    70,    69,    68,    27,
       0,     0,    53,    49,     0,     0,     0,     0,    16,    22,
      30,     0,     0,     0,    20,     0,    17,     0,     0,     0,
       0,     0,    44,    46,     0,     0,     0,    37,    39,    28,
      67,    66,     0,    71,    63,    64,    61,    58,     0,    60,
      65,     0,     0,     0,    35,     0,    18,     0,     0,    40,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    55,     0,     0,    14,     0,    43,
       0,    45,    42,    47,     0,    38,    29,    52,     0,    59,
      56,    33,     0,     0,     0,     0,     0,    62,    36,    48,
      24,    26,     0,     0,     0,    25
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -72,   -72,   -72,    58,   208,   -72,   -72,   -72,   -72,   -72,
     -72,   -71,   -37,   -72,   -68,   177,   -43,   -34,   -72,   -72,
     -58,   -72,   110,   -60,   -27
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    39,    11,     6,     7,    18,    50,    27,
      24,    40,    41,    42,    86,    81,    87,    83,    43,    44,
      45,    98,    99,    62,    63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    61,    88,    69,   104,   100,   107,    64,    82,    82,
       8,    53,    19,    46,    88,    72,    65,    54,    55,    30,
      56,    57,    58,    20,     1,    73,     9,   113,    12,    78,
     115,     2,   101,    79,    10,   109,   110,   111,    31,    80,
     102,    32,   117,   113,   103,    53,   115,    90,    91,   129,
      46,    54,    55,    22,    56,    57,    58,    16,     5,   145,
      23,   100,    13,   146,    17,   128,    59,    60,    14,    15,
      69,   132,    94,    95,   148,    96,    57,    58,    21,   131,
      46,   133,   154,   121,    88,    25,   122,     1,    33,    97,
      34,    35,    28,   113,     2,    36,   115,    29,    47,   135,
      37,    38,   113,    46,   114,   115,    48,    46,    69,    69,
     149,    68,    49,    51,    52,    66,    67,    69,    46,    46,
      70,     1,    33,    71,    34,    35,    46,    46,     2,    36,
      74,    75,    76,    77,    37,    38,     1,    33,    64,    34,
      35,    92,    89,     2,    36,   127,    93,    56,   105,    37,
      38,     1,    33,   106,    34,    35,   112,   116,     2,    36,
     150,   119,   118,   123,    37,    38,     1,    33,   126,    34,
      35,   120,   124,     2,    36,   151,    53,   125,   113,    37,
      38,   115,    54,    55,   143,    36,    57,    58,   130,   136,
     155,    94,    95,    53,    96,    57,    58,   134,    85,    54,
      55,   137,    56,    57,    58,     1,    33,   138,    34,    35,
     140,   141,     2,    36,    53,   108,   142,     0,    37,    38,
      54,    55,   144,    56,    57,    58,   147,   152,    26,    84,
     153,   139
};

static const yytype_int16 yycheck[] =
{
      27,    35,    60,    40,    72,    65,    77,    16,    51,    52,
      12,     3,    12,    40,    72,    19,    25,     9,    10,     4,
      12,    13,    14,    23,     4,    29,     4,    20,     0,    21,
      23,    11,    66,    25,    12,    78,    79,    80,    23,    31,
      67,    26,    85,    20,    71,     3,    23,    13,    14,    26,
      77,     9,    10,     4,    12,    13,    14,    12,     0,   130,
      11,   121,     4,   134,    26,   108,    24,    25,    24,    25,
     107,   114,     9,    10,   142,    12,    13,    14,    27,   113,
     107,   115,   153,    23,   142,    24,    26,     4,     5,    26,
       7,     8,    12,    20,    11,    12,    23,    12,    29,    26,
      17,    18,    20,   130,    22,    23,    12,   134,   145,   146,
     144,    28,     4,    25,    25,    25,    25,   154,   145,   146,
      24,     4,     5,    25,     7,     8,   153,   154,    11,    12,
      30,    29,    12,    27,    17,    18,     4,     5,    16,     7,
       8,    29,    24,    11,    12,    28,    12,    12,     9,    17,
      18,     4,     5,    30,     7,     8,    26,    26,    11,    12,
      28,     9,    26,    26,    17,    18,     4,     5,    30,     7,
       8,    29,    26,    11,    12,    28,     3,    26,    20,    17,
      18,    23,     9,    10,    26,    12,    13,    14,    27,    24,
      28,     9,    10,     3,    12,    13,    14,    27,    25,     9,
      10,    30,    12,    13,    14,     4,     5,    12,     7,     8,
      24,    24,    11,    12,     3,    25,    19,    -1,    17,    18,
       9,    10,    20,    12,    13,    14,    30,     6,    20,    52,
      27,   121
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
      40,    25,    25,     3,     9,    10,    12,    13,    14,    24,
      25,    49,    55,    56,    16,    25,    25,    25,    28,    44,
      24,    25,    19,    29,    30,    29,    12,    27,    21,    25,
      31,    47,    48,    49,    47,    25,    46,    48,    52,    24,
      13,    14,    29,    12,     9,    10,    12,    26,    53,    54,
      55,    49,    56,    56,    46,     9,    30,    43,    25,    48,
      48,    48,    26,    20,    22,    23,    26,    48,    26,     9,
      29,    23,    26,    26,    26,    26,    30,    28,    48,    26,
      27,    49,    48,    49,    27,    26,    24,    30,    12,    54,
      24,    24,    19,    26,    20,    43,    43,    30,    46,    49,
      28,    28,     6,    27,    43,    28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    36,    36,
      36,    37,    39,    40,    38,    41,    38,    42,    42,    42,
      42,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    45,    45,    46,    46,    46,
      47,    47,    47,    47,    47,    48,    48,    48,    48,    49,
      49,    49,    49,    49,    49,    50,    51,    52,    52,    53,
      53,    54,    54,    54,    54,    54,    55,    55,    55,    55,
      56,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     1,     4,     4,
       1,     3,     0,     0,    10,     0,     8,     4,     5,     2,
       4,     2,     2,     1,     7,    11,     7,     2,     3,     5,
       2,     1,     1,     5,     1,     3,     6,     1,     3,     1,
       2,     2,     3,     3,     1,     3,     1,     3,     5,     1,
       1,     1,     4,     1,     1,     4,     5,     4,     3,     3,
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
#line 912 "syntaxAnalyser.y"
              { 
    parser_tree = (yyvsp[0].nd);
    #if defined DEBUG
      printf("tree initialized\n"); 
    #endif 
  }
#line 2472 "syntaxAnalyser.tab.c"
    break;

  case 3: /* declaracoes: declaracoes declaracao  */
#line 921 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("declaracoes \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'D', (yyvsp[-1].nd), (yyvsp[0].nd), "decl"); 
  }
#line 2483 "syntaxAnalyser.tab.c"
    break;

  case 4: /* declaracoes: declaracao  */
#line 927 "syntaxAnalyser.y"
             { 
    #if defined DEBUG
      printf("declaracao \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2494 "syntaxAnalyser.tab.c"
    break;

  case 5: /* declaracao: var_decl  */
#line 936 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("var_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2505 "syntaxAnalyser.tab.c"
    break;

  case 6: /* declaracao: TUPLE declaracao_tupla  */
#line 942 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("tuple_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2516 "syntaxAnalyser.tab.c"
    break;

  case 7: /* declaracao: func_decl  */
#line 948 "syntaxAnalyser.y"
            { 
    #if defined DEBUG
      printf("func_decl\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2527 "syntaxAnalyser.tab.c"
    break;

  case 8: /* declaracao_tupla: TIPO ID ',' declaracao_tupla  */
#line 957 "syntaxAnalyser.y"
                               { 
    #if defined DEBUG
      printf("declaracao_tupla #1\n"); 
      printf("var: %s | type: %s\n", (yyvsp[0].nd)->val, (yyvsp[0].nd)->var_type);
      printf("should_type: %s\n", (yyvsp[-3].tipo));
    #endif
    s_node* s = find_in_s_table((yyvsp[0].nd)->val);
    s->var_type = concat((yyvsp[-3].tipo),(yyvsp[0].nd)->var_type);
    (yyvsp[0].nd)->var_type = concat((yyvsp[-3].tipo), (yyvsp[0].nd)->var_type);
    s->params_count++;
    s_node *aux = (s_node *)malloc(sizeof *aux);
    aux->id = (yyvsp[-2].id);
    aux->var_type = (yyvsp[-3].tipo);
    s->params_list[s->params_count] = aux;
    s->params_count++;
    s_node *aux2 = (s_node *)malloc(sizeof *aux);
    aux2->id = tuple_gamb;
    aux2->var_type = tuple_gamb_kind;
    s->params_list[s->params_count] = aux2;
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2553 "syntaxAnalyser.tab.c"
    break;

  case 9: /* declaracao_tupla: TIPO ID ID ';'  */
#line 978 "syntaxAnalyser.y"
                {
    #if defined DEBUG
      printf("declaracao_tupla #2\n"); 
    #endif
    // $$ = $1;
    tuple_gamb = (yyvsp[-2].id);
    tuple_gamb_kind = (yyvsp[-3].tipo);
    (yyval.nd) = ins_node_symbol((yyvsp[-3].tipo), SYMBOL_NODE,'T', (yyvsp[-1].id));
  }
#line 2567 "syntaxAnalyser.tab.c"
    break;

  case 10: /* declaracao_tupla: ID  */
#line 987 "syntaxAnalyser.y"
     {
    (yyval.nd) = ins_node_symbol((yyvsp[0].id), SYMBOL_NODE,'T', (yyvsp[0].id));;
  }
#line 2575 "syntaxAnalyser.tab.c"
    break;

  case 11: /* var_decl: TIPO ID ';'  */
#line 993 "syntaxAnalyser.y"
              { 
    #if defined DEBUG
      printf("var_decl \n"); 
    #endif
    (yyval.nd) = ins_node_symbol((yyvsp[-2].tipo), SYMBOL_NODE,'V', (yyvsp[-1].id));
  }
#line 2586 "syntaxAnalyser.tab.c"
    break;

  case 12: /* $@1: %empty  */
#line 1002 "syntaxAnalyser.y"
              {
    // $<nd>$ = ins_node_symbol($1, SYMBOL_NODE,'F', $2); 
    add_to_s_table((yyvsp[-1].id), (yyvsp[-2].tipo), FUNCTION_TYPE, 0);
    s_push((yyvsp[-1].id));
  }
#line 2596 "syntaxAnalyser.tab.c"
    break;

  case 13: /* $@2: %empty  */
#line 1007 "syntaxAnalyser.y"
                 { ; }
#line 2602 "syntaxAnalyser.tab.c"
    break;

  case 14: /* func_decl: TIPO ID '(' $@1 parm_tipos ')' $@2 '{' cod_blocks '}'  */
#line 1008 "syntaxAnalyser.y"
                     { 
      (yyval.nd) = ins_node((yyvsp[-9].tipo), REGULAR_NODE, FUNCTION_CHAR, (yyvsp[-5].nd), (yyvsp[-1].nd), (yyvsp[-8].id));
      s_pop(); 
  }
#line 2611 "syntaxAnalyser.tab.c"
    break;

  case 15: /* $@3: %empty  */
#line 1012 "syntaxAnalyser.y"
                      {
  #if defined DEBUG
    printf("func_decl #4 \n");
  #endif
  add_to_s_table((yyvsp[-3].id), (yyvsp[-4].tipo), FUNCTION_TYPE, 0); 
  s_push((yyvsp[-3].id));
}
#line 2623 "syntaxAnalyser.tab.c"
    break;

  case 16: /* func_decl: TIPO ID '(' ')' '{' $@3 cod_blocks '}'  */
#line 1019 "syntaxAnalyser.y"
                { (yyval.nd) = ins_node((yyvsp[-7].tipo), REGULAR_NODE, FUNCTION_CHAR, NULL, (yyvsp[-1].nd), (yyvsp[-6].id)); s_pop(); }
#line 2629 "syntaxAnalyser.tab.c"
    break;

  case 17: /* parm_tipos: parm_tipos ',' TIPO ID  */
#line 1023 "syntaxAnalyser.y"
                         { 
    #if defined DEBUG
      printf("parm_tipos #1 \n"); 
    #endif
    s_node* aux = add_to_s_table((yyvsp[0].id), (yyvsp[-1].tipo), VARIABLE_TYPE, 0);
    s_node* func = find_in_s_table(s_stack->id);
    func->params_count++;
    func->params_list[func->params_count] = aux;
    (yyval.nd) = (yyvsp[-3].nd);
  }
#line 2644 "syntaxAnalyser.tab.c"
    break;

  case 18: /* parm_tipos: parm_tipos TIPO ID '[' ']'  */
#line 1033 "syntaxAnalyser.y"
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
#line 2661 "syntaxAnalyser.tab.c"
    break;

  case 19: /* parm_tipos: TIPO ID  */
#line 1045 "syntaxAnalyser.y"
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
#line 2678 "syntaxAnalyser.tab.c"
    break;

  case 20: /* parm_tipos: TIPO ID '[' ']'  */
#line 1057 "syntaxAnalyser.y"
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
#line 2695 "syntaxAnalyser.tab.c"
    break;

  case 21: /* parm_tipos: TUPLE ID  */
#line 1069 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("parm_tipos #6\n"); 
    #endif
    // $$ = NULL; 
    (yyval.nd) = ins_node_symbol((yyvsp[-1].str), 'S','V', (yyvsp[0].id));
  }
#line 2707 "syntaxAnalyser.tab.c"
    break;

  case 22: /* cod_blocks: cod_blocks cod_block  */
#line 1079 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("cod_blocks #1\n"); 
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'C', (yyvsp[-1].nd), (yyvsp[0].nd), "code_block"); 
  }
#line 2718 "syntaxAnalyser.tab.c"
    break;

  case 23: /* cod_blocks: cod_block  */
#line 1085 "syntaxAnalyser.y"
             {
    #if defined DEBUG
      printf("cod_blocks #2\n");
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
   }
#line 2729 "syntaxAnalyser.tab.c"
    break;

  case 24: /* cod_block: IF '(' expressao_logica ')' '{' cod_blocks '}'  */
#line 1094 "syntaxAnalyser.y"
                                                 {
    #if defined DEBUG
      printf("cod_block #1 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'I', (yyvsp[-4].nd), (yyvsp[-1].nd), "if"); 
  }
#line 2740 "syntaxAnalyser.tab.c"
    break;

  case 25: /* cod_block: IF '(' expressao_logica ')' '{' cod_blocks '}' ELSE '{' cod_blocks '}'  */
#line 1100 "syntaxAnalyser.y"
                                                                         { 
    #if defined DEBUG
      printf("cod_block #2 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'I', (yyvsp[-8].nd), ins_node("-", REGULAR_NODE,'I', (yyvsp[-5].nd), (yyvsp[-1].nd), "cb"), "if-else"); 
  }
#line 2751 "syntaxAnalyser.tab.c"
    break;

  case 26: /* cod_block: LACOS '(' expressao_logica ')' '{' cod_blocks '}'  */
#line 1106 "syntaxAnalyser.y"
                                                    { 
    #if defined DEBUG
      printf("cod_block #3 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'L', (yyvsp[-4].nd), (yyvsp[-1].nd), "while"); 
  }
#line 2762 "syntaxAnalyser.tab.c"
    break;

  case 27: /* cod_block: RETORNO ';'  */
#line 1112 "syntaxAnalyser.y"
              { 
    #if defined DEBUG
      printf("cod_block #4 \n");
    #endif
    (yyval.nd) = NULL; 
  }
#line 2773 "syntaxAnalyser.tab.c"
    break;

  case 28: /* cod_block: RETORNO termo ';'  */
#line 1118 "syntaxAnalyser.y"
                    {
    #if defined DEBUG
      printf("cod_block #4.5 \n");
    #endif
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
#line 2798 "syntaxAnalyser.tab.c"
    break;

  case 29: /* cod_block: RETORNO '(' expressao ')' ';'  */
#line 1138 "syntaxAnalyser.y"
                                {
    #if defined DEBUG
      printf("cod_block #5 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE,'R', NULL, (yyvsp[-2].nd), "retorno"); 
  }
#line 2809 "syntaxAnalyser.tab.c"
    break;

  case 30: /* cod_block: assign ';'  */
#line 1144 "syntaxAnalyser.y"
             { 
    #if defined DEBUG
      printf("cod_block #6 \n"); 
    #endif
    (yyval.nd) = (yyvsp[-1].nd);
  }
#line 2820 "syntaxAnalyser.tab.c"
    break;

  case 31: /* cod_block: print  */
#line 1150 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("cod_block #7 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2831 "syntaxAnalyser.tab.c"
    break;

  case 32: /* cod_block: func_call  */
#line 1156 "syntaxAnalyser.y"
            { 
  #if defined DEBUG
    printf("cod_block #7.5 \n"); 
  #endif
  (yyval.nd) = (yyvsp[0].nd);
  }
#line 2842 "syntaxAnalyser.tab.c"
    break;

  case 33: /* cod_block: scan '(' variable ')' ';'  */
#line 1162 "syntaxAnalyser.y"
                            {
    #if defined DEBUG
      printf("cod_block #10 \n"); 
    #endif
  }
#line 2852 "syntaxAnalyser.tab.c"
    break;

  case 34: /* cod_block: declaracao  */
#line 1167 "syntaxAnalyser.y"
             { (yyval.nd) = (yyvsp[0].nd) ;}
#line 2858 "syntaxAnalyser.tab.c"
    break;

  case 35: /* assign: variable OP_ASSIGN expressao  */
#line 1171 "syntaxAnalyser.y"
                               { 
    // $$ = $3;
    
    #if defined DEBUG
      printf("assign #1 \n");
      printf("TIIIPO: %s | %s\n", (yyvsp[0].nd)->var_type, s->var_type);
    #endif
    s_node* s = find_in_s_table_plain((yyvsp[-2].str));
    s_node* ss = find_in_s_table_plain((yyvsp[0].nd)->val);
    if(s != NULL){
      if(ss != NULL){
        if(!types_match(s->var_type, ss->var_type)){
          char msg[50];
          sprintf(msg, "%s %s\n", ss->var_type, s->var_type);
          semantic_error(TYPES_MISSMATCH_ERROR, msg);
        }
      } else {
        if(!types_match((yyvsp[0].nd)->var_type, s->var_type)){
          char msg[50];
          sprintf(msg, "%s %s\n", (yyvsp[0].nd)->var_type, s->var_type);
          semantic_error(TYPES_MISSMATCH_ERROR, msg);
        }
      }
    }
    (yyval.nd) = ins_node((yyvsp[0].nd)->var_type, 'C','R', ins_node("-", 'C','R', NULL, NULL, (yyvsp[-2].str)), (yyvsp[0].nd), "assign");
  }
#line 2889 "syntaxAnalyser.tab.c"
    break;

  case 36: /* assign: variable '[' INT ']' OP_ASSIGN expressao  */
#line 1197 "syntaxAnalyser.y"
                                           { 
    #if defined DEBUG
      printf("assign #2 \n");
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2900 "syntaxAnalyser.tab.c"
    break;

  case 37: /* expressao: op_expressao  */
#line 1206 "syntaxAnalyser.y"
               { (yyval.nd) = (yyvsp[0].nd); }
#line 2906 "syntaxAnalyser.tab.c"
    break;

  case 38: /* expressao: '(' op_expressao ')'  */
#line 1207 "syntaxAnalyser.y"
                         { (yyval.nd) = (yyvsp[-1].nd); }
#line 2912 "syntaxAnalyser.tab.c"
    break;

  case 39: /* expressao: func_call  */
#line 1208 "syntaxAnalyser.y"
              {
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 2920 "syntaxAnalyser.tab.c"
    break;

  case 40: /* expressao_logica: OP_LOG op_expressao  */
#line 1231 "syntaxAnalyser.y"
                      { 
    #if defined DEBUG
      printf("expressao_logica #1 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2931 "syntaxAnalyser.tab.c"
    break;

  case 41: /* expressao_logica: '!' op_expressao  */
#line 1237 "syntaxAnalyser.y"
                   { 
    #if defined DEBUG
      printf("expressao_logica #2 \n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2942 "syntaxAnalyser.tab.c"
    break;

  case 42: /* expressao_logica: op_expressao OP_COMP op_expressao  */
#line 1243 "syntaxAnalyser.y"
                                    { 
    #if defined DEBUG
      printf("expressao_logica #3 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE, 'E', (yyvsp[-2].nd), (yyvsp[0].nd), (yyvsp[-1].operador)); 
  }
#line 2953 "syntaxAnalyser.tab.c"
    break;

  case 43: /* expressao_logica: '(' op_expressao ')'  */
#line 1249 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("expressao_logica #4 \n"); 
    #endif
    (yyval.nd) = (yyvsp[-1].nd); 
  }
#line 2964 "syntaxAnalyser.tab.c"
    break;

  case 44: /* expressao_logica: op_expressao  */
#line 1255 "syntaxAnalyser.y"
               { 
    #if defined DEBUG
      printf("expressao_logica #5\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd); 
  }
#line 2975 "syntaxAnalyser.tab.c"
    break;

  case 45: /* op_expressao: op_expressao OP_ARITM termo  */
#line 1264 "syntaxAnalyser.y"
                              { 
    #if defined DEBUG
      printf("op_expressao #1\n");
    #endif
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
#line 3000 "syntaxAnalyser.tab.c"
    break;

  case 46: /* op_expressao: termo  */
#line 1284 "syntaxAnalyser.y"
          { 
    #if defined DEBUG
      printf("op_expressao #2\n"); 
    #endif
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 3011 "syntaxAnalyser.tab.c"
    break;

  case 47: /* op_expressao: op_expressao ',' termo  */
#line 1290 "syntaxAnalyser.y"
                           {
      (yyval.nd) = ins_node(concat((yyvsp[-2].nd)->var_type, (yyvsp[0].nd)->var_type), REGULAR_NODE, 'T', (yyvsp[0].nd), (yyvsp[-2].nd), "tuple_args");
  }
#line 3019 "syntaxAnalyser.tab.c"
    break;

  case 48: /* op_expressao: '(' op_expressao ')' OP_ARITM termo  */
#line 1293 "syntaxAnalyser.y"
                                        { 
      #if defined DEBUG
        printf("op_expressao #1\n");
      #endif
      int tm = types_match((yyvsp[-3].nd)->var_type, (yyvsp[0].nd)->var_type);
      if(tm){
        #if defined DEBUG
          printf("types OK\n");
        #endif
      } else {
        #if defined DEBUG
          printf("types MISSMATCH: %s | %s\n", (yyvsp[-3].nd)->var_type, (yyvsp[0].nd)->var_type);
        #endif
        char msg[50];
        sprintf(msg, "%s %s\n", (yyvsp[-3].nd)->var_type, (yyvsp[0].nd)->var_type);
        semantic_error(TYPES_MISSMATCH_ERROR, msg);
      }  
      (yyval.nd) = ins_node((yyvsp[-3].nd)->var_type, REGULAR_NODE, 'E', (yyvsp[-3].nd), (yyvsp[0].nd), (yyvsp[-1].operador)); 
  }
#line 3043 "syntaxAnalyser.tab.c"
    break;

  case 49: /* termo: variable  */
#line 1315 "syntaxAnalyser.y"
           { 
    #if defined DEBUG
      printf("termo #1 \n");
    #endif
    // $$ = ins_node_symbol($1, 'S','V', $1);
    s_node* s = find_in_s_table_plain((yyvsp[0].str));
    if(s != NULL){
      (yyval.nd) = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
    } else {
      (yyval.nd) = ins_node("-", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
    }
    // free(s);
  }
#line 3061 "syntaxAnalyser.tab.c"
    break;

  case 50: /* termo: INT  */
#line 1328 "syntaxAnalyser.y"
      { 
    #if defined DEBUG
      printf("termo #2 \n");
    #endif
    // $$ = NULL; 
    (yyval.nd) = ins_node("int", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
  }
#line 3073 "syntaxAnalyser.tab.c"
    break;

  case 51: /* termo: FLOAT  */
#line 1335 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("termo #3 \n");
    #endif
    (yyval.nd) = ins_node("float", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
  }
#line 3084 "syntaxAnalyser.tab.c"
    break;

  case 52: /* termo: variable '[' INT ']'  */
#line 1341 "syntaxAnalyser.y"
                       { 
    #if defined DEBUG
      printf("termo #4 \n");
    #endif
    s_node* s = find_in_s_table((yyvsp[-3].str));
    if(s != NULL){
      (yyval.nd) = ins_node(s->var_type, REGULAR_NODE, 'E', NULL, NULL, (yyvsp[-3].str));
    } else {
      (yyval.nd) = ins_node("-", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[-3].str)); 
    }
    // free(s);
  }
#line 3101 "syntaxAnalyser.tab.c"
    break;

  case 53: /* termo: palavra  */
#line 1353 "syntaxAnalyser.y"
         {
  #if defined DEBUG
    printf("termo #5 \n");
  #endif
  (yyval.nd) = ins_node("char", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
}
#line 3112 "syntaxAnalyser.tab.c"
    break;

  case 54: /* termo: BOOL  */
#line 1359 "syntaxAnalyser.y"
       { 
    #if defined DEBUG
      printf("termo #3 \n");
    #endif
    (yyval.nd) = ins_node("bool", REGULAR_NODE, 'E', NULL, NULL, (yyvsp[0].str));
  }
#line 3123 "syntaxAnalyser.tab.c"
    break;

  case 55: /* scan: SCAN '(' variable ')'  */
#line 1368 "syntaxAnalyser.y"
                        { 
    #if defined DEBUG
      printf("scan #1 \n"); 
    #endif
  }
#line 3133 "syntaxAnalyser.tab.c"
    break;

  case 56: /* print: PRINT '(' termo ')' ';'  */
#line 1376 "syntaxAnalyser.y"
                          { 
    #if defined DEBUG
      printf("print #1 \n");
    #endif
    (yyval.nd) = ins_node("-", REGULAR_NODE, 'P', NULL, (yyvsp[-2].nd), "print"); 
  }
#line 3144 "syntaxAnalyser.tab.c"
    break;

  case 57: /* func_call: ID '(' func_args ')'  */
#line 1385 "syntaxAnalyser.y"
                      {
    s_node* aux = find_in_s_table((yyvsp[-3].id));
    (yyval.nd) = ins_node_func_call(aux->var_type, REGULAR_NODE,'F', NULL, (yyvsp[-1].nd), "func_call", (yyvsp[-3].id)); 
    check_params((yyval.nd), (yyvsp[-3].id));
    // free(aux);
  }
#line 3155 "syntaxAnalyser.tab.c"
    break;

  case 58: /* func_call: ID '(' ')'  */
#line 1391 "syntaxAnalyser.y"
              {
    s_node* aux = find_in_s_table((yyvsp[-2].id));
    (yyval.nd) = ins_node_func_call(aux->var_type, REGULAR_NODE,'F', NULL, NULL, "func_call", (yyvsp[-2].id)); 
    check_params((yyval.nd), (yyvsp[-2].id));
    // free(aux);
  }
#line 3166 "syntaxAnalyser.tab.c"
    break;

  case 59: /* func_args: func_args ',' func_arg  */
#line 1400 "syntaxAnalyser.y"
                         {
    (yyval.nd) = ins_node("-", REGULAR_NODE,'F', (yyvsp[-2].nd), (yyvsp[0].nd), "func_args"); 
  }
#line 3174 "syntaxAnalyser.tab.c"
    break;

  case 60: /* func_args: func_arg  */
#line 1403 "syntaxAnalyser.y"
             {
    (yyval.nd) = (yyvsp[0].nd);
  }
#line 3182 "syntaxAnalyser.tab.c"
    break;

  case 61: /* func_arg: ID  */
#line 1409 "syntaxAnalyser.y"
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
#line 3197 "syntaxAnalyser.tab.c"
    break;

  case 62: /* func_arg: ID '[' ID ']'  */
#line 1420 "syntaxAnalyser.y"
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
#line 3213 "syntaxAnalyser.tab.c"
    break;

  case 63: /* func_arg: INT  */
#line 1431 "syntaxAnalyser.y"
        { (yyval.nd) = ins_node("int", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 3219 "syntaxAnalyser.tab.c"
    break;

  case 64: /* func_arg: FLOAT  */
#line 1432 "syntaxAnalyser.y"
          { (yyval.nd) = ins_node("float", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 3225 "syntaxAnalyser.tab.c"
    break;

  case 65: /* func_arg: palavra  */
#line 1433 "syntaxAnalyser.y"
            { (yyval.nd) = ins_node("char", REGULAR_NODE,'A', NULL, NULL, (yyvsp[0].str)); }
#line 3231 "syntaxAnalyser.tab.c"
    break;

  case 66: /* palavra: palavra LETRA  */
#line 1437 "syntaxAnalyser.y"
                { 
    #if defined DEBUG
      printf("palavra #1 \n");
    #endif
  }
#line 3241 "syntaxAnalyser.tab.c"
    break;

  case 67: /* palavra: palavra DIGITO  */
#line 1442 "syntaxAnalyser.y"
                 { 
    #if defined DEBUG
      printf("palavra #2 \n"); 
    #endif
  }
#line 3251 "syntaxAnalyser.tab.c"
    break;

  case 68: /* palavra: LETRA  */
#line 1447 "syntaxAnalyser.y"
        { 
    #if defined DEBUG
      printf("palavra #3 \n"); 
    #endif
  }
#line 3261 "syntaxAnalyser.tab.c"
    break;

  case 69: /* palavra: DIGITO  */
#line 1452 "syntaxAnalyser.y"
         { 
    #if defined DEBUG
      printf("palavra #4 \n"); 
    #endif
  }
#line 3271 "syntaxAnalyser.tab.c"
    break;

  case 70: /* variable: ID  */
#line 1460 "syntaxAnalyser.y"
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
#line 3290 "syntaxAnalyser.tab.c"
    break;

  case 71: /* variable: ID PONTO ID  */
#line 1474 "syntaxAnalyser.y"
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
#line 3307 "syntaxAnalyser.tab.c"
    break;


#line 3311 "syntaxAnalyser.tab.c"

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

#line 1487 "syntaxAnalyser.y"


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
    printf("\n");

    print_s_table();
  } else {
    printErrors();
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
  if(!has_error){
    generateTacFile(parser_tree, pure_file_name);
  }
  return 0;
}
