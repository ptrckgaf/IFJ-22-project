#include "utils.h"
#define MAX_SIZE 23 //Maximalna velkost tabulky
typedef char *tKey;

typedef struct st_item {
    int key_type; //Typ kluca, kde 1..integer
    tKey key;                   // 2..double
    char *value;                // 3..string
    struct st_item *next;
    struct st_item *nextScope;
} st_item;

typedef st_item *STable[MAX_SIZE];

typedef struct st_function {
    int key_type;
    tKey key;
    int values;
    int params;
    struct st_function *next;
    STable *symtab_ptr;
} st_function;

typedef st_function *FSTable[MAX_SIZE];

int get_hash(tKey key);
void st_init(STable *table);
st_item *st_search(STable *table, tKey key);
void st_insert(STable *table, int key_type, tKey key, char *value);
void st_remove(STable *table, char *key);
void st_delete(STable *table);

void fst_init(FSTable *table);
st_function *fst_search(FSTable *table, tKey key);
void fst_insert(FSTable *table, STable *symtab_ptr, int key_type, tKey key, int values, int params);
void fst_remove(FSTable *table, tKey key);
void fst_delete(FSTable *table);
extern FSTable *fst;
