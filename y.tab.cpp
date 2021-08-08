/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "ada_parser.y"


#include <iostream>
#include <fstream>
#include <stack>
#include "symboltable.hpp"
#include "lex.yy.cpp"

#define Trace(t) if (DebugPrint) printf("======TRACE====== %s\n", t)
#define YYDEBUG_LEXER_TEXT yytext

int DebugPrint = 1; //trace
int DebugDump = 0;
int Opt_D = 1; //symbol table

void yyerror(string s);
SymbolTableList syms;
vector<vector<IDInfo>> fps;
extern char *yytext;

string filename;
ofstream out;

struct Label {
	Label(int n) {
		count = n;
  		flag = -1;
	}
	int count;
	int flag;
};

class LabelAdministrator {
	public:
		LabelAdministrator() {
			count = 0;
		}
    	stack<Label> stacks;
    	void pushSeveralLabels(int n) {
			stacks.push(Label(count));
  			count += n;
		}
    	void severalLabels(int n) {
			stacks.top().count += n;
  			count += n;
		}
    	void removeLabel() {
			stacks.pop();
		}
    	int takeLabel(int n) {
			return stacks.top().count + n;
		}
    	int getLable() {
			return count++;
		}
    	void addFlag() {
			stacks.top().flag = stacks.top().flag + 1;
		}
    	int getFlag() {
			return stacks.top().flag;
		}
	private:
		int count;
};

LabelAdministrator la;

void constant(IDInfo inf)
{
	string temp = "\t\t";

	if(inf.type == int_type) {
		temp += "sipush " + to_string(inf.value.i);
	}
	else if(inf.type == string_type) {
		temp += "ldc \"" + inf.value.s + "\"";
	}
	else if(inf.type == bool_type) {
		if (inf.value.b) temp += "iconst_1";
		else temp += "iconst_0";
	}

	out << temp << endl;
}

void load_static_global_variable(IDInfo inf)
{
	if (inf.flag == const_var_flag) {
		constant(inf);
		return;
	}
	if (inf.scope == local_t) {
		out << "\t\tiload " << to_string(inf.idx) << endl;
		return;
	}

	string temp = "\t\tgetstatic ";

	if(inf.type == int_type) temp += "int ";
	else if(inf.type == real_type) temp += "float ";
	else if(inf.type == bool_type) temp += "bool ";
	else if(inf.type == string_type) temp += "string ";
	else if(inf.type == array_type) temp += "arr ";
	else if(inf.type == void_type) temp += "void ";
	else if(inf.type == void_type) temp += "unk ";
	
	temp += filename + "." + inf.id_name;
	out << temp << endl;
}

void global_declaration(IDInfo inf)
{
	string temp = "\tfield static ";

	if(inf.type == int_type) {
		temp += "int ";
		temp += inf.id_name;
		if (inf.initialized) temp += " = " + to_string(inf.value.i);
	}
	else if(inf.type == bool_type) {
		temp += "bool "; 
		temp += inf.id_name;
		if (inf.initialized) {
			temp += " = ";
			if (inf.value.b) temp += "true";
			else temp += "false";
		}
	}

	out << temp << endl;
}

void local_declaration(IDInfo inf)
{
	string temp = "\t\tsipush ";

	if(inf.type == int_type) {
		temp += to_string(inf.value.i);
	}
	else if(inf.type == bool_type) {
		if (inf.value.b) temp += "true";
		else temp += "false";
	}

	temp += "\t\tistore " + to_string(inf.idx) + "\n";

	out << temp;
}

struct LabelSet
{
	int start = 0;
	int count = 0;
	bool is_loop_label = false;
};

class LS
{
public:
	stack<LabelSet> ls;
	int lc;
	LS() {
		lc = 0;
	}
	void newLabelSet(int n, bool is_loop) {
		LabelSet l;
		l.start = lc;
		l.count = n;
		l.is_loop_label = is_loop;
		lc += n;
		this->ls.push(l);
	}
	int getLabel(int l) {
		return ls.top().start + l;
	}
	void popLabelSet() {
		this->ls.pop();
	}
};

LS ls;

void store(IDInfo inf)
{
	if (inf.scope == local_t) {
		out << "\t\tistore " << to_string(inf.idx) << endl;
		return;
	}

	string temp = "\t\tputstatic ";

	if(inf.type == int_type) temp += "int ";
	else if(inf.type == real_type) temp += "float ";
	else if(inf.type == bool_type) temp += "bool ";
	else if(inf.type == string_type) temp += "string ";
	else if(inf.type == array_type) temp += "arr ";
	else if(inf.type == void_type) temp += "void ";
	else if(inf.type == void_type) temp += "unk ";

	temp += filename + "." + inf.id_name;
	out << temp << endl;
}

void beginning_for(IDInfo inf, IDInfo toplim, IDInfo botlim)
{
	ls.newLabelSet(4, true);

	string l1 = "L" + to_string(ls.getLabel(0)); // Lbegin
	string l2 = "L" + to_string(ls.getLabel(1)); // Lexit
	string l3 = "L" + to_string(ls.getLabel(2)); // Ltrue
	string l4 = "L" + to_string(ls.getLabel(3)); // Lfalse

	constant(toplim);
	store(inf);

	out << l1 << ":\n"; //Lbegin
	load_static_global_variable(inf);
	constant(botlim);

	out << "\t\tisub\n";
	out << "\t\tifle " << l3 << endl; //Ltrue
	out << "\t\ticonst_0\n";
	out << "\t\tgoto " << l4 << endl; //Lfalse

	out << l3 << ":\n"; //Ltrue
	out << "\t\ticonst_1\n";

	out << l4 << ":\n"; //Lfalse
	out << "\t\tifeq " << l2 << endl; //Lexit

}

