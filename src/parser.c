/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"

Token *token;
ASTstruct *ast;

char *displayNodes[] = {"SEQ", "PROLOG", "NODE_DEF_FUNC", "NODE_PARAMS_RETURNTYPE", "RETURN_TYPE_INT", "RETURN_TYPE_FLOAT", "RETURN_TYPE_STRING", "NODE_PARAM_ID_INT", "NODE_PARAM_ID_FLOAT", "NODE_PARAM_ID_STRING","NODE_RETURN"};


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

    // zobere token z vrchola zasobniku
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

        root = createNode(SEQ, NULL, program(stack), createNode(PROLOG, NULL, NULL, NULL));
    }
    else
    {
        error_exit(SYN_ERR, "Syntax error! Prolog missing.");
    }


    return root;
}


ASTstruct *program(Stack *stack)
{

    DynamicString *func_name = NULL;
    ASTstruct *root = NULL;
    token = loadToken(stack);

    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_KEYWORD_FUNCTION)
    {
        root = createNode(SEQ, NULL, function_define(stack), stmt(stack));
    }
    else
    {
        // call stmt
    }

    return root;
}


ASTstruct *function_define(Stack *stack)
{
    ASTstruct *root = NULL;
    ASTstruct *func = NULL;
    ASTstruct *parameters = NULL;
    ASTstruct *returntype = NULL;
    ASTstruct *params_returntype = NULL;
    DynamicString *func_name = NULL;

    token = loadToken(stack);


    if (token->type == TOKEN_ID)
    {

        func_name = token->value.stringPtr;
        expectToken(TOKEN_L_PAR, stack);
        parameters = params(stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_COLON, stack);
        returntype = rt(stack);

        expectToken(TOKEN_L_BRACKET, stack);
        params_returntype = createNode(NODE_PARAMS_RETURNTYPE, NULL, parameters, returntype);

        func = createNode(NODE_DEF_FUNC, NULL, params_returntype, stmt(stack));
        func->value = func_name;
        expectToken(TOKEN_R_BRACKET, stack);

        root = createNode(SEQ, NULL, program(stack), func);
        
    }


    return root;
    
}


ASTstruct *rt(Stack *stack)
{
    ASTstruct *returntype = NULL;
    ASTstruct *returntypeNode = NULL;
    token = loadToken(stack);

    if (token->type == TOKEN_COLON)
    {
        returntype = getRT(stack);
        if (returntype != NULL)
        {
            returntypeNode = createNode(SEQ, NULL, returntype, NULL);
        }
 
        return returntypeNode;
    }
    else
    {
        unloadToken(stack);
        return NULL;
    }

}


ASTstruct *getRT(Stack *stack)
{
    token = loadToken(stack);

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

        default:
            error_exit(SYN_ERR, "Syntax error!");
    }
}


ASTstruct *params(Stack *stack)
{
    ASTstruct *param = NULL;
    DynamicString *value = NULL;
    token = loadToken(stack);

    switch(token->type)
    {
        case TOKEN_KEYWORD_INT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                value = token->value.stringPtr;
                param = createNode(NODE_PARAM_ID_INT, value, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Identifier expected");
            break;
            

        case TOKEN_KEYWORD_FLOAT:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                value = token->value.stringPtr;
                param = createNode(NODE_PARAM_ID_FLOAT, value, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Identifier expected");
            break;

        case TOKEN_KEYWORD_STRING:
            token = loadToken(stack);
            if (token->type == TOKEN_VAR_ID)
            {
                value = token->value.stringPtr;
                param = createNode(NODE_PARAM_ID_STRING, value, NULL, NULL);
                break;
            }
            error_exit(SYN_ERR, "Syntax error! Identifier expected");
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

ASTstruct *stmt(Stack *stack)
{
    
    //return root;
}



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
