#include "utils.h"
#define MAX_SIZE 23 //Maximalna velkost tabulky
typedef char *tKey;
typedef char *tParams;

typedef struct st_item {
    int key_type; //Typ kluca, kde 1..integer, 2..double, 3..string
    tKey key;   
    bool is_defined;  // 1 - variable is deinfed in function scope, 0 - isn't
    struct st_item *next;
} st_item;

typedef st_item *STable[MAX_SIZE];


typedef struct st_function {
    tParams parameters; //typy parametrov: 1 - int, 2 - double, 3 - string
    tKey key;           //nazov funkcie
    int retType;        //navratovy typ: 1 - int, 2 - double, 3 - string
    int params;         //pocet parametrov
    struct st_function *next;
    STable *symtab_ptr;
} st_function;

typedef st_function *FSTable[MAX_SIZE];

int get_hash(tKey key);
void st_init(STable *table);
st_item *st_search(STable *table, tKey key);
void st_insert(STable *table, int key_type, tKey key);
void st_delete(STable *table);

void fst_init(FSTable *table);
st_function *fst_search(FSTable *table, tKey key);
void fst_insert(FSTable *table, STable *symtab_ptr, tParams parameters, tKey key, int retType, int params);
void fst_delete(FSTable *table);

st_function getSymPtr(tKey key);
/**
 * Function returns symtable item from given function scope
 * @param funcName
 * @param varName
 * @return
 */
st_item *getStItem(tKey funcName, tKey varName, FSTable (*fsTable));
extern FSTable *fsTable;