void end_for(IDInfo inf)
{
	string l1 = "L" + to_string(ls.getLabel(0)); // Lbegin
	string l2 = "L" + to_string(ls.getLabel(1)); // Lexit
	string l3 = "L" + to_string(ls.getLabel(2)); // Ltrue
	string l4 = "L" + to_string(ls.getLabel(3)); // Lfalse

	load_static_global_variable(inf);
	out << "\t\tsipush 1\n";
	out << "\t\tiadd\n";
	store(inf);
	out << "\t\tgoto " << l1 << endl; //Lbegin

	out << l2 << ":\n"; //Lexit

	ls.popLabelSet(); 
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 443 "y.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 456 "y.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   355

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  101
/* YYNRULES -- Number of states.  */
#define YYNSTATES  206

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   298

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      55,    56,    48,    46,    57,    47,     2,    49,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    51,
      40,    41,    42,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    43,    44,    45,    50
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,     6,    20,    23,    24,    27,
      29,    31,    33,    35,    42,    51,    54,    59,    64,    71,
      79,    81,    83,    85,    87,    89,    90,    93,    95,    96,
      97,   107,   109,   112,   116,   117,   121,   123,   127,   130,
     131,   133,   134,   137,   139,   141,   143,   145,   148,   150,
     152,   157,   165,   166,   171,   172,   177,   181,   184,   188,
     191,   193,   194,   200,   209,   210,   222,   223,   224,   225,
     235,   236,   251,   252,   258,   260,   261,   265,   267,   269,
     271,   273,   275,   277,   279,   281,   286,   288,   291,   295,
     299,   303,   307,   311,   315,   319,   323,   327,   331,   334,
     338,   342
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    -1,    -1,    -1,    24,    33,    60,    63,
      70,     3,    61,    80,    62,    13,    51,    13,    33,    -1,
      10,    64,    -1,    -1,    65,    64,    -1,    65,    -1,    66,
      -1,    67,    -1,    68,    -1,    33,    52,     9,    36,   102,
      51,    -1,    33,    52,     9,    52,    69,    36,   102,    51,
      -1,    33,    51,    -1,    33,    52,    69,    51,    -1,    33,
      36,   103,    51,    -1,    33,    52,    69,    36,   103,    51,
      -1,    33,    52,    69,    53,   102,    54,    51,    -1,    19,
      -1,    28,    -1,     4,    -1,    15,    -1,    71,    -1,    -1,
      72,    71,    -1,    72,    -1,    -1,    -1,    23,    33,    73,
      76,    74,    79,    87,    13,    75,    -1,    33,    -1,    33,
      51,    -1,    55,    77,    56,    -1,    -1,    78,    51,    77,
      -1,    78,    -1,    33,    52,    69,    -1,    27,    69,    -1,
      -1,    81,    -1,    -1,    82,    81,    -1,    82,    -1,    83,
      -1,    90,    -1,    93,    -1,    97,    51,    -1,    87,    -1,
      84,    -1,    33,    36,   103,    51,    -1,    33,    53,   103,
      54,    36,   103,    51,    -1,    -1,    85,    21,   103,    51,
      -1,    -1,    86,    22,   103,    51,    -1,    26,    33,    51,
      -1,    27,    51,    -1,    27,   103,    51,    -1,    88,    51,
      -1,    88,    -1,    -1,    63,     3,    89,    81,    13,    -1,
      17,   103,    92,    29,    83,    13,    17,    51,    -1,    -1,
      17,   103,    92,    29,    83,    12,    91,    83,    13,    17,
      51,    -1,    -1,    -1,    -1,    30,    94,   103,    95,    20,
      83,    13,    20,    51,    -1,    -1,    16,    55,    33,    18,
     102,    25,   102,    56,    20,    96,    83,    13,    20,    51,
      -1,    -1,    33,    98,    55,    99,    56,    -1,   100,    -1,
      -1,   101,    57,   100,    -1,   101,    -1,   103,    -1,    34,
      -1,    35,    -1,    31,    -1,    32,    -1,    33,    -1,   102,
      -1,    33,    53,   103,    54,    -1,    97,    -1,    47,   103,
      -1,   103,    48,   103,    -1,   103,    49,   103,    -1,   103,
      46,   103,    -1,   103,    47,   103,    -1,   103,    40,   103,
      -1,   103,    45,   103,    -1,   103,    41,   103,    -1,   103,
      44,   103,    -1,   103,    42,   103,    -1,   103,    43,   103,
      -1,    39,   103,    -1,   103,    38,   103,    -1,   103,    37,
     103,    -1,    55,   103,    56,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   286,   286,   290,   295,   286,   306,   306,   310,   310,
     314,   314,   314,   317,   330,   346,   365,   383,   401,   422,
     432,   433,   434,   435,   439,   439,   443,   443,   447,   455,
     447,   477,   477,   480,   480,   484,   484,   488,   499,   502,
     508,   508,   512,   512,   516,   516,   516,   516,   520,   520,
     524,   541,   551,   551,   560,   560,   569,   576,   581,   588,
     588,   592,   592,   602,   609,   609,   621,   628,   632,   628,
     643,   643,   660,   660,   692,   692,   696,   696,   700,   706,
     710,   714,   717,   723,   731,   735,   745,   749,   761,   774,
     787,   801,   814,   838,   862,   886,   910,   934,   958,   970,
     983,   996
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BEGIN_TOKEN", "BOOLEAN", "BREAK",
  "CHARACTER", "CASE", "CONTINUE", "CONSTANT", "DECLARE", "DO", "ELSE",
  "END", "EXIT", "FLOAT", "FOR", "IF", "IN", "INTEGER", "LOOP", "PRINT",
  "PRINTLN", "PROCEDURE", "PROGRAM", "RANGE", "READ", "RETURN", "STRING",
  "THEN", "WHILE", "BOOLConst", "FLOATConst", "ID", "INTConst", "STRConst",
  "ASSIGN", "OR", "AND", "NOT", "'<'", "'='", "'>'", "NE", "GE", "LE",
  "'+'", "'-'", "'*'", "'/'", "Unary_minus", "';'", "':'", "'['", "']'",
  "'('", "')'", "','", "$accept", "program", "@1", "@2", "@3",
  "zero_one_vc_dec", "one_more_vc", "var_const_dec", "const_dec",
  "var_dec", "arr_dec", "var_type", "zero_more_fp_dec", "one_more_fp_dec",
  "fp_dec", "@4", "@5", "endFunc", "zero_more_formal_arg",
  "one_more_formal_arg", "arg", "opt_return_type", "zero_more_stat",
  "one_more_stat", "stmt", "bos_stmt", "simple_stmt", "@6", "@7",
  "block_stat", "block_stat_clean", "@8", "conditional_stat", "@9",
  "ifStart", "loop_stat", "@10", "@11", "@12", "func_invoc", "@13",
  "opt_comma_sep_exprs", "comma_sep_exprs", "func_expr", "const_val",
  "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      60,    61,    62,   295,   296,   297,    43,    45,    42,    47,
     298,    59,    58,    91,    93,    40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    60,    61,    62,    59,    63,    63,    64,    64,
      65,    65,    65,    66,    66,    67,    67,    67,    67,    68,
      69,    69,    69,    69,    70,    70,    71,    71,    73,    74,
      72,    75,    75,    76,    76,    77,    77,    78,    79,    79,
      80,    80,    81,    81,    82,    82,    82,    82,    83,    83,
      84,    84,    85,    84,    86,    84,    84,    84,    84,    87,
      87,    89,    88,    90,    91,    90,    92,    94,    95,    93,
      96,    93,    98,    97,    99,    99,   100,   100,   101,   102,
     102,   102,   102,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     0,    13,     2,     0,     2,     1,
       1,     1,     1,     6,     8,     2,     4,     4,     6,     7,
       1,     1,     1,     1,     1,     0,     2,     1,     0,     0,
       9,     1,     2,     3,     0,     3,     1,     3,     2,     0,
       1,     0,     2,     1,     1,     1,     1,     2,     1,     1,
       4,     7,     0,     4,     0,     4,     3,     2,     3,     2,
       1,     0,     5,     8,     0,    11,     0,     0,     0,     9,
       0,    14,     0,     5,     1,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     7,     0,    25,     0,     6,
       9,    10,    11,    12,     0,     0,    24,    27,     0,    15,
       0,     8,    28,     3,    26,    81,    82,    83,    79,    80,
       0,     0,     0,    86,    84,     0,    22,     0,    23,    20,
      21,     0,    34,     7,     0,     0,    98,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,    16,     0,     0,    29,     0,
       0,     0,     0,    67,    72,     0,     4,    40,     7,    44,
      49,     0,     0,    48,    60,    45,    46,     0,     0,    75,
     101,   100,    99,    92,    94,    96,    97,    95,    93,    90,
      91,    88,    89,     0,     0,     0,     0,     0,     0,    36,
      39,     0,    66,     0,    57,     0,     0,     0,     0,    61,
       0,    42,     0,     0,    59,    47,    85,     0,    74,    77,
      78,    13,     0,    18,     0,     0,    33,     0,     0,     7,
       0,     0,    56,    58,    68,     0,     0,     7,     0,     0,
       0,    73,     0,     0,    19,    37,    35,    38,     0,     0,
       7,     0,    50,     0,     0,     0,    53,    55,    76,    14,
       0,     0,     0,     0,     7,     0,    62,     0,    31,    30,
       0,    64,     0,     0,     0,     5,    32,     0,     7,     0,
       0,    51,     0,     0,    63,     0,    70,     0,    69,     7,
       0,     0,    65,     0,     0,    71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    43,   120,    75,     9,    10,    11,    12,
      13,    41,    15,    16,    17,    42,   110,   179,    68,   108,
     109,   139,    76,    77,    78,    79,    80,    81,    82,    83,
      84,   147,    85,   188,   141,    86,   116,   161,   199,    33,
      45,   127,   128,   129,    34,   130
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -132
static const yytype_int16 yypact[] =
{
     -14,    15,    19,  -132,  -132,    40,    18,    30,    13,  -132,
      18,  -132,  -132,  -132,    25,    63,  -132,    30,    55,  -132,
       2,  -132,  -132,  -132,  -132,  -132,  -132,    -8,  -132,  -132,
      55,    55,    55,  -132,  -132,   172,  -132,   -29,  -132,  -132,
    -132,   -33,    17,    98,    55,    22,   299,  -132,   125,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,  -132,    -7,    65,    55,  -132,    -7,    37,  -132,    23,
      55,    48,    28,  -132,   -31,    79,  -132,  -132,   113,  -132,
    -132,    64,    69,  -132,    41,  -132,  -132,    44,   142,    55,
    -132,   289,   299,   129,   129,   129,   129,   129,   129,     7,
       7,  -132,  -132,    45,    61,   187,    49,    54,    51,    58,
      85,    83,   277,    66,  -132,   202,    55,    55,    55,  -132,
     105,  -132,    55,    55,  -132,  -132,  -132,    71,  -132,    75,
     277,  -132,    -7,  -132,    70,    65,  -132,    37,    65,    40,
     115,   108,  -132,  -132,   277,   217,   157,   128,    90,   232,
     247,  -132,    55,    91,  -132,  -132,  -132,  -132,   138,    -7,
     126,   136,  -132,   124,   151,   179,  -132,  -132,  -132,  -132,
     160,   182,   -31,     3,   126,    55,  -132,   175,   171,  -132,
      -7,  -132,   209,   224,   262,  -132,  -132,   185,   126,   201,
     236,  -132,   251,   254,  -132,   231,  -132,   269,  -132,   126,
     246,   288,  -132,   292,   265,  -132
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -132,  -132,  -132,  -132,   323,   339,  -132,  -132,  -132,
    -132,   -62,  -132,   333,  -132,  -132,  -132,  -132,  -132,   214,
    -132,  -132,  -132,   -76,  -132,  -131,  -132,  -132,  -132,   213,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,   -34,
    -132,  -132,   203,  -132,   -58,   -18
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -73
static const yytype_int16 yytable[] =
{
      35,   104,   121,    64,   103,   117,    36,    62,   106,    87,
       1,    37,    46,    47,    48,   181,   182,    38,    65,     4,
      66,    39,   118,    63,    25,    26,    88,    28,    29,   173,
      40,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   183,    87,    44,   105,   -72,     3,    18,
       6,     8,   112,    14,   115,    59,    60,   193,    22,    25,
      26,    27,    28,    29,    19,    20,    23,    30,   201,    36,
     107,   164,    67,   155,   153,    31,   157,    89,   111,   114,
      38,   113,   119,    32,    39,   122,    25,    26,    27,    28,
      29,   123,   124,    40,    30,   125,   131,   132,   144,   145,
     146,   171,    31,   134,   149,   150,   135,   136,     6,   137,
      32,   -41,   138,    87,    69,    70,   140,   142,   148,   -52,
     -54,   154,   187,     6,    71,    72,   -43,   151,    73,    69,
      70,    74,   152,   159,   -52,   -54,     6,   160,     6,    71,
      72,   165,   169,    73,    69,    70,    74,   -52,   -54,   -52,
     -54,   170,    71,    72,    71,    72,   174,   184,    73,   172,
     175,    74,    49,    50,   176,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    57,    58,    59,    60,    49,
      50,    90,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   177,   178,    49,    50,   126,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   180,   185,    49,
      50,   163,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   186,    61,    49,    50,   189,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   190,   133,    49,
      50,   192,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   194,   143,    49,    50,   195,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   197,   162,    49,
      50,   196,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   198,   166,    49,    50,   200,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   202,   167,    49,
      50,   203,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   204,   191,    49,    50,   205,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    50,     7,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    21,
      24,   156,   158,     0,     0,   168
};

static const yytype_int16 yycheck[] =
{
      18,    63,    78,    36,    62,    36,     4,    36,    66,    43,
      24,     9,    30,    31,    32,    12,    13,    15,    51,     0,
      53,    19,    53,    52,    31,    32,    44,    34,    35,   160,
      28,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,   174,    78,    53,    64,    55,    33,    36,
      10,    33,    70,    23,    72,    48,    49,   188,    33,    31,
      32,    33,    34,    35,    51,    52,     3,    39,   199,     4,
      33,   147,    55,   135,   132,    47,   138,    55,    55,    51,
      15,    33,     3,    55,    19,    21,    31,    32,    33,    34,
      35,    22,    51,    28,    39,    51,    51,    36,   116,   117,
     118,   159,    47,    54,   122,   123,    52,    56,    10,    51,
      55,    13,    27,   147,    16,    17,    33,    51,    13,    21,
      22,    51,   180,    10,    26,    27,    13,    56,    30,    16,
      17,    33,    57,    18,    21,    22,    10,    29,    10,    26,
      27,    51,    51,    30,    16,    17,    33,    21,    22,    21,
      22,    13,    26,    27,    26,    27,    20,   175,    30,    33,
      36,    33,    37,    38,    13,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    46,    47,    48,    49,    37,
      38,    56,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    13,    33,    37,    38,    54,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    25,    33,    37,
      38,    54,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    51,    51,    37,    38,    17,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    13,    51,    37,
      38,    56,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    51,    51,    37,    38,    20,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    13,    51,    37,
      38,    20,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    51,    51,    37,    38,    17,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    51,    51,    37,
      38,    13,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    20,    51,    37,    38,    51,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    38,     5,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    10,
      17,   137,   139,    -1,    -1,   152
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    59,    33,     0,    60,    10,    63,    33,    64,
      65,    66,    67,    68,    23,    70,    71,    72,    36,    51,
      52,    64,    33,     3,    71,    31,    32,    33,    34,    35,
      39,    47,    55,    97,   102,   103,     4,     9,    15,    19,
      28,    69,    73,    61,    53,    98,   103,   103,   103,    37,
      38,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    51,    36,    52,    36,    51,    53,    55,    76,    16,
      17,    26,    27,    30,    33,    63,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    90,    93,    97,   103,    55,
      56,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   102,    69,   103,   102,    33,    77,    78,
      74,    55,   103,    33,    51,   103,    94,    36,    53,     3,
      62,    81,    21,    22,    51,    51,    54,    99,   100,   101,
     103,    51,    36,    51,    54,    52,    56,    51,    27,    79,
      33,    92,    51,    51,   103,   103,   103,    89,    13,   103,
     103,    56,    57,   102,    51,    69,    77,    69,    87,    18,
      29,    95,    51,    54,    81,    51,    51,    51,   100,    51,
      13,   102,    33,    83,    20,    36,    13,    13,    33,    75,
      25,    12,    13,    83,   103,    33,    51,   102,    91,    17,
      13,    51,    56,    83,    51,    20,    20,    13,    51,    96,
      17,    83,    51,    13,    20,    51
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 286 "ada_parser.y"
    {
						filename = *(yyvsp[(2) - (2)].s_val);
						out << "class " << filename << endl << "{" << endl;
					}
    break;

  case 3:
#line 290 "ada_parser.y"
    {
						out << "\tmethod public static void main(java.lang.String[])" << endl;
  						out << "\tmax_stack 15" << endl;
  						out << "\tmax_locals 15" << endl << "\t{" << endl;;
					}
    break;

  case 4:
#line 295 "ada_parser.y"
    {
						out << "\t\treturn" << endl << "\t}" << endl;;
					}
    break;

  case 5:
#line 297 "ada_parser.y"
    {
						Trace("Program Unit");
						if(Opt_D) syms.dump();
						syms.pop();
						out << "}" << endl;
					}
    break;

  case 13:
#line 317 "ada_parser.y"
    {
						Trace("Constant Declaration");
						if (!isConstant(*(yyvsp[(5) - (6)].info))) yyerror("Expression is not a constant\n");
						(yyvsp[(5) - (6)].info)->flag = const_var_flag;
						(yyvsp[(5) - (6)].info)->initialized = true;

						if (syms.get_top_idx() != 0) {							
							(yyvsp[(5) - (6)].info)->scope = local_t;
						}

						if (syms.insert(*(yyvsp[(1) - (6)].s_val), *(yyvsp[(5) - (6)].info)) == -1) yyerror("(Constant) variable redefinition");
					}
    break;

  case 14:
#line 330 "ada_parser.y"
    {
						Trace("Constant Declaration with Type");
						if (!isConstant(*(yyvsp[(7) - (8)].info))) yyerror("Expression is not a constant\n");
						if ((yyvsp[(5) - (8)].type) != (yyvsp[(7) - (8)].info)->type) yyerror("Types do not match\n");
						(yyvsp[(7) - (8)].info)->flag = const_var_flag;
						(yyvsp[(7) - (8)].info)->initialized = true;

						if (syms.get_top_idx() != 0) {
							(yyvsp[(7) - (8)].info)->scope = global_t;
						}

						if (syms.insert(*(yyvsp[(1) - (8)].s_val), *(yyvsp[(7) - (8)].info)) == -1) yyerror("(Constant) variable redefinition");
					}
    break;

  case 15:
#line 346 "ada_parser.y"
    {
						Trace("Variable Declaration");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = int_type;
						info->initialized = false;
						info->id_name = *(yyvsp[(1) - (2)].s_val);

						if (syms.get_top_idx() == 0) {
							info->scope = global_t;
							global_declaration(*info);
						}
						else {
							info->scope = local_t;
						}	

						if (syms.insert(*(yyvsp[(1) - (2)].s_val), *info) == -1) yyerror("Variable redefinition\n");
					}
    break;

  case 16:
#line 365 "ada_parser.y"
    {
						Trace("Variable Declaration with Type");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(3) - (4)].type);
						info->initialized = false;
						info->id_name = *(yyvsp[(1) - (4)].s_val);

						if (syms.get_top_idx() == 0) {
							info->scope = global_t;
							global_declaration(*info);
						} else {
							info->scope = local_t;
						}	

						if (syms.insert(*(yyvsp[(1) - (4)].s_val), *info) == -1) yyerror("Variable redefinition\n");
					}
    break;

  case 17:
#line 383 "ada_parser.y"
    {
						Trace("Variable Declaration with Expression");
						if (!isConstant(*(yyvsp[(3) - (4)].info))) yyerror("Expression is not a constant value\n");
						(yyvsp[(3) - (4)].info)->flag = var_flag;
						(yyvsp[(3) - (4)].info)->initialized = true;
						(yyvsp[(3) - (4)].info)->id_name = *(yyvsp[(1) - (4)].s_val);

						if (syms.get_top_idx() == 0) {
							(yyvsp[(3) - (4)].info)->scope = global_t;
							global_declaration(*(yyvsp[(3) - (4)].info));
						} else
						{
							(yyvsp[(3) - (4)].info)->scope = local_t;
						}

						if (syms.insert(*(yyvsp[(1) - (4)].s_val), *(yyvsp[(3) - (4)].info)) == -1) yyerror("Variable redefinition\n");
					}
    break;

  case 18:
#line 401 "ada_parser.y"
    {
						Trace("Variable Declaration with Type and Expression");
						if (!isConstant(*(yyvsp[(5) - (6)].info))) yyerror("Expression is not a constant value\n");
						if ((yyvsp[(3) - (6)].type) != (yyvsp[(5) - (6)].info)->type) yyerror("Types do not match\n");
						(yyvsp[(5) - (6)].info)->flag = var_flag;
						(yyvsp[(5) - (6)].info)->initialized = true;
						(yyvsp[(5) - (6)].info)->id_name = *(yyvsp[(1) - (6)].s_val);

						if (syms.get_top_idx() == 0) {
							(yyvsp[(5) - (6)].info)->scope = global_t;
							global_declaration(*(yyvsp[(5) - (6)].info));
						} else {
							(yyvsp[(5) - (6)].info)->scope = local_t;
							local_declaration(*(yyvsp[(5) - (6)].info));
						}

						if (syms.insert(*(yyvsp[(1) - (6)].s_val), *(yyvsp[(5) - (6)].info)) == -1) yyerror("Variable redefinition\n");
					}
    break;

  case 19:
#line 422 "ada_parser.y"
    {
					Trace("Array Declaration");
					if (!isConstant(*(yyvsp[(5) - (7)].info))) yyerror("Array size not constant");
					if ((yyvsp[(5) - (7)].info)->type != int_type) yyerror("Array size invalid");
					if ((yyvsp[(5) - (7)].info)->value.i < 1) yyerror("Array size < 1");
					if (syms.insert(*(yyvsp[(1) - (7)].s_val), (yyvsp[(3) - (7)].type), (yyvsp[(5) - (7)].info)->value.i) == -1) yyerror("Variable redefinition\n");
				}
    break;

  case 20:
#line 432 "ada_parser.y"
    { (yyval.type) = int_type; }
    break;

  case 21:
#line 433 "ada_parser.y"
    { (yyval.type) = string_type; }
    break;

  case 22:
#line 434 "ada_parser.y"
    { (yyval.type) = bool_type; }
    break;

  case 23:
#line 435 "ada_parser.y"
    { (yyval.type) = real_type; }
    break;

  case 28:
#line 447 "ada_parser.y"
    {
						Trace("Function Declaration");
						IDInfo* info = new IDInfo();
						info->flag = function_flag;
						info->initialized = false;
						if (syms.insert(*(yyvsp[(2) - (2)].s_val), *info) == -1) yyerror("Procedure redefinition");
						syms.push();
					}
    break;

  case 29:
#line 455 "ada_parser.y"
    {
						IDInfo info = *syms.lookup(*(yyvsp[(2) - (4)].s_val));
						out << "\tmethod public static ";
						out << ((info.type == void_type)? "void" : "int");
						out << " " + info.id_name + "(";
						for (int i = 0; i < info.value.args.size(); i++) {
							if (i != 0) out << ", ";
							out << "int";
						}
						out << ")" << endl;
						out << "\tmax_stack 15" << endl;
						out << "\tmax_locals 15" << endl << "\t{" << endl;
					}
    break;

  case 30:
#line 468 "ada_parser.y"
    {
						if(syms.lookup(*(yyvsp[(2) - (9)].s_val))->type == void_type) out << "\t\treturn" << endl << "\t}" << endl;
						else out << "\t}" << endl;;
						if(Opt_D) syms.dump();
						syms.pop();
					}
    break;

  case 37:
#line 488 "ada_parser.y"
    {
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(3) - (3)].type);
						info->initialized = false;
						if (syms.insert(*(yyvsp[(1) - (3)].s_val), *info) == -1) yyerror("Variable redefinition");
						syms.addFunctionArgument(*(yyvsp[(1) - (3)].s_val), *info);
					}
    break;

  case 38:
