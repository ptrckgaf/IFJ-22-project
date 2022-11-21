/*
    - Implementácia generátoru kódu v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/

#include "utils.h"
#include "codegen.h"



int codegen()
{
    fprintf(stdout, ".IFJcode22\n\n");


    gen_func_def(ast->rightNode);
    gen_statements(ast->rightNode);

    return 0;
}


void gen_statements(ASTstruct *ast)
{
    switch(ast->rightNode->type)
    {
        case NODE_VAR_ASSIGNMENT:
            gen_var_assignment(ast->rightNode);
            break;

        case NODE_FUNC_ID:
            gen_func_call(ast->rightNode);
            break;

        default:
            break;
    }
    if (!ast->leftNode)
    {
        return;
    }
    gen_statements(ast->leftNode);
}

void gen_func_def(ASTstruct *ast)
{
    if (ast->rightNode->type == NODE_FUNC_DEF)
    {
        fprintf(stdout, "\nLABEL %s\n", ast->value->data.stringPtr->value);
        fprintf(stdout, "PUSHFRAME\n");

        if (ast->leftNode)
        {
            gen_func_params(ast->leftNode->leftNode);
        }
        if (ast->rightNode)
        {
            gen_func_body(ast->rightNode);
        }
        fprintf(stdout, "POPFRAME\n");
        fprintf(stdout, "RETURN\n");
        fprintf(stdout, "\n");
    }
    if (!ast->leftNode)
    {
        return;
    }
    gen_func_def(ast->leftNode);

}

void gen_var_assignment(ASTstruct *ast)
{
    fprintf(stdout, "DEFVAR LF@%s\n", ast->leftNode->value->data.stringPtr->value);

    switch(ast->rightNode->type)
    {
        case NODE_STRING:
            fprintf(stdout, "MOVE LF@%s", ast->leftNode->value->data.stringPtr->value);
            fprintf(stdout, "string@%s", ast->leftNode->value->data.stringPtr->value);
            fprintf(stdout, "\n");
            break;

    }
}

void gen_func_call(ASTstruct *ast)
{
    int arg_number = 1;
    switch (ast->rightNode->type)
    {
        case NODE_INT:
        case NODE_STRING:
        case NODE_FLOAT:
            break;

        case NODE_VAR_ID:
            fprintf(stdout, "DEFVAR TF@%d\n", arg_number);
            fprintf(stdout, "MOVE TF@%d LF@%s\n", arg_number, ast->rightNode->value->data.stringPtr->value);
            break;

        default:
            break;
    }
    if (!ast->leftNode)
    {
        return;
    }
    arg_number++;
    gen_func_call(ast->leftNode);
}

void gen_func_params(ASTstruct *ast)
{
    int param_number = 1;
    fprintf(stdout, "DEFVAR LF@%s\n", ast->rightNode->value->data.stringPtr->value);
    fprintf(stdout, "MOVE LF@%s LF@%d\n", ast->rightNode->value->data.stringPtr->value, param_number);

    if (!ast->leftNode)
    {
        return;
    }
    param_number++;
    gen_func_params(ast->leftNode);
}

void gen_func_body(ASTstruct *ast)
{
    switch(ast->rightNode->type)
    {
        case NODE_WRITE:
            if (ast->rightNode->leftNode)
            {
                gen_write(ast->rightNode->leftNode);
            }
            break;
    }
}

void gen_write(ASTstruct *ast)
{
    switch(ast->rightNode->type)
    {
        case NODE_VAR_ID:
            fprintf(stdout, "WRITE ");
            fprintf(stdout, "string@%s", ast->rightNode->value->data.stringPtr->value);
            fprintf(stdout, "\n");
            break;

        default:
            break;
    }
    if (!ast->leftNode)
    {
        return;
    }
    gen_write(ast->leftNode);
}