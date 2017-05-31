/************************************************
 *  Author        :mathon
 *  Email         :luoxinchen96@gmail.com
 *  Time          :2017年 4月13日 星期四
*************************************************/

#include "globals.h"
#include "scan.h"
#include "util.h"

typedef enum {
    START, INCOMMENT, INNUM, INID, INASSIGN, DONE
} StateType;

char tokenString[MAXTOKENLEN+1];

#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in lineBuf */
static int bufsize = 0; /* current size of buffer string */
static bool EOF_flag = false; /* corrects ungetNextChar behavior on EOF */

static int getNextChar(void)
{
    if (linepos >= bufsize) {
        lineno++;
        if (fgets(lineBuf, BUFLEN - 1, source)) {
            if (EchoSource) fprintf(listing, "%d: %s", lineno, lineBuf);
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        } else {
            EOF_flag = true;
            return EOF;
        }
    } else return lineBuf[linepos++];
}

static void ungetNextChar(void) {
    if (!EOF_flag) linepos--;
}

static struct {
    const char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"read", READ},
    {"write", WRITE},
    {"if", IF},
    {"then", THEN},
    {"end", END},
    {"repeat", REPEAT},
    {"until", UNTIL},
    {"else", ELSE}
};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */

static TokenType reservedLookup(char *s) 
{
    for (int i = 0; i < MAXRESERVED; i++) {
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    }
    return ID;
}

/***************************************/
/* the primary function of the scanner */
/***************************************/

/* function getToken returns the next
 * token in source file
 */

TokenType getToken(void)
{
    /* index for storing into tokenString */
    int tokenStringIndex = 0;
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state: always begins at START */
    StateType state = START;
    /* flag to indicate save to tokenString */
    bool save;

    while (state != DONE) {
        int c = getNextChar();
        save = true;
        switch(state) {
        case START:
            if(isdigit(c))
                state = INNUM;
            else if(isalpha(c))
                state = INID;
            else if(c == ':')
                state = INASSIGN;
            else if(isspace(c))
                save = false;
            else if(c == '{')
                save = false, state = INCOMMENT;
            else {
                state = DONE;
                switch(c) {
                case EOF:
                    save = false;
                    currentToken = ENDFILE;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '/':
                    currentToken = OVER;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case '=':
                    currentToken = EQ;
                    break;
                case '<':
                    currentToken = LT;
                    break;
                case '>':
                    currentToken = GT;
                    break;
                default:
                    // printf("c == %d\n", c);
                    currentToken = ERROR;
                    break;
                }
            }
            break;
        case INCOMMENT:
            save = false;
            if (c == EOF) {
                state = DONE;
                currentToken = ENDFILE;
            } else if (c == '}') state = START;
            break;
        case INNUM:
            if(!isdigit(c)) {
                ungetNextChar();
                save = false;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if(!isalpha(c)) {
                ungetNextChar();
                save = false;
                state = DONE;
                currentToken = ID;
            }
            break;
        case INASSIGN:
            if(c == '=') {
               state = DONE;
               currentToken = ASSIGN;
            } else {
                currentToken = ERROR;
            }
            break;
        case DONE:
        default:
            fprintf(listing, "Scanner bug: state = %d\n", state);
            state = DONE;
            currentToken = ERROR;
            break;
        }
        if (save && (tokenStringIndex <= MAXTOKENLEN))
            tokenString[tokenStringIndex++] = (char)c;
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan) {
        fprintf(listing, "%d:\t", lineno);
        printToken(currentToken, tokenString);
    }

    return currentToken;
}
