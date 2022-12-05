#include "symtable.h"
#include "semantics.h"
#include "utils.h"

ASTstruct *ast;


semCheck(ASTstruct *tree,FSTable *ftab){
    printf("%d\n",tree->rightNode->type);
}
//zjisti datovy typ promenne
int getVarType(int type){
    switch(type){
        case 12:
            return 1;
        case 13:
            return 2;
        case 14:
            return 3;
    }
}
void functionBody(ASTstruct *tree,FSTable *ftab,char *name){
    printf("name %s\n",name);
    st_function *pointer = NULL;
    if(tree->rightNode->type == NODE_VAR_ASSIGNMENT){
        pointer = fst_search(ftab,name);
        st_insert(pointer->symtab_ptr, getVarType(tree->rightNode->rightNode->type),tree->rightNode->leftNode->value->data.stringPtr->value,NULL);
    }

    if(tree->leftNode != NULL){
        functionBody(tree->leftNode,ftab,name);
    }
}
void insert_global(ASTstruct *tree,FSTable *ftab){
    tree = ast->rightNode->leftNode;
    STable *stab = (STable *)malloc(sizeof(STable));
    if(stab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    st_init(stab);
    fst_insert(ftab,stab,NULL,"0",NULL,NULL);
    functionBody(tree,ftab,"0");
}
char *string_concatenate(char *s1,char *s2){
    int s1_length = strlen(s1);
    int s2_length = strlen(s2);
    int length = s1_length + s2_length + 1;
    char *s = (char *)malloc(sizeof(char)*length);

    for(int i = 0; i < s1_length; i++){
        s[i] = s1[i];
    }
    for(int i = 0; i < s2_length; i++){
        s[s1_length + i] = s2[i];
    }
    s[length - 1] = '\0';
    return s;
}


//parametry a navratova typ
void function_params(ASTstruct *tree,FSTable *ftab){
    char *parameters = "";
    char *parametersTmp = "";
    int retType;
    int paramType;
    char paramTypeChar[2];
    int params = 0;
    ASTstruct *tmp;
    tmp = tree->rightNode->leftNode;

    STable *stab = (STable *)malloc(sizeof(STable));
    if(stab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    st_init(stab);

    //navratovy typ
    switch(tmp->rightNode->type){
        case RETURN_TYPE_VOID:
            retType = 0;
            break;
        case RETURN_TYPE_INT:
            retType = 1;
            break;
        case RETURN_TYPE_FLOAT:
            retType = 2;
            break;
        case RETURN_TYPE_STRING:
            retType = 3;
            break;
    }

    if(tmp->leftNode == NULL){
        parameters = NULL;
        params = 0;
    }else {
        while (tmp->leftNode != NULL) {
            //typy parametru
            switch (tmp->leftNode->rightNode->type) {
                case NODE_PARAM_ID_INT:
                    paramType = 1;
                    itoac(paramType, paramTypeChar);
                    break;
                case NODE_PARAM_ID_FLOAT:
                    paramType = 2;
                    itoac(paramType, paramTypeChar);
                    break;
                case NODE_PARAM_ID_STRING:
                    paramType = 3;
                    itoac(paramType, paramTypeChar);
                    break;
            }
            parametersTmp = string_concatenate(parameters, paramTypeChar);
            parameters = parametersTmp;
            tmp = tmp->leftNode;
            params++;
        }
    }
    fst_insert(ftab,stab,parameters,tree->rightNode->value->data.stringPtr->value,retType,params);
}
void insert_function(ASTstruct *tree,FSTable *ftab){
    //printf("tree->rightNode->type %d\n",tree->rightNode->type);
    if(tree->rightNode->type == NODE_FUNC_DEF){
        if(fst_search(ftab,tree->rightNode->value->data.stringPtr->value) != NULL){
            error_exit(UNDEF_FUNC_ERR,"Semantic error! Re-definition of function. ");
        }else{
            function_params(tree,ftab);
        }
    }
    if(tree->rightNode->type == NODE_FUNC_DEF) {
        functionBody(tree->rightNode->rightNode,ftab,tree->rightNode->value->data.stringPtr->value);
    }
    if(tree->leftNode != NULL){
        insert_function(tree->leftNode,ftab);
    }
}


int semantics(){
    FSTable *ftab = (FSTable *)malloc(sizeof(FSTable));
    ASTstruct *tree;
    tree = ast->rightNode->leftNode;
    if(ftab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    fst_init(ftab);
    insert_global(ast,ftab);
    insert_function(tree,ftab);
    //printf("\ndone");
}