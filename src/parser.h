#ifndef IFJ22_PARSER_H
#define IFJ22_PARSER_H

#include "utils.h"

ASTstruct *return_type(Stack *stack);
ASTstruct *params(Stack *stack);
ASTstruct *statement(Stack *stack);
ASTstruct *func_args(Stack *stack);
ASTstruct *str_arg(Stack *stack);
ASTstruct *int_arg(Stack *stack);
ASTstruct *substr_args(Stack *stack);
ASTstruct *expression(Stack *stack, int preced);
ASTstruct *function_define(Stack *stack);
void Print_tree(ASTstruct* TempTree);
void Print_tree2(ASTstruct* TempTree, char* sufix, char fromdir);

ASTstruct *parse(Stack *stack);
ASTstruct *prolog(Stack *stack);
ASTstruct *prog(Stack *stack);


#endif //IFJ22_PARSER_H
