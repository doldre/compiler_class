#include "globals.h"

int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;


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

    return 0;
}
