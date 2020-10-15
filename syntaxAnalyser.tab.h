/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OP_ARITM = 258,
     OP_COMP = 259,
     OP_LOG = 260,
     OP_ASSIGN = 261,
     BOOL = 262,
     TIPO = 263,
     CONDICOES = 264,
     LACOS = 265,
     RETORNO = 266,
     INT = 267,
     FLOAT = 268,
     ID = 269,
     DIGITO = 270,
     LETRA = 271,
     SEPARADOR = 272,
     PRINT = 273,
     SCAN = 274
   };
#endif
/* Tokens.  */
#define OP_ARITM 258
#define OP_COMP 259
#define OP_LOG 260
#define OP_ASSIGN 261
#define BOOL 262
#define TIPO 263
#define CONDICOES 264
#define LACOS 265
#define RETORNO 266
#define INT 267
#define FLOAT 268
#define ID 269
#define DIGITO 270
#define LETRA 271
#define SEPARADOR 272
#define PRINT 273
#define SCAN 274




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 69 "syntaxAnalyser.y"
{
  char *id;
  char *tipo;
  char *operador;

  char *str;
  struct node* nd;
}
/* Line 1529 of yacc.c.  */
#line 96 "syntaxAnalyser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
