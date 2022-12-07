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

    CODE(".IFJcode22\n");

    //temporary variables
    CODE("DEFVAR GF@%%tmp\n");
    CODE("DEFVAR GF@%%type\n");
    //variable for string operations
    CODE("DEFVAR GF@%%str1\n");
    CODE("DEFVAR GF@%%str2\n");

    CODE("JUMP $$main\n");

    //runtime error
    CODE("LABEL $$runerr\n");
    CODE("EXIT int@7\n");

    gen_func_def(ast->rightNode, NULL, 0, 0);

    CODE("LABEL $$main\n");
    CODE("CREATEFRAME\n");
    CODE("PUSHFRAME\n");
    gen_statements(ast->rightNode, "0", 0, 0);
//    gen_func_def(ast->rightNode);
//    gen_statements(ast->rightNode);

    return 0;
}

void gen_statements(ASTstruct *tree, tKey func_name, int if_number, int while_number) {
    //generating main body of the program
    int ifc, whilec = 0;
    int type;
    if (!tree){
        return;
    }
    switch (tree->rightNode->type) {
        case NODE_IF:
            gen_if(tree->rightNode, func_name, if_number, while_number);
            break;

        case NODE_WHILE:
            gen_while(tree->rightNode, func_name, if_number, while_number);
            break;

        case NODE_VAR_ASSIGNMENT:{
            tKey variable_id = tree->rightNode->leftNode->value->data.stringPtr->value;
            st_item *stItem = getStItem(func_name, variable_id, fsTable);
            //define variable if wasn't defined before
            if (!stItem->is_defined){
                CODE("DEFVAR LF@%s\n", variable_id);
                stItem->is_defined = 1;
            }
            if (tree->rightNode->rightNode->type == NODE_READS){
                //built-in reads call
                CODE("READ LF@%s string\n", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->rightNode->type == NODE_READF) {
                //built-in readf call
                CODE("READ LF@%s float\n", tree->rightNode->leftNode->value->data.stringPtr->value);
            } else if (tree->rightNode->rightNode->type == NODE_READI){
                //built-in readi call
//                PRINT_CODE("DEFVAR LF@%%type\n");
                CODE("READ LF@%s int\n", tree->rightNode->leftNode->value->data.stringPtr->value);
//                PRINT_CODE("TYPE LF@%%type LF@%s\n", tree->rightNode->leftNode->value->data.stringPtr->value);
//                PRINT_CODE("DPRINT LF@%%type\n");
            } else if (tree->rightNode->rightNode->type == NODE_FUNC_ID){
                //user defined function call
                gen_func_call(tree->rightNode);
            } else if(tree->rightNode->rightNode->type == NODE_STRLEN){
                //built-in strlen
                CODE("STRLEN LF@%s", tree->rightNode->leftNode->value->data.stringPtr->value);
                generate_constant(tree->rightNode->rightNode->leftNode->rightNode);
            }
            else{
                calculate_expr(tree->rightNode->rightNode);
                CODE("POPS LF@%s\n", variable_id);
            }
            break;
        }
        case NODE_WRITE:
            gen_write(tree->rightNode);
            generate_constant(tree);
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
            CODE("MOVE LF@%%retval");
            generate_constant(tree->rightNode->leftNode);
            PRINT_NL();

            CODE("POPFRAME\n");
            CODE("RETURN\n");
            PRINT_NL();
            break;
        case NODE_PROLOG:
            break;

    }
    if (tree->leftNode){
        gen_statements(tree->leftNode, func_name, if_number, while_number);
    }
}

void gen_func_def(ASTstruct *tree, tKey func_name, int if_number, int while_number)
{
    if (tree->rightNode->type == NODE_FUNC_DEF)
    {
        tKey func_name = tree->rightNode->value->data.stringPtr->value;
        fprintf(stdout, "\nLABEL %s\n", func_name);
        fprintf(stdout, "PUSHFRAME\n");
        CODE("DEFVAR LF@%%retval\n");
        CODE("MOVE LF@%%retval nil@nil\n");

        if (tree->rightNode->leftNode)
        {
            gen_func_params(tree->rightNode->leftNode->leftNode, 1, func_name);
        }
        if (tree->rightNode->rightNode)
        {
            gen_statements(tree->rightNode->rightNode, func_name, if_number, while_number);
        }
    }
    if (!tree->leftNode)
    {
        return;
    }
    gen_func_def(tree->leftNode, NULL, if_number, while_number);

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
    CODE("DEFVAR LF@%s\n", variable_id);

    fprintf(stdout, "MOVE LF@%s LF@%%%d\n", tree->rightNode->value->data.stringPtr->value, param_id);

    if (!tree->leftNode)
    {
        return;
    }
    gen_func_params(tree->leftNode, ++param_id, func_name);
}

void gen_func_call_params(ASTstruct *tree, int param_id){
    CODE("DEFVAR TF@%%%d\n", param_id);
    CODE("MOVE TF@%%%d", param_id);
    generate_constant(tree->rightNode);
    PRINT_NL();

    if (!tree->leftNode)
    {
        return;
    }
    gen_func_call_params(tree->leftNode, ++param_id);
}

void gen_func_call(ASTstruct *tree){
    CODE("CREATEFRAME\n");
    if (tree->rightNode->leftNode){
        gen_func_call_params(tree->rightNode->leftNode, 1);
    }

    CODE("CALL %s\n", tree->rightNode->value->data.stringPtr->value);
    CODE("MOVE LF@%s TF@%%retval\n", tree->leftNode->value->data.stringPtr->value);
}

void gen_void_func_call(ASTstruct *tree){
    CODE("CREATEFRAME\n");
    if (tree->leftNode){
        gen_func_call_params(tree->leftNode, 1);
    }

    CODE("CALL %s\n", tree->value->data.stringPtr->value);
}

void gen_write(ASTstruct *tree){
    tree = tree->leftNode;
    while (tree){
        CODE("WRITE");
        generate_constant(tree->rightNode);
        PRINT_NL();
        tree = tree->leftNode;
    }
}

void generate_constant(ASTstruct *node){

    switch (node->type){
        case NODE_INT:
            CODE(" int@%d", node->value->data.integer);
            break;

        case NODE_STRING:
            CODE(" string@");
            processString(node->value->data.stringPtr);
            break;

        case NODE_FLOAT:
            CODE(" float@%a", node->value->data.decimal);
            break;

        case NODE_NULL:
            CODE(" nil@nil");
            break;

        case NODE_VAR_ID:
            CODE(" LF@%s", node->value->data.stringPtr->value);
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
            CODE("ADDS\n");
            break;

        case NODE_MINUS:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("SUBS\n");
            break;

        case NODE_MUL:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("MULS\n");
            break;

        case NODE_DIV:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("DIVS\n");

            break;
        case NODE_LESS:
            //todo >= <=
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("LTS\n");
            break;
        case NODE_LESS_EQ:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("LTS\n");
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("EQS\n");
            CODE("ORS\n");
            break;
        case NODE_GREATER_EQ:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("GTS\n");
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("EQS\n");
            CODE("ORS\n");
            break;

        case NODE_GREATER:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("GTS\n");
            break;
        case NODE_COMPARE:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("EQS\n");
            break;
        case NODE_NEG_COMPARE:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            CODE("EQS\n");
            CODE("NOTS\n");
            break;
        case NODE_CONCATENATE:
            calculate_expr(tree->leftNode);
            calculate_expr(tree->rightNode);
            expect_type("string");
            CODE("POPS GF@%%str2\n");
            expect_type("string");
            CODE("POPS GF@%%str1\n");
            CODE("CONCAT GF@%%tmp GF@%%str1 GF@%%str2\n");
            CODE("PUSHS GF@%%tmp\n");
            break;
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_VAR_ID:
        case NODE_NULL:
        case NODE_STRING:
            CODE("PUSHS");
            generate_constant(tree);
            PRINT_NL();
    }
}

void gen_if(ASTstruct *tree, tKey func_name, int if_number, int while_number) {
    if_number++;
    calculate_expr(tree->leftNode->rightNode);
    CODE("PUSHS bool@false\n");

    CODE("JUMPIFEQS $$else$%d$%s\n", if_number, func_name);

    //if body
    gen_statements(tree->rightNode->rightNode, func_name, if_number, while_number);
    CODE("JUMP $$elseend$%d$%s\n", if_number, func_name);
    CODE("LABEL $$else$%d$%s\n", if_number, func_name);
    //else body
    gen_statements(tree->rightNode->leftNode, func_name, if_number, while_number);
    CODE("LABEL $$elseend$%d$%s\n", if_number, func_name);
}

void gen_while(ASTstruct *tree, tKey func_name, int if_number, int while_number) {
    PRINT_NL();
    while_number++;

    CODE("JUMP $$loop$%d$%s\n", while_number, func_name);

    CODE("LABEL $$cloop$%d$%s\n", while_number, func_name);
    gen_statements(tree->rightNode, func_name, if_number, while_number);
    CODE("LABEL $$loop$%d$%s\n", while_number, func_name);
    calculate_expr(tree->leftNode->rightNode);
    CODE("PUSHS bool@true\n");

    CODE("JUMPIFEQS $$cloop$%d$%s\n", while_number, func_name);
}


void expect_type(char *type){
    CODE("POPS GF@%%tmp\n");
    CODE("TYPE GF@%%type GF@%%tmp\n");
    CODE("JUMPIFNEQ $$runerr GF@%%type string@%s\n", type);
    CODE("PUSHS GF@%%tmp\n");
}
