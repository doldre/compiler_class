#include "scan.h"
#include "globals.h"
#include "util.h"
#include "parser.h"

static TokenType token;

static TreeNode* declaration_list();
static TreeNode* declaration();
static TreeNode* var_declaration();
static TreeNode* type_specifier();
static TreeNode* params();
static TreeNode* param_list();
static TreeNode* param();
static TreeNode* compound_stmt();
static TreeNode* local_declarations();
static TreeNode* statement_list();
static TreeNode* statement();
static TreeNode* expression_stmt();
static TreeNode* selection_stmt();
static TreeNode* iteration_stmt();
static TreeNode* return_stmt();
static TreeNode* expression();
static TreeNode* var();
static TreeNode* simple_expression();
static TreeNode* additive_expression();
static TreeNode* term();
static TreeNode* factor();
static TreeNode* call();
static TreeNode* args();
static TreeNode* arg_list();
static bool is_relop(TokenType);
static void printOp(const TokenType&);
static void printStmtNode(TreeNode*&, int);
static void printExpNode(TreeNode*&, int);
static void printDeclNode(TreeNode*&, int);
static void printTypeNode(TreeNode*&);
static void printTree(TreeNode*&, int);
static TokenType getToken();
static void recovery(int);
bool Error;


std::vector<TokenNode> token_list;
int tokenno;

TokenType getToken() 
{
    if (tokenno < token_list.size()) {
        strcpy(tokenString, token_list[tokenno].tokenString.c_str());
        lineno = token_list[tokenno].lineno;
        return token_list[tokenno++].token;
    } else {
        return ENDFILE;
    }
}

void recovery(int no) 
{
    tokenno = no;
    token = token_list[no - 1].token;
    strcpy(tokenString, token_list[no - 1].tokenString.c_str());
    lineno = token_list[no - 1].lineno;
}

static void syntaxError(const char* message)
{
    fprintf(listing, ">> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = true;
}

static void match(TokenType expected) 
{
    if (token == expected) token = getToken();
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }
}


// declaration_list -> declaration_list declaration | declaration
TreeNode* declaration_list()
{
    TreeNode* rt = declaration();
    TreeNode* pre = rt;
    while(token != ENDFILE) {
        TreeNode* cur = declaration();
        if (cur != NULL) {
            if (rt == NULL) rt = pre = cur;
            else {
                pre->sibling = cur;
                pre = cur;
            }
        }
    }
    return rt;
}

// declaration -> var_declaration | fun_declaration
TreeNode* declaration()
{
    TreeNode* rt = newDeclNode(Var_DeclK);
    rt->child[0] = type_specifier();

    char* id_name = NULL;
    if (token == ID) {
        TreeNode* p = newExpNode(IdK);
        id_name = copyString(tokenString);
        p->attr.name = id_name;
        match(ID);
        rt->child[1] = p;
    } else {
        syntaxError("declaration: unexpected token->");
        printToken(token, tokenString);
    }

    if(rt != NULL && token == LSQUARE) {
        match(LSQUARE);
        TreeNode* p = newDeclNode(Arry_DeclK);
        if (token == NUM) {
            p->child[0] = newExpNode(IdK);
            p->child[0]->attr.name = id_name;
            p->child[1] = newExpNode(ConstK);
            p->child[1]->attr.val = atoi(tokenString);
        }
        rt->child[1] = p;
        match(NUM); match(RSQUARE);
    }
    if (token == SEMI) match(SEMI);
    else if (rt != NULL && token == LPAREN) {
        rt->kind.decl = Funck;
        match(LPAREN);
        rt->child[2] = params();
        match(RPAREN);
        rt->child[3] = compound_stmt();
    }
    return rt;
}

