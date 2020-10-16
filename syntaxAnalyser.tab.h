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
     BOOL = 258,
     TIPO = 259,
     IF = 260,
     ELSE = 261,
     LACOS = 262,
     RETORNO = 263,
     INT = 264,
     FLOAT = 265,
     TUPLE = 266,
     ID = 267,
     DIGITO = 268,
     LETRA = 269,
     SEPARADOR = 270,
     PRINT = 271,
     SCAN = 272,
     OP_ASSIGN = 273,
     OP_ARITM = 274,
     OP_LOG = 275,
     OP_COMP = 276
   };
#endif
/* Tokens.  */
#define BOOL 258
#define TIPO 259
#define IF 260
#define ELSE 261
#define LACOS 262
#define RETORNO 263
#define INT 264
#define FLOAT 265
#define TUPLE 266
#define ID 267
#define DIGITO 268
#define LETRA 269
#define SEPARADOR 270
#define PRINT 271
#define SCAN 272
#define OP_ASSIGN 273
#define OP_ARITM 274
#define OP_LOG 275
#define OP_COMP 276




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 190 "syntaxAnalyser.y"
{
  char *id;
  char *tipo;
  char *operador;

  char *str;
  int itype;
  struct node* nd;
}
/* Line 1529 of yacc.c.  */
#line 101 "syntaxAnalyser.tab.h"
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
