#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType, const char *);


/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

char* copyString(char *);

void printNode(TreeNode* rt, int deep);

#endif