TreeNode* var_declaration()
{
    TreeNode* rt = NULL;
    if (token == INT || token == VOID) {
        rt = newDeclNode(Var_DeclK);
        rt->child[0] = type_specifier();
    } else return rt;
    char *id_name = NULL;
    if (token == ID) {
        TreeNode* p = newExpNode(IdK);
        id_name = copyString(tokenString);
        p->attr.name = id_name;
        match(ID);
        rt->child[1] = p;
    } else {
        syntaxError("var_declaration: unexpected token->");
        printToken(token, tokenString);
    }

    if (rt != NULL && token == LSQUARE) {
        TreeNode* p = newDeclNode(Arry_DeclK);
        p->attr.name = id_name;
        rt->child[1] = p;
        if (token == NUM) {
            rt->child[2] = newExpNode(ConstK);
            rt->child[2]->attr.val = atoi(tokenString);
        }
        match(NUM); match(RSQUARE);
    }
    match(SEMI);
    return rt;
}

// type_specifier -> INT | VOID

TreeNode* type_specifier() 
{
    TreeNode* rt = NULL;
    if (token == INT) {
        rt = new TreeNode();
        rt->nodekind = TypeK;
        rt->exptype = Integer;
        match(INT);
    } else if (token == VOID) {
        rt = new TreeNode();
        rt->nodekind = TypeK;
        rt->exptype = Void;
        match(VOID);
    } else {
        syntaxError("type_specifier: unexpected token->");
        printToken(token, tokenString);
    }
    return rt;
}

TreeNode* params() {
    TreeNode* rt = newDeclNode(ParamsK);
    if (token == VOID) {
        rt->child[0] = newDeclNode(VoidK);
        match(VOID);
    } else rt->child[0] = param_list();
    return rt;
}

TreeNode* param_list() 
{
    TreeNode* rt = param();
    TreeNode* pre = rt;
    while(token == COMMA) {
        TreeNode* cur = NULL;
        match(COMMA);
        cur = param();
        if (cur != NULL) {
            if (rt == NULL) {
                rt = pre = cur;
            } else {
                pre->sibling = cur;
                pre = cur;
            }
        }
    }
    return rt;
}

TreeNode* param()
{
    TreeNode* rt = newDeclNode(ParamK);
    rt->child[0] = type_specifier();

    if (token == ID) {
        TreeNode* p = newExpNode(IdK);
        p->attr.name = copyString(tokenString);
        match(ID);
        rt->child[1] = p;
    } else {
        syntaxError("param: unexpected token->");
        printToken(token, tokenString);
    }

    // param may be array;
    if (token == LSQUARE) {
        match(LSQUARE);
        match(RSQUARE);
    }
    return rt;
}

TreeNode* compound_stmt() 
{
    TreeNode* rt = newStmtNode(CompK);
    match(LBRACE);
    if (rt != NULL) rt->child[0] = local_declarations();
    if (rt != NULL) rt->child[1] = statement_list();
    match(RBRACE);
    return rt;
}

TreeNode* local_declarations() 
{
    TreeNode* rt = var_declaration();
    TreeNode* pre = rt;
    while(token == INT || token == VOID) {
        TreeNode* cur;
        cur = var_declaration();
        if (cur != NULL) {
            if (rt == NULL) {
                rt = pre = cur;
            } else {
                pre->sibling = cur;
                pre = cur;
            }
        }
    }
    return rt;
}

TreeNode* statement_list() 
{
    TreeNode* rt = statement();
    TreeNode* pre = rt;
    while(token != ENDFILE && token != RBRACE && token != ELSE) {
        TreeNode* cur;
        cur = statement();
        if (cur != NULL) {
            if (rt == NULL) {
                rt = pre = cur;
            } else {
                pre->sibling = cur;
                pre = cur;
            }
        }
    }
    return rt;
}

TreeNode* statement() 
{
    TreeNode* rt = NULL;
    switch(token) {
        case IF: rt = selection_stmt(); break;
        case WHILE: rt = iteration_stmt(); break;
        case ID: rt = expression_stmt(); break;
        case COMMA: rt = expression_stmt(); break;
        case LBRACE: rt = compound_stmt(); break;
        case RETURN: rt = return_stmt(); break;
        default:
            syntaxError("stamtent: unexpected token->");
            printToken(token, tokenString);
            break;
    }
    return rt;
}

// expression_stmt -> expression ; | ;
TreeNode* expression_stmt()
{
    TreeNode* rt = NULL;
    if (token == SEMI) {
        rt = newStmtNode(VoidStmtK);
    } else {
        rt = expression();
    }
    match(SEMI);
    return rt;
}

