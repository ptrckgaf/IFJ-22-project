#include "symtable.h"
int ST_SIZE = MAX_SIZE;

//Rozptylovacia funkcia, prideli index zadanemu klucu
int get_hash(char *key) {
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
st_item *st_search(STable *table, char *key) {
    return;
}

//Vracia pointer na hodnotu prvku, ak nenasiel vracia NULL
char *ht_get(STable *table, char *key) {
    return;
}

//Vlozenie noveho prvku do tabulky
void st_insert(STable *table, int key_type, char *key, char *value) {
    return;
}

//Odstranenie prvku z tabulky, uvolni alokovane zdroje
void st_remove(STable *table, char *key) {
    return;
}

//Zmazanie celej tabulky, odstrani vsetky prvky
void st_delete(STable *table) {
    return;
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
st_function *fst_search(FSTable *table, char *key) {
    return;
}

//Vlozenie novej funckie do tabulky
void fst_insert(FSTable *table, STable *symtab_ptr, int key_type, char *key, int values, int params) {
    return;
}

//Odstranenie funkcie z tabulky, uvolni alokovane zdroje
void fst_remove(FSTable *table, char *key) {
    return;
}

//Zmazanie celej tabulky, odstrani vsetky funkcie
void fst_delete(FSTable *table) {
    return;
}
