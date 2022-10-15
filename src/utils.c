#include "utils.h"

static int ERROR_CODE;

void DynamicStringInit(DynamicString *string){
    string->length = 0;
    string->capacity = 8;
    string->value = malloc(string->capacity);
    if (!string->value){
        ERROR_CODE = INT_ERR;
    }
}

void DynamicStringResize(DynamicString *string, unsigned int new_capacity){
    string->capacity = new_capacity;
    string->value = realloc(string->value, string->capacity);
    if (!string->value){
        ERROR_CODE = INT_ERR;
    }
}

void DynamicStringCopy(DynamicString *src, DynamicString *dst){
    DynamicStringResize(dst, src->capacity);
    dst->length = src->length;
    strcpy(dst->value, src->value);
}

void DynamicStringAddChar(DynamicString *string, char ch){
    if (string->length >= 0.75 * string->capacity){
        DynamicStringResize(string, 2 * string->capacity);
    }
    string->value[string->length] = ch;
    string->value[++string->length] = '\0';
}

void DynamicStringFree(DynamicString *string){
    free(string->value);
    string->capacity = 0;
    string->length = 0;
}

bool DynamicStringCompare(DynamicString *string, const char *string2){
    return strcmp(string->value, string2);
}