#line 499 "ada_parser.y"
    {
						syms.setFunctionType((yyvsp[(2) - (2)].type));
					}
    break;

  case 39:
#line 502 "ada_parser.y"
    {
						syms.setFunctionType(void_type);
					}
    break;

  case 50:
#line 524 "ada_parser.y"
    {
						Trace("Variable Assignment");
						IDInfo* info = syms.lookup(*(yyvsp[(1) - (4)].s_val));
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag == const_var_flag) yyerror("Constant variables cannot be reassigned");
						if (info->flag == function_flag || info->flag == procedure_flag) yyerror("Procedures cannot be assigned\n");
						if (info->type != (yyvsp[(3) - (4)].info)->type) yyerror("Types do not match");

						if(info->type == int_type || info->type == bool_type) {
							int idx = syms.getIndex(*(yyvsp[(1) - (4)].s_val));
							if(idx == -1) {
								out << "\t\tputstatic int " << filename << "." << *(yyvsp[(1) - (4)].s_val) << endl;
							}
							else out << "\t\tistore " << idx << endl;
						}
					}
    break;

  case 51:
#line 541 "ada_parser.y"
    {
						Trace("Array Assignment");
						IDInfo* info = syms.lookup(*(yyvsp[(1) - (7)].s_val));
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != var_flag) yyerror("Not a variable");
						if (info->type != array_type) yyerror("Not an array");
						if ((yyvsp[(3) - (7)].info)->type != int_type) yyerror("Invalid index");
						if ((yyvsp[(3) - (7)].info)->value.i < 0 || (yyvsp[(3) - (7)].info)->value.i >= info->value.args.size()) yyerror("Index out of range");
						if (info->value.args[0].type != (yyvsp[(6) - (7)].info)->type) yyerror("Types do not match");
					}
    break;

  case 52:
