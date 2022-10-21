/*
    - Implementacia syntaktickej analýzy v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/


#include "utils.h"

Token *token;
ASTstruct *ast;


// funkcia vykona syntakticku analyzu tokenov a vytvori AST

int parser()
{
    ast = parse();


  return 0;
}

// zaciatok syntaktickej analyzy

ASTstruct *parse()
{
    ASTstruct *root = NULL;
    
    do
    {
        root = createNode();
    } while (!stackEmpty(&stack));

    return root;
}


ASTstruct *prolog()
{
    ASTstruct *root = NULL;

    // zobere token z vrchola zasobniku
    token = getToken(&stack);

    if (token == NULL)
    {
        fprintf(stderr, "Syntax error! Prolog missing.");
    }

    if (token->type == PROLOG)
    {
        root = createNode();
    }
    else
    {
        fprintf(stderr, "Syntax error! Prolog missing.");
    }


    return root;
}


ASTstruct *program()
{
    char *func_name = NULL;
    ASTstruct *parameters = NULL;
    ASTstruct *returntype = NULL;
    ASTstruct *func = NULL;
    ASTstruct *root = NULL;

    token = getToken(&stack);
    if (token == NULL) return NULL;

    if (token->type == KEYWORD_FUNCTION)
    {
        token = getToken(&stack);

        if (token->type == TOKEN_IDENTIF)
        {
            func_name = my_strdup(token->value);
        }
        else
        {
            fprintf(stderr, "Syntax error!");
        }

        expectToken(L_BRACKET);
        parameters = params();
        expectToken(R_BRACKET);

        returntype = rt();
        expectToken(L_CBRACKET);

        func = createNode(); // statement
        func->value = func_name;
        expectToken(R_CBRACKET);

        root = createNode();

    }

    return root;
}


ASTstruct *rt()
{
    ASTstruct *returntype = NULL;
    ASTstruct *returntypeNode = NULL;
    token = getToken(&stack);

    if (token->type == COLON)
    {
        returntype = getRT();
        if (returntype != NULL)
        {
            returntypeNode = createNode(SEQ, , returntype);
        }
        
        return returntypeNode;
    }
    else
    {
        fprintf(stderr, "Syntax error!");
    }

}


ASTstruct *getRT()
{
    token = getToken(&stack);

    switch(token->type)
    {
        case INT:
            return createNode(RETURN_TYPE_INT, NULL);
            break;

        case FLOAT:
            return createNode(RETURN_TYPE_FLOAT, NULL);
            break;

        case STRING:
            return createNode(RETURN_TYPE_STR, NULL);
            break;

        default:
            fprintf(stderr, "Syntax error!");
    }
}


ASTstruct *params()
{
    ASTstruct *param = NULL;
    token = getToken(&stack);

    if (token->type == TOKEN_IDENTIF)
    {
        char *value = token->value;
        token = getToken(&stack);
        switch (token->type)
        {
            case INT:
                param = createNode();
                break;

            case FLOAT:
                param = createNode();
                break;

            case STRING:
                param = createNode();
                break;

            default:
                fprintf(stderr, "Syntax error!");
        }
    }
    else
    {
        ungetToken(&stack);
        return NULL;
    }
    token = getToken(&stack);

    if (token->type == TOKEN_COMMA)
    {
        return createNode(SEQ, params(), param);
    }
    else
    {
        ungetToken(&stack);
        return createNode(SEQ, NULL, param);
    }
}

ASTstruct *stmt()
{
    
}