#ifndef IFJ22_SEMANTICS_H
#define IFJ22_SEMANTICS_H
#include "symtable.h"
int getVarType(int type);
void *getParamTypes(int type,char paramTypeChar[2]);
void semCheck(ASTstruct *tree,FSTable *ftab,char *name);
void functionBody(ASTstruct *tree,FSTable *ftab,char *name,ASTstruct *back);
void insert_global(ASTstruct *tree,FSTable *ftab);
void paramsToTable(ASTstruct *tree,FSTable *ftab);
void function_params(ASTstruct *tree,FSTable *ftab);
void insert_function(ASTstruct *tree,FSTable *ftab);
void semantics();
#endif //IFJ22_SEMANTICS_H
