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
bool in_while = false;

bool neg = 0;

ASTstruct *ast;
FSTable *fsTable;

int codegen()
{
    PRINT_CODE(".IFJcode22\n\n");
    PRINT_CODE("JUMP $$main\n");

    gen_func_def(ast->rightNode, NULL);

    PRINT_CODE("LABEL $$main\n");
    PRINT_CODE("CREATEFRAME\n");
    PRINT_CODE("PUSHFRAME\n");
    gen_statements(ast->rightNode, "0");
//    gen_func_def(ast->rightNode);
//    gen_statements(ast->rightNode);

    return 0;
}

void gen_statements(ASTstruct *tree, tKey func_name) {
    //generating main body of the program
    int ifc, whilec = 0;
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
            gen_cond(tree->rightNode->leftNode->rightNode, ifc, 0);

            if (tree->rightNode->rightNode->rightNode)
            {
                gen_statements(tree->rightNode->rightNode->rightNode, func_name);
            }
            PRINT_CODE("JUMP %s%dtrue\n", func, ifc);
            PRINT_CODE("LABEL %s%dfalse", func, ifc);
            PRINT_NL();

            if (tree->rightNode->rightNode->leftNode)
            {
                gen_statements(tree->rightNode->rightNode->leftNode, func_name);
            }
            PRINT_CODE("LABEL %s%dtrue", func, ifc);
            PRINT_NL();
            break;

        case NODE_WHILE:
            PRINT_CODE("LABEL %d%strue\n", whilec, func);
            // podmienka
            gen_cond(tree->rightNode->leftNode, whilec, 1);
            in_while = true;
            // telo
            gen_statements(tree->rightNode->rightNode, func_name);
            in_while = false;

            PRINT_CODE("JUMP %d%strue\n", whilec, func);
            PRINT_CODE("LABEL %d%sfalse\n", whilec, func);
            break;

        case NODE_VAR_ASSIGNMENT:{
            tKey variable_id = tree->rightNode->leftNode->value->data.stringPtr->value;
            st_item *stItem = getStItem(func_name, variable_id, fsTable);
            //define variable if wasn't defined before
            if (!stItem->is_defined){
                PRINT_CODE("DEFVAR LF@%s\n", variable_id);
                stItem->is_defined = 1;
            }
            if (tree->rightNode->rightNode->type == NODE_READS){
                //built-in reads call
                PRINT_CODE("READ LF@%s string", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->leftNode->type == NODE_READF) {
                //built-in readf call
                PRINT_CODE("READ LF@%s flaot", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->leftNode->type == NODE_READI){
                //built-in readi call
                PRINT_CODE("READ LF@%s int", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->rightNode->type == NODE_FUNC_ID){
                //user defined function call
                gen_func_call(tree->rightNode);
            }
            else{
                calculate_expr(tree->rightNode->rightNode);
                PRINT_CODE("POPS LF@%s\n", variable_id);
            }
            break;
        }
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
            gen_void_func_call(tree->rightNode);
            break;
        case NODE_RETURN:
            PRINT_CODE("MOVE LF@%%retval");
            generate_constant(tree->rightNode->leftNode);
            PRINT_NL();

            PRINT_CODE("POPFRAME\n");
            PRINT_CODE("RETURN\n");
            PRINT_NL();
            break;
        case NODE_PROLOG:
            break;

    }
    if (tree->leftNode){
        gen_statements(tree->leftNode, func_name);
    }
}

void gen_func_def(ASTstruct *tree, tKey func_name)
{
    if (tree->rightNode->type == NODE_FUNC_DEF)
    {
        tKey func_name = tree->rightNode->value->data.stringPtr->value;
        fprintf(stdout, "\nLABEL %s\n", func_name);
        fprintf(stdout, "PUSHFRAME\n");
        PRINT_CODE("DEFVAR LF@%%retval\n");
        PRINT_CODE("MOVE LF@%%retval nil@nil\n");

        if (tree->rightNode->leftNode)
        {
            gen_func_params(tree->rightNode->leftNode->leftNode, 1, func_name);
        }
        if (tree->rightNode->rightNode)
        {
            gen_statements(tree->rightNode->rightNode, func_name);
        }
    }
    if (!tree->leftNode)
    {
        return;
    }
    gen_func_def(tree->leftNode, NULL);

}

void gen_func_params(ASTstruct *tree, int param_id, tKey func_name)
{;
    //if no parametres given, return from function
    if (!tree){
        return;
    }
    //define variable and set is_defined flag
    tKey variable_id = tree->rightNode->value->data.stringPtr->value;
    st_item *stItem = getStItem(func_name, variable_id, fsTable);
    if (stItem){
        stItem->is_defined = 1;
    }
    PRINT_CODE("DEFVAR LF@%s\n", variable_id);

    fprintf(stdout, "MOVE LF@%s LF@%%%d\n", tree->rightNode->value->data.stringPtr->value, param_id);

    if (!tree->leftNode)
    {
        return;
    }
    gen_func_params(tree->leftNode, ++param_id, func_name);
}

void gen_func_call_params(ASTstruct *tree, int param_id){
    PRINT_CODE("DEFVAR TF@%%%d\n", param_id);
    PRINT_CODE("MOVE TF@%%%d", param_id);
    generate_constant(tree->rightNode);
    PRINT_NL();

    if (!tree->leftNode)
    {
        return;
    }
    gen_func_call_params(tree->leftNode, ++param_id);
}

void gen_func_call(ASTstruct *tree){
    PRINT_CODE("CREATEFRAME\n");
    if (tree->rightNode->leftNode){
        gen_func_call_params(tree->rightNode->leftNode, 1);
    }

    PRINT_CODE("CALL %s\n", tree->rightNode->value->data.stringPtr->value);
    PRINT_CODE("MOVE LF@%s TF@%%retval\n", tree->leftNode->value->data.stringPtr->value);
}

void gen_void_func_call(ASTstruct *tree){
    PRINT_CODE("CREATEFRAME\n");
    if (tree->leftNode){
        gen_func_call_params(tree->leftNode, 1);
    }

    PRINT_CODE("CALL %s\n", tree->value->data.stringPtr->value);
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
            PRINT_CODE(" LF@%s", node->value->data.stringPtr->value);
        default:
            break;
    }
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
            //todo zero division
            //todo type conversion
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("DIVS\n");
            break;
        case NODE_LESS:
            //todo >= <=
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("LTS\n");
            break;
        case NODE_GREATER:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("GTS\n");
            break;
        case NODE_COMPARE:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("EQS\n");
            break;
        case NODE_NEG_COMPARE:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            PRINT_CODE("EQS\n");
            PRINT_CODE("NOTS\n");
            break;
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_VAR_ID:
            PRINT_CODE("PUSHS");
            generate_constant(tree);
            PRINT_NL();
    }
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
