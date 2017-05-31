#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
    switch(token) {
    case IF ... WRITE:
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
    case ASSIGN:
        fprintf(listing, ":="); break;
    case EQ:
        fprintf(listing, "="); break;
    case LT:
        fprintf(listing, "<"); break;
     case GT:
        fprintf(listing, ">"); break;
    case PLUS:
        fprintf(listing, "+"); break;
    case MINUS:
        fprintf(listing, "-"); break;
    case TIMES:
        fprintf(listing, "*"); break;
    case OVER: 
        fprintf(listing, "/"); break;
    case LPAREN:
        fprintf(listing, "("); break;
    case RPAREN:
        fprintf(listing, ")"); break;
    case SEMI:
        fprintf(listing, ";"); break;
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

void printNode(TreeNode* rt, int deep)
{
    char str[MAXCHILDREN + 20];
    if (rt->nodekind == StmtK) {
        switch (rt->kind.stmt) {
        case IfK:
            iprint("IF", deep);
            fprintf(listing, "\n");
            if (rt->child[0] != NULL) {
                printNode(rt->child[0], deep + 1);
            }
            if (rt->child[1] != NULL) {
                // iprint("THEN", deep);
                printNode(rt->child[1], deep + 1);
            }
            if (rt->child[2] != NULL) {
                // iprint("ELSE", deep);
                printNode(rt->child[2], deep + 1);
            }
            break;
        case RepeatK:
            iprint("Repeat", deep);
            fprintf(listing, "\n");
            if (rt->child[0] != NULL) {
                printNode(rt->child[0], deep + 1);
            }
            // iprint("UNTIL", deep);
            if (rt->child[1] != NULL) {
                printNode(rt->child[1], deep + 1);
            }
            break;
        case AssignK:
            sprintf(str, "Assign to: %s", rt->attr.name);
            iprint(str, deep);
            fprintf(listing, "\n");
            if (rt->child[0] != NULL) {
                printNode(rt->child[0], deep + 1);
            }
            break;
        case ReadK:
            sprintf(str, "Read: %s", rt->attr.name);
            iprint(str, deep);
            fprintf(listing, "\n");
            break;
        case WriteK:
            iprint("Write", deep);
            fprintf(listing, "\n");
            if (rt->child[0] != NULL) {
                printNode(rt->child[0], deep + 1);
            }
            break;
        }
    } else if(rt->nodekind == ExpK) {
        switch(rt->kind.exp) {
        case OpK:
            sprintf(str, "Op: ");
            iprint(str, deep);
            printToken(rt->attr.op, "");
            if (rt->child[0] != NULL) {
                printNode(rt->child[0], deep + 1);
            }
            if (rt->child[1] != NULL) {
                printNode(rt->child[1] , deep + 1);
            }
            break;
        case ConstK:
            sprintf(str, "Const: %d", rt->attr.val);
            iprint(str, deep + 1);
            fprintf(listing, "\n");
            break;
        case IdK:
            sprintf(str, "Id: %s", rt->attr.name);
            iprint(str, deep + 1);
            fprintf(listing, "\n");
            break;
        }
    }
    if (rt->sibling != NULL) {
        printNode(rt->sibling, deep);
    }
}