TreeNode* selection_stmt()
{
    TreeNode* rt = newStmtNode(IfK);
    match(IF);
    match(LPAREN);
    if (rt != NULL) rt->child[0] = expression();
    match(RPAREN);
    if (rt != NULL) rt->child[1] = compound_stmt();
    if (token == ELSE) {
        match(ELSE);
        if (rt != NULL) rt->child[2] = compound_stmt();
    }
    return rt;
}

TreeNode* iteration_stmt()
{
    TreeNode* rt = newStmtNode(WhileK);
    match(WHILE);
    match(LPAREN);
    if (rt != NULL) rt->child[0] = expression();
    match(RPAREN);
    if (rt != NULL) rt->child[1] = compound_stmt();
    return rt;
}

TreeNode* return_stmt()
{
    TreeNode* rt = newExpNode(ReturnK);
    match(RETURN);
    rt->child[0] = expression();
    match(SEMI);
    return rt;
}


//expression -> var = expression | simple_expression
TreeNode* expression()
{
    TreeNode* rt = NULL;
    int old_tokenno = tokenno;
    if (token == ID) {
        rt = newStmtNode(AssignK);
        rt->child[0] = var();
        // match(ID);
        if (token == ASSIGN) {
            match(ASSIGN);
            rt->child[1] = expression();
        } else {
            recovery(old_tokenno);
            delete rt->child[0];
            delete rt;
            rt = simple_expression();
        } 
    } else rt = simple_expression();
    return rt;
}

TreeNode* var()
{
    TreeNode* rt = newExpNode(IdK);
    char* id_name = copyString(tokenString);
    rt->attr.name = id_name;
    match(ID);
    if(token == LSQUARE) {
        TreeNode* p = newExpNode(Arry_ElemK);
        p->child[0] = newExpNode(IdK);
        p->child[0]->attr.name = id_name;
        delete rt;
        rt = p;
        match(LBRACE);
        rt->child[1] = expression();
        match(RBRACE);
    }
    return rt;
}

// simple_expression->additive_expression relop additive_expression | additive_expression
TreeNode* simple_expression()
{
    TreeNode* rt = additive_expression();
    if (is_relop(token)) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = rt;
            p->attr.op = token;
            match(token);
            p->child[1] = additive_expression();
            rt = p;
        }
    }
    return rt;
}


TreeNode* additive_expression()
{
    TreeNode* rt = term();
    while(token == PLUS || token == MINUS) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = rt;
            p->attr.op = token;
            match(token);
            p->child[1] = term();
            rt = p;
        }
    }
    return rt;
}

TreeNode* term()
{
    TreeNode* rt = factor();
    while(token == TIMES || token == OVER) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = rt;
            p->attr.op = token;
            match(token);
            p->child[1] = factor();
            rt = p;
        }
    }
    return rt;
}

TreeNode* factor() 
{
    TreeNode* rt = NULL;
    switch(token) {
    case LPAREN:
        match(LPAREN); rt = expression(); match(RPAREN);
        break;
    case ID: {
        int old_tokenno = tokenno;
        match(ID);
        if (token == LPAREN) {
            recovery(old_tokenno);
            rt = call();
        } else {
            recovery(old_tokenno);
            rt = var();
        }
        break;
    }
    case NUM:
        rt = newExpNode(ConstK);
        rt->attr.val = atoi(tokenString);
        match(NUM);
        break;
    default:
        // syntaxError("factor: unexpected token->");
        // printToken(token, tokenString);
        break;
    }
    return rt;
}

TreeNode* call()
{
    TreeNode* rt = newStmtNode(CallK);
    if (token == ID) {
        TreeNode* p = newExpNode(IdK);
        p->attr.name = copyString(tokenString);
        rt->child[0] = p;
    }
    match(ID);
    match(LPAREN);
    rt->child[1] = args();
    match(RPAREN);
    return rt;
}

TreeNode* args()
{
    TreeNode * rt = arg_list();
    return rt;
}

