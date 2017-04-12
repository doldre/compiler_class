#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#define ENDFILE 0

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef int TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;


#define ELSE 258
#define IF 259
#define INT 260
#define RETURN 261
#define VOID 262
#define WHILE 263
#define ID 264
#define NUM 265
#define PLUS 266
#define MINUS 267
#define TIMES 268
#define OVER 269
#define LT 270
#define LTEQ 271
#define GT 272
#define GTEQ 273
#define EQ 274
#define NEQ 275
#define ASSIGN 276
#define SEMI 277
#define COMMA 278
#define LPAREN 279
#define RPAREN 280
#define LBRACK 281
#define RBRACK 282
#define LBRACE 283
#define RBRACE 284
#define ERROR 285

#endif
