#include "symtable.h"
int ST_SIZE = MAX_SIZE;
int tableCount = 0;


//Rozptylovacia funkcia, prideli index zadanemu klucu
int get_hash(tKey key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        result += key[i];
    }
    return (result % ST_SIZE);
}


//Inicializacia tabulky
void st_init(STable *table) {
    if (table != NULL) {
        for (int i = 0; i < ST_SIZE; i++) {
            (*table)[i] = NULL;
        }
    }
    else return;
}


//Vyhladanie prvku v tabulke, vracia NULL ak prvok nebol najdeny
st_item *st_search(STable *table, tKey key) {
    int hash = get_hash(key);
    if (table != NULL) {
        st_item *ptr = (*table)[hash];
        while (ptr != NULL && strcmp(ptr->key, key)) {
            ptr = ptr->next;
        }
        return ptr;
    }
    else return NULL;
}


//Vlozenie noveho prvku do tabulky
void st_insert(STable *table, int key_type, tKey key, char *value) {
    st_item *insrt = st_search(table, key);
    if (insrt != NULL) {
        insrt->value = value;
        insrt->key_type = key_type;
    }
    else {
        int hash = get_hash(key);

        insrt = malloc(sizeof(st_item));
        if (insrt == NULL) {
            error_exit(INT_ERR, "Memory allocation failure.");
        }

        insrt->value = value;
        insrt->key = key;
        insrt->key_type = key_type;
        insrt->next = (*table)[hash];

        (*table)[hash] = insrt;
    }
}


//Zmazanie celej tabulky, odstrani vsetky prvky
void st_delete(STable *table) {
    st_item *tmp;
    st_item *next;

    for (int i = 0; i < ST_SIZE; i++) {
        tmp = (*table)[i];
        (*table)[i] = NULL;

        while (tmp != NULL) {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }
}



//Inicializacia tabulky
void fst_init(FSTable *table) {
    if (table != NULL) {
        for (int i = 0; i < ST_SIZE; i++) {
            (*table)[i] = NULL;
        }
    }
    else return;
}


//Vyhladanie funkcie v tabulke, vracia NULL ak funkcia nebola najdena
st_function *fst_search(FSTable *table, tKey key) {
    int hash = get_hash(key);
    if (table != NULL) {
        st_function *ptr = (*table)[hash];
        while (ptr != NULL && strcmp(ptr->key, key)) {
            ptr = ptr->next;
        }
        return ptr;
    }
    else return NULL;
}


//Vlozenie novej funckie do tabulky
void fst_insert(FSTable *table, STable *symtab_ptr, tParams parameters, tKey key, int retType, int params) {
    st_function *insrt = fst_search(table, key);
    if (insrt != NULL) {
        insrt->retType = retType;
        insrt->params = params;
        insrt->parameters = parameters;
        insrt->symtab_ptr = symtab_ptr;
    }
    else {
        int hash = get_hash(key);

        insrt = malloc(sizeof(st_function));
        if (insrt == NULL) {
            error_exit(INT_ERR, "Memory allocation failure.");
        }

        insrt->retType = retType;
        insrt->params = params;
        insrt->key = key;
        insrt->parameters = parameters;
        insrt->next = (*table)[hash];
        insrt->symtab_ptr = symtab_ptr;

        (*table)[hash] = insrt;
        tableCount++;
    }
}


//Zmazanie celej tabulky, odstrani vsetky funkcie
void fst_delete(FSTable *table) {
    st_function *tmp;
    st_function *next;

    for (int i = 0; i < ST_SIZE; i++) {
        tmp = (*table)[i];
        (*table)[i] = NULL;

        while (tmp != NULL) {
            next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }
}


st_item *getStItem(tKey funcName, tKey varName, FSTable *fsTable) {
    st_function *function = fst_search(fsTable,funcName);
    return st_search(function->symtab_ptr, varName);
}