#line 551 "ada_parser.y"
    {
						out << "\t\tgetstatic java.io.PrintStream java.lang.System.out" << endl;
					}
    break;

  case 53:
#line 555 "ada_parser.y"
    {
						Trace("Print Expression");
						if((yyvsp[(3) - (4)].info)->type == string_type) out << "\t\tinvokevirtual void java.io.PrintStream.print(java.lang.String)" << endl;
						else out << "\t\tinvokevirtual void java.io.PrintStream.print(int)" << endl;;
					}
    break;

  case 54:
#line 560 "ada_parser.y"
    {
						out << "\t\tgetstatic java.io.PrintStream java.lang.System.out" << endl;
					// simple -> println expression
					}
    break;

  case 55:
#line 563 "ada_parser.y"
    {
						Trace("Println Expression");
						if((yyvsp[(3) - (4)].info)->type == string_type) out << "\t\tinvokevirtual void java.io.PrintStream.println(java.lang.String)" << endl;
						else out << "\t\tinvokevirtual void java.io.PrintStream.println(int)" << endl;
					}
    break;

  case 56:
#line 569 "ada_parser.y"
    {
						Trace("Read Identifier");
						IDInfo* info = syms.lookup(*(yyvsp[(2) - (3)].s_val));
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != const_var_flag || info->flag != var_flag) yyerror("Not a variable\n");
						if (info->type != string_type) yyerror("Incompatible type");
					// return
					}
    break;

  case 57:
