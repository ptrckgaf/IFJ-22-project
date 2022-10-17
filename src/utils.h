#ifndef IFJ_22_PROJEKT_UTILS_H
#define IFJ_22_PROJEKT_UTILS_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * error codes definition
 */
#define LEX_ERR 1;
#define SYN_ERR 2;
#define UNDEF_FUNC_ERR 3;
#define PARAMS_ERR 4;
#define UNDEF_VAR_ERR 5;
#define TERM_ERR 6;
#define TYPE_ERR 7;
#define LEX_ERROR 8;
#define INT_ERR 99;

extern int ERROR_CODE;

typedef struct DynamicString{
    char *value;
    unsigned int length;
    unsigned int capacity;  //memory allocated
}DynamicString;


/**
 * Dynamic string initialization
 * @param string
 */
DynamicString* DynamicStringInit();

/**
 * Increases string's capacity
 * @param string
 */
void DynamicStringResize(DynamicString *string, unsigned int new_capacity);

/**
 * Creates deep copy of string
 * @param string
 */
void DynamicStringCopy(DynamicString *src, DynamicString *dst);

/**
 * Frees memory occupied by string
 * @param string
 */
void DynamicStringFree(DynamicString *string);

/**
 * Adds character to string
 * @param string
 * @param ch
 */
void DynamicStringAddChar(DynamicString *string, char ch);

/**
 * Cleans dynamic string value
 * @param string
 */
void DynamicStringClean(DynamicString *string);

/**
 * Compares dynamic string with static string
 * @param string
 * @param string1
 * @return
 */
bool DynamicStringCompare(DynamicString *string, const char *string2);



#endif //IFJ_22_PROJEKT_UTILS_H
