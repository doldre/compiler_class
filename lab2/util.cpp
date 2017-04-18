#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString)
{
    switch(token) {
    case IF ... WHILE:
        fprintf(listing, "reserved word: %s\n", tokenString);
        break;
    case ENDFILE: fprintf(listing, "EOF\n"); break;
    case ERROR: 
        fprintf(listing, "ERROR: %s\n", tokenString);
        break;
    case ID:
        fprintf(listing, "ID, name = %s\n", tokenString);
        break;
    case NUM:
        fprintf(listing, "NUM, val = %s\n", tokenString);
        break;
    case PLUS ... RBRACE:
        fprintf(listing, "%s\n", tokenString);
        break;
    default:
        fprintf(listing, "Unknown token: %d\n", token);
    }
}
