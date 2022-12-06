/*
    - Implementácia generátoru kódu v rámci projektu z IFJ
    - Autor: Patrik Gáfrik (xgafri00)
*/

#include "utils.h"
#include "codegen.h"


int runtime_err = 0;
int value = 0;
char *func;
int input_cnt = 0; //counter for read
bool in_while = false;

bool neg = 0;

FSTable *fsTable;

int codegen()
{

    PRINT_CODE(".IFJcode22\n");

    //variable for string concatenation
    PRINT_CODE("DEFVAR GF@%%con\n");

    PRINT_CODE("JUMP $$main\n");

    //runtime error
    PRINT_CODE("LABEL $$runerr\n");
    PRINT_CODE("EXIT int@7\n");

    gen_func_def(ast->rightNode, NULL, 0);

    PRINT_CODE("LABEL $$main\n");
    PRINT_CODE("CREATEFRAME\n");
    PRINT_CODE("PUSHFRAME\n");
    gen_statements(ast->rightNode, "0", 0);
//    gen_func_def(ast->rightNode);
//    gen_statements(ast->rightNode);

    return 0;
}

void gen_statements(ASTstruct *tree, tKey func_name, int if_number) {
    //generating main body of the program
    int ifc, whilec = 0;
    int type;
    if (!tree){
        return;
    }
    switch (tree->rightNode->type) {
        case NODE_IF:
            gen_if(tree->rightNode, func_name, if_number);
            break;

        case NODE_WHILE:
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
                PRINT_CODE("READ LF@%s string\n", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->rightNode->type == NODE_READF) {
                //built-in readf call
                PRINT_CODE("READ LF@%s float\n", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->rightNode->type == NODE_READI){
                //built-in readi call
//                PRINT_CODE("DEFVAR LF@%%type\n");
                PRINT_CODE("READ LF@%s int\n", tree->rightNode->leftNode->value->data.stringPtr->value);
//                PRINT_CODE("TYPE LF@%%type LF@%s\n", tree->rightNode->leftNode->value->data.stringPtr->value);
//                PRINT_CODE("DPRINT LF@%%type\n");
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
        gen_statements(tree->leftNode, func_name, if_number);
    }
}

void gen_func_def(ASTstruct *tree, tKey func_name, int if_number)
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
            gen_statements(tree->rightNode->rightNode, func_name, if_number);
        }
    }
    if (!tree->leftNode)
    {
        return;
    }
    gen_func_def(tree->leftNode, NULL, if_number);

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
            break;
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
//        case NODE_CONCATENATE:
//            calculate_expr(tree->leftNode);
//            calculate_expr(tree->rightNode);
//            PRINT_CODE("DEFVAR GF%%con\n");
//            PRINT_CODE("POPS GF%%con\n");
//            PRINT_CODE("CONCAT GF%%con\n");
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_VAR_ID:
        case NODE_NULL:
            PRINT_CODE("PUSHS");
            generate_constant(tree);
            PRINT_NL();
    }
}

void gen_if(ASTstruct *tree, tKey func_name, int if_number) {
    if_number++;
    calculate_expr(tree->leftNode->rightNode);
    PRINT_CODE("PUSHS bool@false\n");

    PRINT_CODE("JUMPIFEQS $$else$%d$%s\n", if_number, func_name);

    //if body
    gen_statements(tree->rightNode->rightNode, func_name, if_number);
    PRINT_CODE("JUMP $$elseend$%d$%s\n", if_number, func_name);
    PRINT_CODE("LABEL $$else$%d$%s\n", if_number, func_name);
    //else body
    gen_statements(tree->rightNode->leftNode, func_name, if_number);
    PRINT_CODE("LABEL $$elseend$%d$%s\n", if_number, func_name);
}
