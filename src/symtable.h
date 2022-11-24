#include "utils.h"
#define MAX_SIZE 23 //Maximalna velkost tabulky

typedef struct st_item {
    int key_type; //Typ kluca, kde 1..integer
    char *key;                  // 2..double
    char *value;                // 3..string
    struct st_item *next;
} st_item;

typedef st_item *STable[MAX_SIZE];

typedef struct st_function {
    int key_type;
    char *key;
    int values;
    int params;
    struct st_function *next;
    STable *symtab_ptr;
} st_function;

typedef st_function *FSTable[MAX_SIZE];

int get_hash(char *key);
void st_init(STable *table);
st_item *st_search(STable *table, char *key);
char *ht_get(STable *table, char *key);
void st_insert(STable *table, int key_type, char *key, char *value);
void st_remove(STable *table, char *key);
void st_delete(STable *table);

void fst_init(FSTable *table);
st_function *fst_search(FSTable *table, char *key);
void fst_insert(FSTable *table, STable *symtab_ptr, int key_type, char *key, int values, int params);
void fst_remove(FSTable *table, char *key);
void fst_delete(FSTable *table);
extern FSTable *fst;
