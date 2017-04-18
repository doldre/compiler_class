#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#define MAXRESERVED 6


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
    IF, ELSE, INT, RETURN, VOID, WHILE,
    // multi character tokens
    ID, NUM,
    // special symbols
    PLUS, MINUS, TIMES, OVER, LT, LTEQ, GT, GTEQ, EQ, NEQ,
    ASSIGN, SEMI, COMMA, LSQUARE, RSQUARE, LPAREN, RPAREN,
    LBRACE, RBRACE
};


#endif
