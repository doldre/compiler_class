#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#define MAXRESERVED 8

#define MAXTOKENLEN 40

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

enum NodeKind {StmtK, ExpK};
enum StmtKind {IfK, RepeatK, AssignK, ReadK, WriteK};
enum ExpKind {OpK, ConstK, IdK};

const int MAXCHILDREN = 3;

struct TreeNode 
{
    TreeNode *child[MAXCHILDREN];
    TreeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union {
        TokenType op;
        int val;
        int idx;
        char * name;
    } attr;

    TreeNode() {
        memset(child, NULL, sizeof(child));
        sibling = NULL;
    }
};
#endif
