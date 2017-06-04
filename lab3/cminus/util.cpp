#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
    switch(token) {
    case IF ... WHILE:
        fprintf(listing, "reserved word: %s", tokenString);
        break;
    case ENDFILE: fprintf(listing, "EOF"); break;
    case ERROR: 
        fprintf(listing, "ERROR: %s", tokenString);
        break;
    case ID:
        fprintf(listing, "ID, name = %s", tokenString);
        break;
    case NUM:
        fprintf(listing, "NUM, val = %s", tokenString);
        break;
    // case ASSIGN ... SEMI:
    //     fprintf(listing, "%s\n", tokenString);
    //     break;
    case PLUS:
        fprintf(listing, "+"); break;
    case MINUS:
        fprintf(listing, "-"); break;
    case TIMES:
        fprintf(listing, "*"); break;
    case OVER: 
        fprintf(listing, "/"); break;
    case LT:
        fprintf(listing, "<"); break;
    case LTEQ:
        fprintf(listing, "<="); break;
    case GT:
        fprintf(listing, ">"); break;
    case GTEQ:
        fprintf(listing, ">="); break;
    case EQ:
        fprintf(listing, "=="); break;
    case NEQ:
        fprintf(listing, "!="); break;
    case ASSIGN:
        fprintf(listing, "="); break;
    case SEMI:
        fprintf(listing, ";"); break;
    case COMMA:
        fprintf(listing, ","); break;
    case LSQUARE:
        fprintf(listing, "["); break;
    case RSQUARE:
        fprintf(listing, "]"); break;
    case LPAREN:
        fprintf(listing, "("); break;
    case RPAREN:
        fprintf(listing, ")"); break;
    case LBRACE:
        fprintf(listing, "{"); break;
    case RBRACE:
        fprintf(listing, "}"); break;
    default:
        fprintf(listing, "Unknown token: %d\n", token);
    }
    fprintf(listing, "\n");
}


TreeNode* newStmtNode(StmtKind kind)
{
    TreeNode* t = new TreeNode();
    if (t == NULL) {
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    } else {
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}


TreeNode* newExpNode(ExpKind kind)
{
    TreeNode* t = new TreeNode();
    if (t == NULL) {
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    } else {
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode* newDeclNode(DeclKind kind)
{
    TreeNode* t = new TreeNode();
    t->nodekind = DeclaK;
    t->kind.decl = kind;
    return t;
}

char* copyString(char* s) 
{
    if (s == NULL) return NULL;
    int n = strlen(s);
    char *t = (char*)malloc(n);
    if (t == NULL) {
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    } else strcpy(t, s);
    return t;
}

void iprint(const char* str, int deep) 
{
    for (int i = 0; i < deep; i++) fprintf(listing, "  ");
    fprintf(listing, "%s", str);
}
