/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"

Token *token;
ASTstruct *ast;

char *displayNodes[] = {"SEQ",
                        "NODE_PROLOG",
                        "NODE_FUNC_DEF",
                        "NODE_PARAMS_RETURNTYPE",
                        "RETURN_TYPE_INT",
                        "RETURN_TYPE_FLOAT",
                        "RETURN_TYPE_STRING",
                        "RETURN_TYPE_VOID",
                        "NODE_PARAM_ID_INT",
                        "NODE_PARAM_ID_FLOAT",
                        "NODE_PARAM_ID_STRING",
                        "NODE_RETURN",
                        "NODE_INT",
                        "NODE_FLOAT",
                        "NODE_STRING",
                        "NODE_VAR_ID",
                        "NODE_IF",
                        "NODE_ELSE",
                        "NODE_WHILE",
                        "NODE_VAR_ASSIGNMENT"};


precedence_table preced_table[] = {
        {TOKEN_VAR_ID, -1, NODE_VAR_ID},
        {TOKEN_INT, -1, NODE_INT},
        {TOKEN_FLOAT, -1, NODE_FLOAT},
        {TOKEN_STRING, -1, NODE_STRING},
        {TOKEN_SEMICOLON, -1, -1},

};


// funkcia vykona syntakticku analyzu tokenov a vytvori AST

int parser(Stack *stack)
{
    ast = parse(stack); 
    Print_tree(ast);

  return 0;
}

// zaciatok syntaktickej analyzy

ASTstruct *parse(Stack *stack)
{
    ASTstruct *root = NULL;
    
    do
    {
        root = createNode(SEQ, NULL, root, prolog(stack));
    } while (!StackIsEmpty(stack));

    return root;
}


ASTstruct *prolog(Stack *stack)
{
    ASTstruct *root = NULL;
    token = loadToken(stack);

    if (token == NULL)
    {
        error_exit(SYN_ERR, "Syntax error! Prolog missing.");
    }

    if (token->type == TOKEN_PROLOG)
    {
        expectToken(TOKEN_DECLARE, stack);
        expectToken(TOKEN_L_PAR, stack);
        expectToken(TOKEN_STRICT_TYPES, stack);
        expectToken(TOKEN_ASSIGN, stack);
        expectToken(TOKEN_INT, stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_SEMICOLON, stack);


        root = createNode(SEQ, NULL, program(stack), createNode(NODE_PROLOG, NULL, NULL, NULL));


    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Prolog missing.");
    }

    return root;
}


ASTstruct *program(Stack *stack)
{
    ASTstruct *root = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_KEYWORD_FUNCTION)
    {
        root = function_define(stack);
    }
    else
    {
        unloadToken(stack);
        root = stmt(stack);
    }

    return root;
}


ASTstruct *function_define(Stack *stack)
{
    ASTstruct *root = NULL;
    ASTstruct *parameters = NULL;
    ASTstruct *returntype = NULL;
    ASTstruct *params_returntype = NULL;
    ASTstruct *func = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;


    if (token->type == TOKEN_ID)
    {
        expectToken(TOKEN_L_PAR, stack);
        parameters = params(stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_COLON, stack);
        returntype = rt(stack);
        expectToken(TOKEN_L_BRACKET, stack);
        params_returntype = createNode(NODE_PARAMS_RETURNTYPE, NULL, parameters, returntype);
        
        func = createNode(NODE_FUNC_DEF, NULL, params_returntype, stmt(stack));

        expectToken(TOKEN_R_BRACKET, stack);

        root = createNode(SEQ, NULL, program(stack), func);
    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Function identifier expected.")
    }

    return root;
}


