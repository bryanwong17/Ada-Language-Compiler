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
     BEGIN_TOKEN = 258,
     BOOLEAN = 259,
     BREAK = 260,
     CHARACTER = 261,
     CASE = 262,
     CONTINUE = 263,
     CONSTANT = 264,
     DECLARE = 265,
     DO = 266,
     ELSE = 267,
     END = 268,
     EXIT = 269,
     FLOAT = 270,
     FOR = 271,
     IF = 272,
     IN = 273,
     INTEGER = 274,
     LOOP = 275,
     PRINT = 276,
     PRINTLN = 277,
     PROCEDURE = 278,
     PROGRAM = 279,
     RANGE = 280,
     READ = 281,
     RETURN = 282,
     STRING = 283,
     THEN = 284,
     WHILE = 285,
     BOOLConst = 286,
     FLOATConst = 287,
     ID = 288,
     INTConst = 289,
     STRConst = 290,
     ASSIGN = 291,
     OR = 292,
     AND = 293,
     NOT = 294,
     NE = 295,
     GE = 296,
     LE = 297,
     Unary_minus = 298
   };
#endif
/* Tokens.  */
#define BEGIN_TOKEN 258
#define BOOLEAN 259
#define BREAK 260
#define CHARACTER 261
#define CASE 262
#define CONTINUE 263
#define CONSTANT 264
#define DECLARE 265
#define DO 266
#define ELSE 267
#define END 268
#define EXIT 269
#define FLOAT 270
#define FOR 271
#define IF 272
#define IN 273
#define INTEGER 274
#define LOOP 275
#define PRINT 276
#define PRINTLN 277
#define PROCEDURE 278
#define PROGRAM 279
#define RANGE 280
#define READ 281
#define RETURN 282
#define STRING 283
#define THEN 284
#define WHILE 285
#define BOOLConst 286
#define FLOATConst 287
#define ID 288
#define INTConst 289
#define STRConst 290
#define ASSIGN 291
#define OR 292
#define AND 293
#define NOT 294
#define NE 295
#define GE 296
#define LE 297
#define Unary_minus 298




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 254 "ada_parser.y"
{
	int i_val;
	string *s_val;
	bool b_val;
	double d_val;
	IDInfo* info;
	int type;
}
/* Line 1529 of yacc.c.  */
#line 144 "y.tab.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