#line 577 "ada_parser.y"
    {
						Trace("Return");
						out << "\t\treturn" << endl;
					// return expression
					}
    break;

  case 58:
#line 581 "ada_parser.y"
    {
						Trace("Return Expression");
						out << "\t\tireturn" << endl;
					}
    break;

  case 61:
#line 592 "ada_parser.y"
    {
						syms.push();
					}
    break;

  case 62:
#line 595 "ada_parser.y"
    {
						if(Opt_D) syms.dump();
						syms.pop();
					}
    break;

  case 63:
#line 602 "ada_parser.y"
    {
						Trace("Conditional statement: if");
						if ((yyvsp[(2) - (8)].info)->type != bool_type) yyerror("Condition type does not match");
						out << "L" << la.takeLabel(0) << ":" << endl; //Lexit
  						la.removeLabel();
					}
    break;

  case 64:
#line 609 "ada_parser.y"
    {
						out << "\t\tgoto L" << la.takeLabel(1) << endl; //Lexit
  						out << "L" << la.takeLabel(0) << ":" << endl; //Lfalse
					}
    break;

  case 65:
#line 613 "ada_parser.y"
    {
						Trace("Conditional statement: if else");
						if ((yyvsp[(2) - (11)].info)->type != bool_type) yyerror("Condition type does not match");
						out << "L" << la.takeLabel(1) << ":" << endl; //Lexit
  						la.removeLabel();
					}
    break;

  case 66:
#line 621 "ada_parser.y"
    {
			la.pushSeveralLabels(2);
  			out << "\t\tifeq L" << la.takeLabel(0) << endl;
		}
    break;

  case 67:
#line 628 "ada_parser.y"
    {
					la.pushSeveralLabels(1);
  					out << "L" << la.takeLabel(0) << ":" << endl; //Lbegin
				}
    break;

  case 68:
#line 632 "ada_parser.y"
    {
						la.severalLabels(1);
  						out << "\t\tifeq L" << la.takeLabel(3 + la.getFlag()) << endl; //Ltrue
					}
    break;

  case 69:
#line 636 "ada_parser.y"
    {
						Trace("Loop statement: while");
						if ((yyvsp[(3) - (9)].info)->type != bool_type) yyerror("Condition type does not match");
						out << "\t\tgoto L" << la.takeLabel(la.getFlag()) << endl; //Lfalse
  						out << "L" << la.takeLabel(3 + la.getFlag()) << ":" << endl; //Ltrue
  						la.removeLabel();
					}
    break;

  case 70:
#line 643 "ada_parser.y"
    {
						Trace("loop stmt");

						IDInfo* info = syms.lookup(*(yyvsp[(3) - (9)].s_val));
						if (info == NULL) yyerror("Undeclared identifier");

						if ((yyvsp[(5) - (9)].info)->type != int_type || (yyvsp[(7) - (9)].info)->type != int_type) yyerror("Incompatible range");

						beginning_for(*info, *(yyvsp[(5) - (9)].info), *(yyvsp[(7) - (9)].info));

					}
    break;

  case 71:
#line 653 "ada_parser.y"
    {
						IDInfo* info = syms.lookup(*(yyvsp[(3) - (14)].s_val));
						end_for(*info);
					}
    break;

  case 72:
#line 660 "ada_parser.y"
    {
				fps.push_back(vector<IDInfo>());
				}
    break;

  case 73:
#line 663 "ada_parser.y"
    {
						Trace("Function or Procedure Invocation");
						IDInfo* info = syms.lookup(*(yyvsp[(1) - (5)].s_val));
						if (info == NULL) yyerror("Undeclared identifier");
						if (info->flag != function_flag && info->flag != procedure_flag) yyerror("Not a function nor a procedure, flag " + to_string(info->flag));

						vector<IDInfo> parameters = info->value.args;
						if (parameters.size() != fps[fps.size() - 1].size()) yyerror("Number of parameters do not match");

						for (int i = 0; i < parameters.size(); i++)
						{
							if (parameters[i].type != fps[fps.size() - 1].at(i).type) yyerror("Parameter types do not match");
						}

						out << "\t\tinvokestatic ";
  						out << ((info->type == void_type)? "void" : "int");
  						out << " " + filename + "." + info->id_name + "(";
  						for (int i = 0; i < info->value.args.size(); ++i) {
    						if (i != 0) out << ", ";
    							out << "int";
  						}
  						out << ")" << endl;

						(yyval.info) = info;
						fps.pop_back();
					}
    break;

  case 78:
#line 700 "ada_parser.y"
    {
					fps[fps.size() - 1].push_back(*(yyvsp[(1) - (1)].info));
				}
    break;

  case 79:
#line 706 "ada_parser.y"
    {
						Trace("Constant Value");
						(yyval.info) = intConstant((yyvsp[(1) - (1)].i_val));
					}
    break;

  case 80:
#line 710 "ada_parser.y"
    {
						Trace("String Constant Value");
						(yyval.info) = strConstant((yyvsp[(1) - (1)].s_val));
					}
    break;

  case 81:
#line 714 "ada_parser.y"
    {
						(yyval.info) = boolConstant((yyvsp[(1) - (1)].b_val));
					}
    break;

  case 82:
