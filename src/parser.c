/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"

Token *token;
ASTstruct *ast;

char *displayNodes[] = {"SEQ", "NODE_PROLOG", "NODE_FUNC_DEF", "NODE_PARAMS_RETURNTYPE", "RETURN_TYPE_INT", "RETURN_TYPE_FLOAT", "RETURN_TYPE_STRING", "NODE_PARAM_ID_INT", "NODE_PARAM_ID_FLOAT", "NODE_PARAM_ID_STRING","NODE_RETURN"};


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

    switch(token->type)
    {
        case TOKEN_KEYWORD_FUNCTION:
            root = createNode(SEQ, NULL, function_define(stack), NULL);
            break;

        case TOKEN_KEYWORD_IF:
            root = createNode(SEQ, NULL, stmt(stack), NULL);
            break;

        case TOKEN_KEYWORD_WHILE:
            root = createNode(SEQ, NULL, stmt(stack), NULL);
            break;

        case TOKEN_VAR_ID:
            root = createNode(SEQ, NULL, stmt(stack), NULL);
            break;

        case TOKEN_ID:
            root = createNode(SEQ, NULL, stmt(stack), NULL);
            break;

        default:
            unloadToken(stack);
            return NULL;
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


    if (token->type == TOKEN_ID)
    {
        expectToken(TOKEN_L_PAR, stack);
        parameters = params(stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_COLON, stack);
        returntype = rt(stack);
        expectToken(TOKEN_L_BRACKET, stack);
        if (parameters != NULL || returntype != NULL)
            params_returntype = createNode(NODE_PARAMS_RETURNTYPE, NULL, parameters, returntype);
        
        func = createNode(NODE_FUNC_DEF, NULL, params_returntype, stmt(stack));

        expectToken(TOKEN_R_BRACKET, stack);

        root = createNode(SEQ, NULL, function_define(stack), func);
    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Function identifier expected.")
    }

    return root;
}


ASTstruct *params(Stack *stack)
{
    ASTstruct *root = NULL;
    DynamicString *value = NULL;
    ASTstruct *param = NULL;

    token = loadToken(stack);

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_INT, token->value.integer, NULL, NULL); // TO FIX
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        case TOKEN_KEYWORD_FLOAT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_FLOAT, token->value.decimal, NULL, NULL); // TO FIX
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        case TOKEN_KEYWORD_STRING:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                param = createNode(NODE_PARAM_ID_STRING, token->value.stringPtr, NULL, NULL); // TO FIX
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Variable identifier expected!");
            break;

        default:
            unloadToken(stack);
            return NULL;
    }

    token = loadToken(stack);
    if (token->type == TOKEN_COMMA)
    {
        return createNode(SEQ, NULL, params(stack), param);
    }
    else
    {
        unloadToken(stack);
        return createNode(SEQ, NULL, NULL, param);
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