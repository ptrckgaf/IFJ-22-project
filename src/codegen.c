/*
    - Implementácia generátoru kódu v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/

#include "utils.h"
#include "codegen.h"


int runtime_err = 0;
int value = 0;
ASTstruct *ast;
FSTable *ftab;

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

    switch (tree->rightNode->type) {
        case NODE_FUNC_DEF:

            break;

        case NODE_VAR_ID:
            break;

        case NODE_IF:
            break;

        case NODE_WHILE:
            break;

        case NODE_VAR_ASSIGNMENT:{
            /*
            *todo check if it is first occurance of variable(is_defined flag), if not print DEFVAR LF@VAR_ID
            * add flag is_defined to ST item
            * remove line below
            */
            ASTstruct *variable_id = tree->rightNode->leftNode;
            PRINT_CODE("DEFVAR LF@%s\n", variable_id->value->data.stringPtr->value);
            if (tree->rightNode->rightNode->type == NODE_READS){
                PRINT_CODE("READ LF@%s string", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->leftNode->type == NODE_READF) {
                PRINT_CODE("READ LF@%s flaot", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->leftNode->type == NODE_READI){
                PRINT_CODE("READ LF@%s int", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else{
                calculate_expr(tree->rightNode->rightNode);
                PRINT_CODE("POPS LF@%s\n", variable_id->value->data.stringPtr->value);
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
            gen_func_body(tree->rightNode->rightNode);
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

void gen_func_body(ASTstruct *tree)
{
    gen_statements(tree);
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

            PRINT_CODE(" LF@%s", node->value->data.stringPtr->value);
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
        case NODE_VAR_ID:
            PRINT_CODE("PUSHS");
            generate_constant(tree);
            PRINT_NL();
    }
}


void gen_assignment(ASTstruct *ast){

}

