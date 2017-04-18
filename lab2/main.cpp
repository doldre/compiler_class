/************************************************
 *  Author        :mathon
 *  Email         :luoxinchen96@gmail.com
 *  Time          :2017年 4月13日 星期四
*************************************************/
#include "globals.h"
#include "scan.h"

int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

bool EchoSource = false;
bool TraceScan = true;

int main(int argc, char *argv[]) 
{
    char pgm[120];
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    strcpy(pgm, argv[1]);
    source = fopen(pgm, "r");

    if (source == NULL) {
        fprintf(stderr, "File %s not found\n", pgm);
    }
    listing = stdout;
    fprintf(listing, 
            "/*************************************************/\n"
            "TINY Lexical Analazer Result\n"
            "/*************************************************/\n\n");
    while(getToken() != ENDFILE);
    return 0;
}