#line 717 "ada_parser.y"
    {
						(yyval.info) = floatConstant((yyvsp[(1) - (1)].d_val));
					}
    break;

  case 83:
#line 723 "ada_parser.y"
    {
						IDInfo* info = syms.lookup(*(yyvsp[(1) - (1)].s_val));
						if (info == NULL) yyerror("Undeclared identifier\n");
						load_static_global_variable(*info);
						(yyval.info) = info;

					}
    break;

  case 84:
#line 731 "ada_parser.y"
    {
						constant(*(yyvsp[(1) - (1)].info));
					}
    break;

  case 85:
#line 735 "ada_parser.y"
    {
						IDInfo* info = syms.lookup(*(yyvsp[(1) - (4)].s_val));
						if (info == NULL) yyerror("Undeclared identifier\n");
						if (info->type != array_type) yyerror("Not an array\n");
						if ((yyvsp[(3) - (4)].info)->type != int_type) yyerror("Array subscript not an integer\n");
						if ((yyvsp[(3) - (4)].info)->value.i < 0) yyerror("Array subscript invalid\n");
						if ((yyvsp[(3) - (4)].info)->value.i >= info->value.args.size()) yyerror("Array subscript out of range\n");
						(yyval.info) = new IDInfo(info->value.args[(yyvsp[(3) - (4)].info)->value.i]);
					}
    break;

  case 86:
#line 745 "ada_parser.y"
    {
						Trace("Function Invocation as Expression");
					}
    break;

  case 87:
#line 749 "ada_parser.y"
    {
						Trace("Unary minus Expression");
						if (!((yyvsp[(2) - (2)].info)->type == int_type || (yyvsp[(2) - (2)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(2) - (2)].info)->type;
						(yyval.info) = info;

						if ((yyvsp[(2) - (2)].info)->type == int_type) out << "\t\tineg" << endl;
					}
    break;

  case 88:
#line 761 "ada_parser.y"
    {
						Trace("Expression * Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match\n");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(1) - (3)].info)->type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) out << "\t\timul" << endl;
					}
    break;

  case 89:
#line 774 "ada_parser.y"
    {
						Trace("Expression / Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match\n");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(1) - (3)].info)->type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) out << "\t\tidiv" << endl;
					}
    break;

  case 90:
#line 788 "ada_parser.y"
    {
						Trace("Expression + Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match\n");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(1) - (3)].info)->type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) out << "\t\tiadd" << endl;
					}
    break;

  case 91:
#line 801 "ada_parser.y"
    {
						Trace("Expression - Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match\n");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = (yyvsp[(1) - (3)].info)->type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) out << "\t\tisub" << endl;
					}
    break;

  case 92:
#line 814 "ada_parser.y"
    {
						Trace("Expression < Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tiflt";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl; 
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 93:
#line 838 "ada_parser.y"
    {
						Trace("Expression <= Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifle";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 94:
#line 862 "ada_parser.y"
    {
						Trace("Expression = Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifeq";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 95:
#line 886 "ada_parser.y"
    {
						Trace("Expression => Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifge";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 96:
#line 910 "ada_parser.y"
    {
						Trace("Expression > Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifgt";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 97:
#line 934 "ada_parser.y"
    {
						Trace("Expression /= Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if (!((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == real_type)) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == int_type || (yyvsp[(1) - (3)].info)->type == bool_type) {
							out << "\t\tisub" << endl;
  							int lb1 = la.getLable();
  							int lb2 = la.getLable();
							out << "\t\tifne";
							out << " L" << lb1 << endl; //Ltrue
  							out << "\t\ticonst_0" << endl;
  							out << "\t\tgoto L" << lb2 << endl; //Lfalse
							out << "L" << lb1 << ":" << endl; //Ltrue
							out << "\t\ticonst_1" << endl;
							out << "L" << lb2 << ":" << endl; //Lfalse
						}
					}
    break;

  case 98:
#line 958 "ada_parser.y"
    {
						Trace("NOT Expression");
						if ((yyvsp[(2) - (2)].info)->type != bool_type) yyerror("operator error");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(2) - (2)].info)->type == bool_type) out << "\t\tldc 1" << endl << "\t\tixor" << endl;
					}
    break;

  case 99:
#line 970 "ada_parser.y"
    {
						Trace("expr AND expr");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if ((yyvsp[(1) - (3)].info)->type != bool_type) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == bool_type) out << "\t\tiand" << endl;
					}
    break;

  case 100:
#line 983 "ada_parser.y"
    {
						Trace("Expression OR Expression");
						if ((yyvsp[(1) - (3)].info)->type != (yyvsp[(3) - (3)].info)->type) yyerror("Operand types do not match");
						if ((yyvsp[(1) - (3)].info)->type != bool_type) yyerror("Operator error\n");
						IDInfo* info = new IDInfo();
						info->flag = var_flag;
						info->type = bool_type;
						(yyval.info) = info;

						if ((yyvsp[(1) - (3)].info)->type == bool_type) out << "\t\tior" << endl;
					}
    break;

  case 101:
#line 996 "ada_parser.y"
    {
						Trace("(expr)");
						(yyval.info) = (yyvsp[(2) - (3)].info);
					}
    break;


/* Line 1267 of yacc.c.  */
#line 2711 "y.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1002 "ada_parser.y"


void yyerror(string s) {
  cerr << "line error" << line << ": " << s << endl;
  exit(1);
}

int main(int argc, char **argv) {
	
  yyin = fopen(argv[1], "r");
  string source = string(argv[1]);
  int dotLocation = source.find(".");
  filename = source.substr(0, dotLocation);
  out.open(filename + ".jasm");

  yyparse();
  return 0;
}
