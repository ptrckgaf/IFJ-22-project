/*
    - Implementácia generátoru kódu v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/

#include "utils.h"
#include "codegen.h"


int runtime_err = 0;
int value = 0;
char *func;
int if_s, while_s;

//ASTstruct *ast;
//FSTable *ftab;

int codegen()
{
    PRINT_CODE(".IFJcode22\n\n");
    PRINT_CODE("JUMP $$main\n");

    gen_func_def(ast->rightNode);

    PRINT_CODE("LABEL $$main\n");
    PRINT_CODE("CREATEFRAME\n");
    PRINT_CODE("PUSHFRAME\n");
    gen_statements(ast->rightNode);
//    gen_func_def(ast->rightNode);
//    gen_statements(ast->rightNode);

    return 0;
}

void gen_statements(ASTstruct *tree){
    //generating main body of the program
    int ifc, whilec;
    int type;

    switch (tree->rightNode->type) {
        case NODE_FUNC_DEF:

            break;

        case NODE_VAR_ID:
            break;

        case NODE_IF:
            ifc = if_s;
            if_s++;
            // podmienka if
            gen_cond(ast->rightNode->leftNode->rightNode, ifc, 0);

            if (ast->rightNode->rightNode->rightNode)
            {
                gen_statements(ast->rightNode->rightNode->rightNode);
            }
            PRINT_CODE("JUMP %s%dtrue\n", func, ifc);
            PRINT_CODE("LABEL %s%dfalse", func, ifc);
            PRINT_NL();

            if (ast->rightNode->rightNode->leftNode)
            {
                gen_statements(ast->rightNode->rightNode->leftNode);
            }
            PRINT_CODE("LABEL %s%dtrue", func, ifc);
            PRINT_NL();
            break;

        case NODE_WHILE:
            break;

        case NODE_VAR_ASSIGNMENT:{
            calculate_expr(tree->rightNode->rightNode);
            ASTstruct *variable_id = tree->rightNode->leftNode;
            /*
             *todo check if it is first occurance of variable(is_defined flag), if not print DEFVAR LF@VAR_ID
             * add flag is_defined to ST item
             * remove line below
             */
            PRINT_CODE("DEFVAR LF@%s\n", variable_id->value->data.stringPtr->value);
            PRINT_CODE("POPS LF@%s\n", variable_id->value->data.stringPtr->value);
            break;
        }
        case NODE_READS:
            break;
        case NODE_READI:
            break;
        case NODE_READF:
            break;
        case NODE_WRITE:
            gen_write(tree->rightNode);
            generate_constant(tree);
            break;
        case NODE_STRLEN:
            break;
        case NODE_SUBSTRING:
            break;
        case NODE_CHR:
            break;
        case NODE_ORD:
            break;
        case NODE_FUNC_ID:
            break;
        case NODE_PROLOG:
            break;

    }
    if (tree->leftNode){
        gen_statements(tree->leftNode);
    }
}

void gen_func_def(ASTstruct *tree)
{
    if (tree->rightNode->type == NODE_FUNC_DEF)
    {
        fprintf(stdout, "\nLABEL %s\n", tree->rightNode->value->data.stringPtr->value);
        fprintf(stdout, "PUSHFRAME\n");

        if (tree->rightNode->leftNode)
        {
            gen_func_params(tree->rightNode->leftNode->leftNode);
        }
        if (tree->rightNode->rightNode)
        {
            gen_statements(tree->rightNode->rightNode);
        }
        fprintf(stdout, "POPFRAME\n");
        fprintf(stdout, "RETURN\n");
        fprintf(stdout, "\n");
    }
    if (!tree->leftNode)
    {
        return;
    }
    gen_func_def(tree->leftNode);

}

void gen_func_params(ASTstruct *tree)
{
    int param_number = 1;
    fprintf(stdout, "DEFVAR LF@%s\n", tree->rightNode->value->data.stringPtr->value);
    fprintf(stdout, "MOVE LF@%s LF@%d\n", tree->rightNode->value->data.stringPtr->value, param_number);

    if (!tree->leftNode)
    {
        return;
    }
    param_number++;
    gen_func_params(tree->leftNode);
}

