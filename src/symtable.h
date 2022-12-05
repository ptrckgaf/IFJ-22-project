#include "utils.h"
#define MAX_SIZE 23 //Maximalna velkost tabulky
typedef char *tKey;
typedef char *tParams;

typedef struct st_item {
    int key_type; //Typ kluca, kde 1..integer
    tKey key;                   // 2..double
    char *value;                // 3..string
    struct st_item *next;
    struct st_item *nextScope;
} st_item;

/*typedef struct st_item {
    int key_type; //Typ kluca, kde 1..integer
    tKey key;                   // 2..double
    char *value;                // 3..string
    bool isParam;
    struct st_item *next;
    struct st_item *nextScope;
} st_item;*/

typedef st_item *STable[MAX_SIZE];

typedef struct st_function {
    tParams parameters; //typy parametru: 1 - int,2 - double,3 - string
    //int key_type;
    tKey key;       //nazev funkce
    int retType;  //navratovy typ: 1 - int,2 - double,3 - string
    int params;  //pocet parametru
    struct st_function *next;
    STable *symtab_ptr;
} st_function;

typedef st_function *FSTable[MAX_SIZE];

int get_hash(tKey key);
void st_init(STable *table);
st_item *st_search(STable *table, tKey key);
void st_insert(STable *table, int key_type, tKey key, char *value);
//void st_insert(STable *table, int key_type, tKey key, char *value,bool isParam);
void st_remove(STable *table, char *key);
void st_delete(STable *table);

void fst_init(FSTable *table);
st_function *fst_search(FSTable *table, tKey key);
void fst_insert(FSTable *table, STable *symtab_ptr, tParams parameters, tKey key, int retType, int params);
void fst_remove(FSTable *table, tKey key);
void fst_delete(FSTable *table);
st_function getSymPtr(tKey key);
extern FSTable *ftab;
