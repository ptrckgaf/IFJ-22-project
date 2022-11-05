/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"

Token *token;
ASTstruct *ast;


// funkcia vykona syntakticku analyzu tokenov a vytvori AST

int parser(Stack *stack)
{
    ast = parse(stack);


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
        fprintf(stderr, "Syntax error! Prolog missing.");
    }

    if (token->type == TOKEN_PROLOG)
    {
        expectToken(TOKEN_ID, stack);
        expectToken(TOKEN_L_PAR, stack);
        expectToken(TOKEN_ID, stack);
        expectToken(TOKEN_ASSIGN, stack);
        expectToken(TOKEN_INT, stack);
        expectToken(TOKEN_R_PAR, stack);
        expectToken(TOKEN_SEMICOLON, stack);

        root = createNode(SEQ, NULL, program(stack), createNode(PROLOG, NULL, NULL, NULL));
    }
    else
    {
        fprintf(stderr, "Syntax error! Prolog missing.");
    }


    return root;
}


ASTstruct *program(Stack *stack)
{
    char *func_name = NULL;
    ASTstruct *parameters = NULL;
    ASTstruct *returntype = NULL;
    ASTstruct *func = NULL;
    ASTstruct *root = NULL;
    ASTstruct *params_returntype = NULL;

    token = loadToken(stack);
    if (token == NULL) return NULL;

    if (token->type == TOKEN_KEYWORD_FUNCTION)
    {
        token = loadToken(stack);

        if (token->type == TOKEN_ID)
        {
            // get func_name from token value
        }
        else
        {
            fprintf(stderr, "Syntax error!");
        }

        expectToken(TOKEN_L_PAR, stack);
        parameters = params(stack);
        expectToken(TOKEN_R_PAR, stack);

        returntype = rt(stack);
        expectToken(TOKEN_L_BRACKET, stack);

        if (parameters != NULL || returntype != NULL)
        {
            params_returntype = createNode(NODE_PARAMS_RETURNTYPE, NULL, parameters, returntype);
        }

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
        fprintf(stderr, "Syntax error, ':' expected!");
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
            fprintf(stderr, "Syntax error!");
            return NULL;
    }
}


ASTstruct *params(Stack *stack)
{
    ASTstruct *param = NULL;
    token = loadToken(stack);

    if (token->type == TOKEN_ID)
    {
        DynamicString *value = token->value.stringPtr;
        token = loadToken(stack);
        switch (token->type)
        {
            case TOKEN_KEYWORD_INT:
                param = createNode(NODE_PARAM_ID_INT, value, NULL, NULL);
                break;

            case TOKEN_KEYWORD_FLOAT:
                param = createNode(NODE_PARAM_ID_FLOAT, value, NULL, NULL);
                break;

            case TOKEN_KEYWORD_STRING:
                param = createNode(NODE_PARAM_ID_STRING, value, NULL, NULL);
                break;

            default:
                fprintf(stderr, "Syntax error!");
                return NULL;
        }
    }
    else
    {
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

    ASTstruct *root = NULL;


    if (StackIsEmpty(stack))
        return root;


    token = loadToken(stack);

    switch(token->type)
    {
        case TOKEN_KEYWORD_RETURN:
            token = loadToken(stack);
            root = createNode(SEQ, NULL, NULL, createNode(NODE_RETURN, NULL, NULL, NULL));
            unloadToken(stack);
        break;

        default:
            unloadToken(stack);
            return NULL;
    }

    return root;
}