void gen_write(ASTstruct *tree){
    tree = tree->leftNode;
    while (tree){
        PRINT_CODE("WRITE");
        //todo change to generate expr
        generate_constant(tree->rightNode);
        PRINT_NL();
        tree = tree->leftNode;
    }
}

void generate_constant(ASTstruct *node){

    switch (node->type){
        case NODE_INT:
            PRINT_CODE(" int@%d", node->value->data.integer);
            break;

        case NODE_STRING:
            PRINT_CODE(" string@%s", node->value->data.stringPtr->value);
            break;

        case NODE_FLOAT:
            PRINT_CODE(" float@%a", node->value->data.decimal);
            break;

        case NODE_NULL:
            PRINT_CODE(" nil@nil");
            break;
        case NODE_VAR_ID:
            //todo rozsah platnosti

            PRINT_CODE(" LF@");
        default:
            break;
    }
}

Value *calculate_plus(Value *left, Value *right){

}

Value *calculate_minus(Value *left, Value *right){

}

Value *calculate_division(Value *left, Value *right){

}
void calculate_expr(ASTstruct *tree)
{
    switch (tree->type)
    {
        case NODE_PLUS:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("ADDS\n");
            break;

        case NODE_MINUS:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("SUBS\n");
            break;

        case NODE_MUL:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("MULS\n");
            break;

        case NODE_DIV:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("DIVS\n");
            break;

        case NODE_INT:
        case NODE_FLOAT:
            PRINT_CODE("PUSHS");
            generate_constant(tree);
            PRINT_NL();
    }
}


void gen_assignment(ASTstruct *ast){

}




void gen_cond(ASTstruct *ast, int count, int type)
{
    PRINT_CODE("MOVE LF@!condvar1");
    generate_constant(ast->leftNode->type);
    PRINT_NL();
    gen_expr(ast->leftNode);
    PRINT_CODE("POPS LF@!condvar1\n");

    PRINT_CODE("MOVE LF@!condvar2");
    generate_constant(ast->rightNode->type);
    PRINT_NL();
    gen_expr(ast->rightNode);
    PRINT_CODE("POPS LF@!condvar2\n");

    switch(ast->type)
    {
        case NODE_GREATER:
            PRINT_CODE("GT LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFNEQ %s%dfalse LF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFNEQ %d%sfalse LF@!compvar bool@true\n", count, func);
            }
            break;

        case NODE_LESS:
            PRINT_CODE("LT LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFNEQ %s%dfalse LF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFNEQ %d%sfalse LF@!compvar bool@true\n", count, func);
            }
            break;

        case NODE_COMPARE:
            PRINT_CODE("EQ LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFNEQ %s%dfalse LF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFNEQ %d%sfalse LF@!compvar bool@true\n", count, func);
            }
            break;

        case NODE_NEG_COMPARE:
            PRINT_CODE("EQ LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFEQ %s%dfalse LF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFEQ %d%sfalse LF@!compvar bool@true\n", count, func);
            }
            break;

        case NODE_GREATER_EQ:
            PRINT_CODE("GT LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFEQ %s%dfalse LF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFEQ %d%sfalse LF@!compvar bool@true\n", count, func);
            }
            PRINT_CODE("EQ LF@!compvar LF@!condvar1 LF@!condvar2\n");
            if (type == 0)
            {
                PRINT_CODE("JUMPIFNEQ %s%dfalse GF@!compvar bool@true\n", func, count);
            }
            else
            {
                PRINT_CODE("JUMPIFNEQ %d%sfalse GF@!compvar bool@true\n", count, func);
            }
            if (type == 0)
            {
                PRINT_CODE("LABEL %s%dfalse\n", func, count);
            }
            else
            {
                PRINT_CODE("LABEL %d%sfalse\n", count, func);
            }
            break;

        default:
            break;
    }

}

