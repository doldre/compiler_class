#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#define MAXRESERVED 8


extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;

extern bool EchoSource;
extern bool TraceScan;

enum TokenType {
    // book-keeping tokens
    ENDFILE, ERROR,
    // reserved words
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    // multi character tokens
    ID, NUM,
    // special symbols
    ASSIGN, EQ, LT, GT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
};


#endif
