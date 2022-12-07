#include "semantics.h"
#include "utils.h"

int count = 0; //pomocna promenna, aby nedochazelo k opakovanemu vkladani globalniho ramce do tabulky symbolu
int done = 0;
//funkce, ktera zjisti datovy typ promenne
int getVarType(int type){
    switch(type){
        case NODE_INT:
            return 1;
        case NODE_FLOAT:
            return 2;
        case NODE_STRING:
            return 3;
    }
}
//funkce, ktera zjisti typ parametru a prevede ho z celeho cisla na znak
void *getParamTypes(int type,char paramTypeChar[2]){
    int paramType;
    switch (type) {
        case NODE_INT:
            paramType = 1;
            itoac(paramType, paramTypeChar);   //itoac - funkce, ktera prevadi cele cislo na znak
            break;
        case NODE_FLOAT:
            paramType = 2;
            itoac(paramType, paramTypeChar);
            break;
        case NODE_STRING:
            paramType = 3;
            itoac(paramType, paramTypeChar);
            break;
    }
}

//funkce, ktera provadi semanticke kotroly
void semCheck(ASTstruct *tree,FSTable *ftab,char *name){
    ASTstruct *tmp;
    st_function *pointer = NULL;
    int count = 0;
    char *parameters = "";
    char *parametersTmp = "";
    char paramTypeChar[2];
    st_function *FSTable_ptr = NULL;
    st_item *STable_ptr = NULL;
    int type;
    switch(tree->rightNode->type){
        case NODE_WRITE: {
            st_function *pointer = NULL;
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if(tmp->leftNode->rightNode->type == NODE_VAR_ID){
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr,tmp->leftNode->rightNode->value->data.stringPtr->value) == NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                }
                tmp = tmp->leftNode;
            }
            break;
        }
        case NODE_STRLEN:{
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if (tree->rightNode->leftNode->rightNode->type == NODE_VAR_ID) {
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr,
                                  tree->rightNode->leftNode->rightNode->value->data.stringPtr->value) == NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                    st_item *var;
                    var = st_search(pointer->symtab_ptr,
                                    tree->rightNode->leftNode->rightNode->value->data.stringPtr->value);
                    if (var->key_type != 3) {
                        error_exit(PARAMS_ERR, "Semantic error! Function must have string as parameter. ");
                    }
                }
                count++;
                tmp = tmp->leftNode;
            }
            if(count != 1){
                error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
            }
            break;
        }
        case NODE_SUBSTRING:{

        }
        case NODE_ORD:{
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if (tree->rightNode->leftNode->rightNode->type == NODE_VAR_ID) {
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value) ==
                        NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                    st_item *var;
                    var = st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value);
                    if (var->key_type != 3) {
                        error_exit(PARAMS_ERR, "Semantic error! Function must have string as parameter. ");
                    }
                }
                count++;
                tmp = tmp->leftNode;
            }
            if(count != 1){
                error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
            }
            break;
        }
        case NODE_CHR:{
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if (tree->rightNode->leftNode->rightNode->type == NODE_VAR_ID) {
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value) ==
                        NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                    st_item *var;
                    var = st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value);
                    if (var->key_type != 1) {
                        error_exit(PARAMS_ERR, "Semantic error! Function must have integer as parameter. ");
                    }
                }
                count++;
                tmp = tmp->leftNode;
            }
            if(count != 1){
                error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
            }
            break;
        }
        case NODE_FLOATVAL:{
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if (tree->rightNode->leftNode->rightNode->type == NODE_VAR_ID) {
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value) ==
                        NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                }
                count++;
                tmp = tmp->leftNode;
            }
            if(count != 1){
                error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
            }
            break;
        }
        case NODE_INTVAL:{
            tmp = tree->rightNode;
            while(tmp->leftNode != NULL) {
                if (tree->rightNode->leftNode->rightNode->type == NODE_VAR_ID) {
                    pointer = fst_search(ftab, name);
                    if (st_search(pointer->symtab_ptr, tree->rightNode->leftNode->rightNode->value->data.stringPtr->value) ==
                        NULL) {
                        error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                    }
                }
                count++;
                tmp = tmp->leftNode;
            }
            if(count != 1){
                error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
            }
            break;
        }
        case NODE_FUNC_ID: {
            if (fst_search(ftab, tree->rightNode->value->data.stringPtr->value) == NULL) {
                error_exit(UNDEF_FUNC_ERR, "Semantic error! Call of undefined function. ");
            }
            tmp = tree->rightNode;
            if(tmp->leftNode != NULL) {
                while (tmp->leftNode != NULL) {
                    if (tmp->leftNode->rightNode->type == NODE_VAR_ID) {
                        FSTable_ptr = fst_search(ftab, name);
                        STable_ptr = st_search(FSTable_ptr->symtab_ptr,tmp->leftNode->rightNode->value->data.stringPtr->value);

                        //kotrola, jestli existuje promenna
                        if (st_search(FSTable_ptr->symtab_ptr,
                                      tmp->leftNode->rightNode->value->data.stringPtr->value) == NULL) {
                            error_exit(UNDEF_VAR_ERR, "Semantic error! Undefined variable. ");
                        } else {
                            type = STable_ptr->key_type;
                        }
                        itoac(type, paramTypeChar);
                        parametersTmp = string_concatenate(parameters, paramTypeChar);
                        parameters = parametersTmp;
                    } else {
                        getParamTypes(tmp->leftNode->rightNode->type, &paramTypeChar);
                        parametersTmp = string_concatenate(parameters, paramTypeChar);
                        parameters = parametersTmp;
                    }
                    tmp = tmp->leftNode;
                    count++;
                }
                st_function *func = fst_search(ftab, tree->rightNode->value->data.stringPtr->value);
                if (count != func->params) {
                    error_exit(PARAMS_ERR, "Semantic error! Wrong number of parameters. ");
                }

                if (strcmp(parameters, func->parameters) != 0) {
                    error_exit(PARAMS_ERR, "Semantic error! Wrong type of parameters. ");
                }

                count = 0;
            }
            break;
        }
        case NODE_VAR_ASSIGNMENT:{
            if(tree->rightNode->rightNode->type == NODE_STRLEN || tree->rightNode->rightNode->type == NODE_SUBSTRING || tree->rightNode->rightNode->type == NODE_ORD ||
                    tree->rightNode->rightNode->type == NODE_CHR || tree->rightNode->rightNode->type == NODE_FLOATVAL || tree->rightNode->rightNode->type == NODE_FUNC_ID)
            {
                semCheck(tree->rightNode,ftab,name);
            }
            break;
        }
        case NODE_RETURN:{
            st_function *pointer = NULL;
            int type;
            pointer = fst_search(ftab,name);
            if(tree->rightNode->leftNode == NULL || tree->rightNode->leftNode->type == NODE_NULL){
               type = 0;
            }else{
               type = getVarType(tree->rightNode->leftNode->type);
            }
            if(pointer->retType == 0){
                if(tree->rightNode->leftNode->type == NODE_INT || tree->rightNode->leftNode->type == NODE_FLOAT || tree->rightNode->leftNode->type == NODE_STRING)
                {
                    error_exit(EXPR_ERR, "Semantic error! Return in void function. ");
                }
            }else {
                if (pointer->retType != type) {
                    error_exit(PARAMS_ERR, "Semantic error! Wrong return type. ");
                }
            }
            break;
        }
        default:
            break;
    }
}

