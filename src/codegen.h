
#include "utils.h"

void gen_statements(ASTstruct *ast);
void gen_func_def(ASTstruct *ast);
void gen_var_assignment(ASTstruct *ast);
void gen_func_call(ASTstruct *ast);
void gen_func_params(ASTstruct *ast);
void gen_func_body (ASTstruct *ast);
void gen_write(ASTstruct *ast);