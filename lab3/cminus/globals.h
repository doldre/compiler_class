#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <vector>
#include <string>

#define MAXRESERVED 6

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
    IF, ELSE, INT, RETURN, VOID, WHILE,
    // multi character tokens
    ID, NUM,
    // special symbols
    PLUS, MINUS, TIMES, OVER, LT, LTEQ, GT, GTEQ, EQ, NEQ,
    ASSIGN, SEMI, COMMA, LSQUARE, RSQUARE, LPAREN, RPAREN,
    LBRACE, RBRACE
};


enum NodeKind {StmtK, ExpK, DeclaK, TypeK};   // type of node: expression, statement
enum StmtKind {IfK, WhileK, AssignK, CompK, CallK, VoidStmtK}; // type of sub of statment: ...
enum DeclKind {Var_DeclK, Funck, Arry_DeclK, ParamsK, VoidK, ParamK};
enum ExpKind {OpK, ConstK, IdK, Arry_ElemK, ReturnK, ArgsK};   // type of sub of expression: ...
enum ExpType {Void, Integer}; // value of expression


const int MAXCHILDREN = 4;

struct TreeNode 
{
    TreeNode *child[MAXCHILDREN];
    TreeNode *sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; DeclKind decl; } kind;
    union {
        TokenType op;
        int val;
        int idx;
        char * name;
    } attr;

    TreeNode() {
        memset(child, 0, sizeof(child));
        sibling = NULL;
    }
    ExpType exptype;
};

struct TokenNode 
{
    TokenType token;
    std::string tokenString;
    int lineno;
    TokenNode(TokenType token, std::string tokenString, int lineno):
        token(token), tokenString(tokenString), lineno(lineno) {}
    TokenNode(){}
};

#endif