ASTstruct *params(Stack *stack)
{
    ASTstruct *param = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_INT, token->value.stringPtr, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        case TOKEN_KEYWORD_FLOAT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_FLOAT, token->value.stringPtr, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        case TOKEN_KEYWORD_STRING:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_STRING, token->value.stringPtr, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        default:
            unloadToken(stack);
            return NULL;
    }

    token = loadToken(stack);
    // viac parametrov
    if (token->type == TOKEN_COMMA)
    {
        return createNode(SEQ, NULL, params(stack), param);
    }
    // jeden parameter
    else
    {
        unloadToken(stack);
        return createNode(SEQ, NULL, NULL, param);
    }
}



ASTstruct *rt(Stack *stack)
{
    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            return createNode(RETURN_TYPE_INT, NULL, NULL, NULL);
            break;

        case TOKEN_KEYWORD_FLOAT:
            return createNode(RETURN_TYPE_FLOAT, NULL, NULL, NULL);
            break;

        case TOKEN_KEYWORD_STRING:
            return createNode(RETURN_TYPE_STRING, NULL, NULL, NULL);
            break;

        case TOKEN_KEYWORD_VOID:
            return createNode(RETURN_TYPE_VOID, NULL, NULL, NULL);
            break;

        default:
            error_exit(SYN_ERR, "Syntax error! Invalid returntype.");
    }

}


ASTstruct *stmt(Stack *stack)
{
    ASTstruct *root = NULL;
    ASTstruct *if_cond = NULL;
    ASTstruct *if_body = NULL;
    ASTstruct *else_body = NULL;
    ASTstruct *node_if = NULL;
    ASTstruct *node_else = NULL;
    ASTstruct *while_cond = NULL;
    ASTstruct *while_body = NULL;
    ASTstruct *while_node = NULL;
    ASTstruct *identif_node = NULL;
    ASTstruct *node_var_assignment = NULL;

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_KEYWORD_RETURN:
            root = createNode(SEQ, NULL, NULL, createNode(NODE_RETURN, NULL, expr(stack,0), NULL));
            expectToken(TOKEN_SEMICOLON, stack);
            break;

        case TOKEN_KEYWORD_IF:
            expectToken(TOKEN_L_PAR, stack);
            if_cond = createNode(SEQ, NULL, NULL, expr(stack,0));
            if (if_cond->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! Condition expected after 'if'.");
            }
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            if_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            expectToken(TOKEN_KEYWORD_ELSE, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            else_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            node_else = createNode(NODE_ELSE, NULL, else_body, if_body);
            node_if = createNode(NODE_IF, NULL, if_cond, node_else);
            root = createNode(SEQ, NULL, stmt(stack), node_if);
            break;

        case TOKEN_KEYWORD_WHILE:
            expectToken(TOKEN_L_PAR, stack);
            while_cond = createNode(SEQ, NULL, NULL, expr(stack,0));
            if (if_cond->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! Condition expected after 'while'.");
            }
            expectToken(TOKEN_R_PAR, stack);
            expectToken(TOKEN_L_BRACKET, stack);
            while_body = stmt(stack);
            expectToken(TOKEN_R_BRACKET, stack);
            while_node = createNode(NODE_WHILE, NULL, while_cond, while_body);
            root = createNode(SEQ, NULL, stmt(stack), while_node);
            break;

        case TOKEN_VAR_ID:
            expectToken(TOKEN_ASSIGN, stack);
            identif_node = createNode(NODE_VAR_ID, token->value.stringPtr, NULL, NULL);
            node_var_assignment = createNode(NODE_VAR_ASSIGNMENT, NULL, identif_node, expr(stack,0));
            if (node_var_assignment->rightNode == NULL)
            {
                error_exit(SYN_ERR, "Syntax error! 'R-value' expected in variable assignment.");
            }
            expectToken(TOKEN_SEMICOLON, stack);
            root = createNode(SEQ, NULL, stmt(stack), node_var_assignment);
            break;


        default:
            unloadToken(stack);
            return NULL;
            
    }

    return root;
}


