/*
    - Code generator implementation
    - Autor: Anton Miklis(xmikli05)
*/
#include "utils.h"
#include "symtable.h"

#define CODE(M, ...)               \
    fprintf(stdout, "" M "", ##__VA_ARGS__)

#define PRINT_NL() \
    fprintf(stdout, "\n")




void gen_statements(ASTstruct *tree, tKey func_name, int if_number, int while_number);
void gen_func_def(ASTstruct *tree, tKey func_name, int if_number, int while_number);
void gen_func_call(ASTstruct *ast);
void gen_void_func_call(ASTstruct *tree);
void gen_func_params(ASTstruct *tree, int param_id, tKey func_name);
void gen_write(ASTstruct *ast);
void calculate_expr(ASTstruct *tree);
void gen_if(ASTstruct *tree, tKey func_name, int if_number, int while_number);
void gen_write(ASTstruct *tree);
void gen_constant(ASTstruct *node);
void expect_type(char *type);
void gen_while(ASTstruct *tree, tKey func_name, int if_number, int while_number);