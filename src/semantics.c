#include "symtable.h"
#include "semantics.h"
#include "utils.h"

ASTstruct *ast;
int scope = 0;
void function_atr(ASTstruct *tree,FSTable *ftab){
    STable *stab = (STable *)malloc(sizeof(STable));
    if(stab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    st_init(stab);
    fst_insert(ftab,NULL,1,tree->rightNode->value->data.stringPtr->value,0,0);
}
void insert_function(FSTable *ftab){
    ASTstruct *tree;
    tree = ast->rightNode->leftNode;
    while(tree->leftNode != NULL){
        if(tree->rightNode->type == 2){
            if(fst_search(ftab,tree->rightNode->value->data.stringPtr->value)!= NULL){
                error_exit(UNDEF_FUNC_ERR,"Semantic error! Re-definition of function. ");
            }else{
                function_atr(tree,ftab);
            }
        }
        tree = tree->leftNode;
    }
}
int semantics(){
    FSTable *ftab = (FSTable *)malloc(sizeof(FSTable));
    if(ftab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    fst_init(ftab);
    insert_function(ftab);
}
