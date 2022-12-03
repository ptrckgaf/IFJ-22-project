
#include "utils.h"

#define PRINT_CODE(M, ...)               \
    fprintf(stdout, "" M "", ##__VA_ARGS__)

#define PRINT_NL() \
    fprintf(stdout, "\n")

void generate(ASTstruct *tree);
void gen_statements(ASTstruct *ast);
void gen_func_def(ASTstruct *ast);
void gen_var_assignment(ASTstruct *ast);
void gen_func_call(ASTstruct *ast);
void gen_func_params(ASTstruct *ast);
void gen_func_body (ASTstruct *ast);
void gen_write(ASTstruct *ast);
void calculate_expr(ASTstruct *tree);
void gen_declaration(ASTstruct *tree);
void gen_if(ASTstruct *tree);
void gen_write(ASTstruct *tree);
void generate_constant(ASTstruct *node);