ASTstruct *expr(Stack *stack, int preced)
{
    ASTstruct *root = NULL;
    ASTstruct *help = NULL;
    bool is_loaded = false;


    token = loadToken(stack);
    if (token == NULL) return NULL;

    switch (token->type)
    {
        case TOKEN_INT:
        case TOKEN_FLOAT:
        case TOKEN_STRING:
            root = createNode(NODE_INT, NULL, NULL, NULL);
            //token = loadToken(stack);
            //is_loaded = true;
            break;

        default:
            unloadToken(stack);
    }


    // kontrola precedencie
    /*while (token && preced_table[token->type].preced_value >= preced)
    {
        int op = token->type;
        help = expr(stack, preced_table[op].preced_value + 1);
        root = createNode(preced_table[op].node_type, NULL, root, help);
        is_loaded = false;
    }
    if (is_loaded)
    {
        unloadToken(stack);
    }*/

    return root;
}

ASTstruct *expr2(Stack *stack, int preced)
{
    return NULL;
}

ASTstruct *expr3(Stack *stack, int preced)
{
    return NULL;
}


ASTstruct *term(Stack *stack)
{

    token = loadToken(stack);
    if (token == NULL)
        return NULL;

    switch(token->type)
    {
        case TOKEN_INT:
            return createNode(NODE_INT, NULL, NULL, NULL);
            break;

        case TOKEN_FLOAT:
            return createNode(NODE_FLOAT, NULL, NULL, NULL);
            break;

        case TOKEN_STRING:
            return createNode(NODE_STRING, NULL, NULL, NULL);
            break;

        case TOKEN_VAR_ID:
            return createNode(NODE_VAR_ID, NULL, NULL, NULL);
            break;

        default:
            error_exit(SYN_ERR, "Syntax error! Term has to be INT, FLOAT, STRING, IDENTIFIER.");
    }

}



/**** PRINT AST ******/


void prt_ast(ASTstruct *t) {
    if (t == NULL)
        printf("NULL\n");
    else {
        printf("%s ", displayNodes[t->type]);
        //if (t->type == nIdentifier || t->type == nLitInt || t->type == nLitFloat
        //|| t->type == nLitString || t->type == nLitNone || t->type == nMulticomment) {
        if (t->type == NODE_PARAM_ID_INT ||t->type == NODE_PARAM_ID_FLOAT ||t->type == NODE_PARAM_ID_STRING ){
            printf("%s\n", (char*)t->value);
        } else {
            printf("\n");
            prt_ast(t->leftNode);
            prt_ast(t->rightNode);
        }
    }
}


void Print_tree2(ASTstruct* TempTree, char* sufix, char fromdir) {
/* vykresli sktrukturu binarniho stromu */
  if (TempTree != NULL) {
    char* suf2 = (char*) malloc(strlen(sufix) + 4);
    strcpy(suf2, sufix);
    
    if (fromdir == 'L') {
      suf2 = strcat(suf2, "  |");
      printf("%s\n", suf2);
    } else
      suf2 = strcat(suf2, "   ");
    
    Print_tree2(TempTree->rightNode, suf2, 'R');
    if (TempTree->value)
      printf("%s  +-[ (%d) %s \"%s\" ]\n", sufix, TempTree->type,  displayNodes[TempTree->type], (char*)TempTree->value);
    else
      printf("%s  +-[ (%d) %s ]\n", sufix, TempTree->type,  displayNodes[TempTree->type]);
    
    strcpy(suf2, sufix);
    
    if (fromdir == 'R')
      suf2 = strcat(suf2, "  |");    
    else
      suf2 = strcat(suf2, "   ");

    Print_tree2(TempTree->leftNode, suf2, 'L');

    if (fromdir == 'R')
      printf("%s\n", suf2);

    free(suf2);
  }
}

void Print_tree(ASTstruct* TempTree) {
  printf("===========================================\n");
  printf("Struktura binarniho stromu:\n");
  printf("\n");

  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("Strom je prazdny...\n");

  printf("\n");
  printf("===========================================\n");
} 


/**** PRINT AST ******/