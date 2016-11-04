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
     REAL = 258,
     INT = 259,
     NAME = 260,
     STRING = 261,
     RBNULL = 262,
     RBTAB = 263,
     FALSE = 264,
     TRUE = 265,
     FUNCTION = 266,
     PROCEDURE = 267,
     CLASS = 268,
     FOR = 269,
     IN = 270,
     IF = 271,
     ELSE = 272,
     WHILE = 273,
     NEXT = 274,
     BREAK = 275,
     RETURN = 276,
     MOD_CONST = 277,
     MOD_DYNAMIC = 278,
     MOD_STOCHASTIC = 279,
     MOD_DETERMINISTIC = 280,
     PROTECTED = 281,
     ARROW_ASSIGN = 282,
     TILDE_ASSIGN = 283,
     EQUATION_ASSIGN = 284,
     WORKSPACE_ASSIGN = 285,
     REFERENCE_ASSIGN = 286,
     ADDITION_ASSIGN = 287,
     SUBTRACTION_ASSIGN = 288,
     MULTIPLICATION_ASSIGN = 289,
     DIVISION_ASSIGN = 290,
     DECREMENT = 291,
     INCREMENT = 292,
     EQUAL = 293,
     AND = 294,
     OR = 295,
     AND2 = 296,
     OR2 = 297,
     GT = 298,
     GE = 299,
     LT = 300,
     LE = 301,
     EQ = 302,
     NE = 303,
     END_OF_INPUT = 304,
     UNOT = 305,
     UPLUS = 306,
     UMINUS = 307,
     UAND = 308
   };
#endif
/* Tokens.  */
#define REAL 258
#define INT 259
#define NAME 260
#define STRING 261
#define RBNULL 262
#define RBTAB 263
#define FALSE 264
#define TRUE 265
#define FUNCTION 266
#define PROCEDURE 267
#define CLASS 268
#define FOR 269
#define IN 270
#define IF 271
#define ELSE 272
#define WHILE 273
#define NEXT 274
#define BREAK 275
#define RETURN 276
#define MOD_CONST 277
#define MOD_DYNAMIC 278
#define MOD_STOCHASTIC 279
#define MOD_DETERMINISTIC 280
#define PROTECTED 281
#define ARROW_ASSIGN 282
#define TILDE_ASSIGN 283
#define EQUATION_ASSIGN 284
#define WORKSPACE_ASSIGN 285
#define REFERENCE_ASSIGN 286
#define ADDITION_ASSIGN 287
#define SUBTRACTION_ASSIGN 288
#define MULTIPLICATION_ASSIGN 289
#define DIVISION_ASSIGN 290
#define DECREMENT 291
#define INCREMENT 292
#define EQUAL 293
#define AND 294
#define OR 295
#define AND2 296
#define OR2 297
#define GT 298
#define GE 299
#define LT 300
#define LE 301
#define EQ 302
#define NE 303
#define END_OF_INPUT 304
#define UNOT 305
#define UPLUS 306
#define UMINUS 307
#define UAND 308




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 80 "./grammar.y"
{
    char*                                           c_string;
    std::string*                                    string;
    double                                          realValue;
    int                                             intValue;
    bool                                            boolValue;
    RevLanguage::SyntaxElement*                     syntaxElement;
    RevLanguage::SyntaxVariable*                    syntaxVariable;
    RevLanguage::SyntaxFunctionCall*                syntaxFunctionCall;
    RevLanguage::SyntaxLabeledExpr*                 syntaxLabeledExpr;
    RevLanguage::SyntaxFormal*                      syntaxFormal;
    std::list<RevLanguage::SyntaxElement*>*         syntaxElementList;
    std::vector<std::string>*                       stringvector;
    std::list<RevLanguage::SyntaxLabeledExpr*>*     argumentList;
    std::list<RevLanguage::SyntaxFormal*>*          formalList;
}
/* Line 1529 of yacc.c.  */
#line 172 "./grammar.tab.h"
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
