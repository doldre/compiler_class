#include "globals.h"
#include "scan.h"

typedef enum {
    START, INLT, INGL, INEQ, INNOT, INSLASH, INCOMMENT, INCOMMENTSTART,
    INNUM, INID, DONE
} StateType;