TreeNode* arg_list()
{
    TreeNode* rt = expression();
    TreeNode* p = rt;
    while(token == COMMA) {
        TreeNode* q;
        match(COMMA);
        q = expression();
        if (q != NULL) {
            if (rt == NULL) {
                rt = p = q;
            } else {
                p->sibling = q;
                p = q;
            }
        }
    }
    return rt;
}

bool is_relop(TokenType mtoken)
{
    return mtoken >= LT && mtoken <= NEQ;
}



void printOp(const TokenType& token) {
      switch (token) {
        case PLUS:
            printf("Op: +\n");
            break;
        case OVER:
            printf("Op: /\n");
            break;
        case MINUS:
            printf("Op: -\n");
            break;
        case TIMES:
            printf("Op: *\n");
            break;
        case LT:
            printf("Op: <\n");
            break;
        case LTEQ:
            printf("Op: <=\n");
            break;
        case GT:
            printf("Op: >\n");
            break;
        case GTEQ:
            printf("Op: >=\n");
            break;
        case ASSIGN:
            printf("Op: =\n");
            break;
        case EQ:
            printf("Op: ==\n");
            break;
        case NEQ:
            printf("Op: !=\n");
            break;
        default:
            break;
    }
}

void printStmtNode(TreeNode*& rt, int deep) 
{
    switch(rt->kind.stmt) {
    case IfK: printf("If\n"); break;
    case WhileK: printf("While\n"); break;
    case AssignK: printf("Assign\n"); break;
    case CompK: printf("Compound Statement\n"); break;
    case CallK: printf("Call\n"); break;
    default:
        break;
    }
    printTree(rt->child[0], deep + 1);
    printTree(rt->child[1], deep + 1);
    if (rt->kind.stmt == IfK && rt->child[2] != NULL) {
        for (int i = 0; i < deep; i++) printf("    ");
        printf("ELSE\n");
        printTree(rt->child[2], deep + 1);
    }
}

void printExpNode(TreeNode*& rt, int deep)
{
    switch(rt->kind.exp) {
    case OpK:
        printOp(rt->attr.op);
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        break;
    case ConstK:
        printf("Const: %d\n", rt->attr.val);
        break;
    case IdK:
        printf("ID: %s\n", rt->attr.name);
        break;
    case Arry_ElemK:
        printf("Ararry\n");
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        break;
    case ReturnK:
        printf("Return\n");
        printTree(rt->child[0], deep + 1);
        break;
    default:
        break;
    }
}

void printDeclNode(TreeNode*& rt, int deep) 
{
    switch(rt->kind.decl) {
    case Var_DeclK:
        printf("Var\n");
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        break;
    case Arry_DeclK:
        printf("Ararry Declaration\n");
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        break;
    case Funck:
        printf("Function\n");
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        printTree(rt->child[2], deep + 1);
        printTree(rt->child[3], deep + 1);
        break;
    case ParamsK:
        printf("Params\n");
        printTree(rt->child[0], deep + 1);
        break;
    case VoidK:
        printf("Void\n");
        printTree(rt->child[0], deep + 1);
        break;
    case ParamK:
        printf("Param\n");
        printTree(rt->child[0], deep + 1);
        printTree(rt->child[1], deep + 1);
        break;
    default:
        break;
    }
}

void printTypeNode(TreeNode*& rt) 
{
    switch(rt->exptype) {
    case Void:
        printf("Void\n");
        break;
    case Integer:
        printf("Integer\n");
        break;
    default:
        break;
    }
}

void printTree(TreeNode*& rt, int deep)
{
    if (NULL == rt) return;
    for (int i = 0; i < deep; i++) printf("    ");
    if (rt->nodekind == StmtK) printStmtNode(rt, deep);
    else if (rt->nodekind == ExpK) printExpNode(rt, deep);
    else if (rt->nodekind == DeclaK) printDeclNode(rt, deep);
    else if (rt->nodekind == TypeK) printTypeNode(rt);
    printTree(rt->sibling, deep);
}


TreeNode* parse()
{
    tokenno = 0;
    token_list = getTokenList();
    token = getToken();
    TreeNode* rt = declaration_list();
    if (token != ENDFILE) {
        printf("Code ends befor file\n");
    }
    return rt;
}

void show_tree(TreeNode* rt)
{
    printTree(rt, 0);
}