void gen_expr(ASTstruct *ast)
{
    switch(ast->type)
    {
        case NODE_INT:
        case NODE_STRING:
        case NODE_FLOAT:
        case NODE_VAR_ID:
            generate_constant(ast->type);
            PRINT_NL();
            break;

        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
            calculate_expr(ast);
            break;
    }
}



//void gen_statements(ASTstruct *ast)
//{
//    switch(ast->rightNode->type)
//    {
//        case NODE_VAR_ASSIGNMENT:
//            gen_var_assignment(ast->rightNode);
//            break;
//
//        case NODE_FUNC_ID:
//            gen_func_call(ast->rightNode);
//            break;
//
//        default:
//            break;
//    }
//    if (!ast->leftNode)
//    {
//        return;
//    }
//    gen_statements(ast->leftNode);
//}
//
//void gen_func_def(ASTstruct *ast)
//{
//    if (ast->rightNode->type == NODE_FUNC_DEF)
//    {
//        fprintf(stdout, "\nLABEL %s\n", ast->value->data.stringPtr->value);
//        fprintf(stdout, "PUSHFRAME\n");
//
//        if (ast->leftNode)
//        {
//            gen_func_params(ast->leftNode->leftNode);
//        }
//        if (ast->rightNode)
//        {
//            gen_func_body(ast->rightNode);
//        }
//        fprintf(stdout, "POPFRAME\n");
//        fprintf(stdout, "RETURN\n");
//        fprintf(stdout, "\n");
//    }
//    if (!ast->leftNode)
//    {
//        return;
//    }
//    gen_func_def(ast->leftNode);
//
//}
//
//void gen_var_assignment(ASTstruct *ast)
//{
//    fprintf(stdout, "DEFVAR LF@%s\n", ast->leftNode->value->data.stringPtr->value);
//
//    switch(ast->rightNode->type)
//    {
//        case NODE_STRING:
//            fprintf(stdout, "MOVE LF@%s", ast->leftNode->value->data.stringPtr->value);
//            fprintf(stdout, "string@%s", ast->leftNode->value->data.stringPtr->value);
//            fprintf(stdout, "\n");
//            break;
//
//    }
//}
//
//void gen_func_call(ASTstruct *ast)
//{
//    int arg_number = 1;
//    switch (ast->rightNode->type)
//    {
//        case NODE_INT:
//        case NODE_STRING:
//        case NODE_FLOAT:
//            break;
//
//        case NODE_VAR_ID:
//            fprintf(stdout, "DEFVAR TF@%d\n", arg_number);
//            fprintf(stdout, "MOVE TF@%d LF@%s\n", arg_number, ast->rightNode->value->data.stringPtr->value);
//            break;
//
//        default:
//            break;
//    }
//    if (!ast->leftNode)
//    {
//        return;
//    }
//    arg_number++;
//    gen_func_call(ast->leftNode);
//}
//
//void gen_func_params(ASTstruct *ast)
//{
//    int param_number = 1;
//    fprintf(stdout, "DEFVAR LF@%s\n", ast->rightNode->value->data.stringPtr->value);
//    fprintf(stdout, "MOVE LF@%s LF@%d\n", ast->rightNode->value->data.stringPtr->value, param_number);
//
//    if (!ast->leftNode)
//    {
//        return;
//    }
//    param_number++;
//    gen_func_params(ast->leftNode);
//}
//
//void gen_func_body(ASTstruct *ast)
//{
//    switch(ast->rightNode->type)
//    {
//        case NODE_WRITE:
//            if (ast->rightNode->leftNode)
//            {
//                gen_write(ast->rightNode->leftNode);
//            }
//            break;
//    }
//}
//
//void gen_write(ASTstruct *ast)
//{
//    switch(ast->rightNode->type)
//    {
//        case NODE_VAR_ID:
//            fprintf(stdout, "WRITE ");
//            fprintf(stdout, "string@%s", ast->rightNode->value->data.stringPtr->value);
//            fprintf(stdout, "\n");
//            break;
//
//        default:
//            break;
//    }
//    if (!ast->leftNode)
//    {
//        return;
//    }
//    gen_write(ast->leftNode);
//}