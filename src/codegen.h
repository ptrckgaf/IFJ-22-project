
#include "utils.h"
#include "symtable.h"

#define PRINT_CODE(M, ...)               \
    fprintf(stdout, "" M "", ##__VA_ARGS__)

#define PRINT_NL() \
    fprintf(stdout, "\n")

#define PRINT_ERR() \
    PRINT_CODE("JUMP $$runerr\n");



void gen_statements(ASTstruct *tree, tKey func_name, int if_number);
void gen_func_def(ASTstruct *tree, tKey func_name, int if_number);
void gen_functions(ASTstruct *tree);
void gen_var_assignment(ASTstruct *ast);
void gen_func_call(ASTstruct *ast);
void gen_void_func_call(ASTstruct *tree);
void gen_func_params(ASTstruct *tree, int param_id, tKey func_name);
void gen_func_body (ASTstruct *tree);
void gen_write(ASTstruct *ast);
void calculate_expr(ASTstruct *tree);
void gen_declaration(ASTstruct *tree);
void gen_if(ASTstruct *tree, tKey func_name, int if_number);
void gen_write(ASTstruct *tree);
void generate_constant(ASTstruct *ast);
void gen_cond(ASTstruct *ast, int count, int type);
void gen_expr(ASTstruct *ast);