//funkce, ktera prochazi telo funkce
void functionBody(ASTstruct *tree,FSTable *ftab,char *name,ASTstruct *back){
    st_function *pointer = NULL;
    //kdyz je telo funkce prazdne, vrati se zpet do funkce "insert_function"
    if(strcmp(name,"0") != 0) {
        if (back->rightNode->type == NODE_FUNC_DEF) {
            if (back->rightNode->rightNode == NULL) {
                return;
            }
        }
    }
        if (tree->rightNode->type == NODE_VAR_ASSIGNMENT) {
            pointer = fst_search(ftab, name);
            st_insert(pointer->symtab_ptr, getVarType(tree->rightNode->rightNode->type),tree->rightNode->leftNode->value->data.stringPtr->value);
        }
        if (tree->rightNode->type == NODE_IF) {
            int x;
            ASTstruct *tmp = tree->rightNode->leftNode->rightNode;
            //if((tmp->rightNode != NODE_VAR_ID) &&(tmp->leftNode != NODE_VAR_ID)) {
            switch (tmp->type) {

                case NODE_GREATER: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = false;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer > tmp->rightNode->value->data.integer) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal > tmp->rightNode->value->data.decimal) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) < 0) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                        }
                    }
                    break;
                }
                case NODE_GREATER_EQ: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = false;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer >= tmp->rightNode->value->data.integer) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal >= tmp->rightNode->value->data.decimal) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) <= 0) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                        }
                    }
                    break;
                }
                case NODE_LESS: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = false;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer < tmp->rightNode->value->data.integer) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal < tmp->rightNode->value->data.decimal) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) > 0) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                        }
                    }
                    break;
                }
                case NODE_LESS_EQ: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = false;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer <= tmp->rightNode->value->data.integer) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal <= tmp->rightNode->value->data.decimal) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) >= 0) {
                                    x = true;
                                } else x = false;
                                break;
                            }
                        }
                    }
                    break;
                }
                case NODE_COMPARE: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = false;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer != tmp->rightNode->value->data.integer) {
                                    x = false;
                                } else {
                                    x = true;
                                }
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal != tmp->rightNode->value->data.decimal) {
                                    x = false;
                                } else {
                                    x = true;
                                }
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) != 0) {
                                    x = false;
                                } else {
                                    x = true;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                case NODE_NEG_COMPARE: {
                    if (tmp->leftNode->type != tmp->rightNode->type) {
                        x = true;
                    } else {
                        switch (tree->rightNode->leftNode->rightNode->leftNode->type) {
                            case NODE_INT: {
                                if (tmp->leftNode->value->data.integer != tmp->rightNode->value->data.integer) {
                                    x = true;
                                } else {
                                    x = false;
                                }
                                break;
                            }
                            case NODE_FLOAT: {
                                if (tmp->leftNode->value->data.decimal != tmp->rightNode->value->data.decimal) {
                                    x = true;
                                } else {
                                    x = false;
                                }
                                break;
                            }
                            case NODE_STRING: {
                                if (strcmp(tmp->leftNode->value->data.stringPtr->value,
                                           tmp->rightNode->value->data.stringPtr->value) != 0) {
                                    x = true;
                                } else {
                                    x = false;
                                }
                                break;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
            if (x) {
                if (tree->rightNode->rightNode->rightNode != NULL) {
                    functionBody(tree->rightNode->rightNode->rightNode, ftab, name, back);
                }
            } else {
                if (tree->rightNode->rightNode->leftNode != NULL) {
                    functionBody(tree->rightNode->rightNode->leftNode, ftab, name, back);
                }
            }
            //}
        }

        semCheck(tree, ftab, name);

    if(tree->leftNode != NULL){
        functionBody(tree->leftNode,ftab,name,back);
    }
}

//funkce, ktera vlozi globalni ramec do tabulky symbolu
void insert_global(ASTstruct *tree,FSTable *ftab){
    tree = ast->rightNode->leftNode;
    STable *stab = (STable *)malloc(sizeof(STable));
    if(stab == NULL){
        error_exit(INT_ERR,"Memory allocation error. ");
    }
    st_init(stab);
    //globalni ramec ma klic "0", protoze zadna jina funkce nemuze zacinat cislem
    fst_insert(ftab,stab,NULL,"0",NULL, NULL);
    functionBody(tree,ftab,"0",NULL);
}

//funkce, ktera vlozi parametry uzivatelem vytvorene funkce do tabulky symbolu pro promenne
void paramsToTable(ASTstruct *tree,FSTable *ftab){
    ASTstruct *tmp;
    int parType;
    st_function *pointer = NULL;
    tmp = tree->rightNode->leftNode;

        if (tree->rightNode->type == NODE_FUNC_DEF) {
            while (tmp->leftNode != NULL) {
                switch (tmp->leftNode->rightNode->type) {
                    case NODE_PARAM_ID_INT:
                        parType = 1;
                        break;
                    case NODE_PARAM_ID_FLOAT:
                        parType = 2;
                        break;
                    case NODE_PARAM_ID_STRING:
                        parType = 3;
                        break;
                }
                pointer = fst_search(ftab, tree->rightNode->value->data.stringPtr->value);
                if (st_search(pointer->symtab_ptr, tmp->leftNode->rightNode->value->data.stringPtr->value) == NULL) {
                    st_insert(pointer->symtab_ptr, parType, tmp->leftNode->rightNode->value->data.stringPtr->value);
                    tmp = tmp->leftNode;
                } else {
                    error_exit(PARAMS_ERR, "Semantic error! Redefinion of parameters in function. ");
                }
            }
        }

}

//funkce, ktera zjisti typy parametru a typ navratove hodnoty funkce
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
                case NODE_PARAM_ID_INT: {
                    paramType = 1;
                    itoac(paramType, paramTypeChar);
                    break;
                }
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
    if(tree->rightNode->type == NODE_FUNC_DEF){

        //kotrola, jestli nedochazi k redefinici funkce
        if(fst_search(ftab,tree->rightNode->value->data.stringPtr->value) != NULL){
            error_exit(UNDEF_FUNC_ERR,"Semantic error! Re-definition of function. ");
        }else{
            function_params(tree,ftab);
        }
    }

    paramsToTable(tree,ftab);
    if(tree->rightNode->type == NODE_FUNC_DEF) {
        functionBody(tree->rightNode->rightNode,ftab,tree->rightNode->value->data.stringPtr->value,tree);
    }

    if(tree->leftNode != NULL){
        insert_function(tree->leftNode,ftab);
    }
    if(count == 0){
        insert_global(tree, ftab);
        count++;
    }
}
//funkce, kde se provede inicializace tabulky symbolu, nastavi se ukazatel do syntaktickeho stromu
void semantics(){
    if(ast->rightNode->leftNode != NULL) {
        FSTable *ftab = (FSTable *) malloc(sizeof(FSTable));
        fsTable = ftab;
        ASTstruct *tree;
        tree = ast->rightNode->leftNode;
        if (ftab == NULL) {
            error_exit(INT_ERR, "Memory allocation error. ");
        }
        fst_init(ftab);
        insert_function(tree, ftab);
    }
}