#include "scanner.h"



Token * TokenInit(TokenType tokenType,TokenValueType valueType, DynamicString *string){
    Token *tokenPtr = malloc(sizeof(Token));
    char **endPtr = NULL;
    if (!tokenPtr){
        ERROR_CODE = INT_ERR;
        return NULL;
    }

    switch (valueType) {
        case VALUE_INT:
            tokenPtr->value.value_int = strtol(string->value, endPtr, 10);
            break;

        case VALUE_DOUBLE:
            tokenPtr->value.value_double = strtod(string->value, endPtr);
            break;

        case VALUE_STRING:
            DynamicStringCopy(string, &tokenPtr->value.value_str);
            break;

        case VALUE_NULL:
            tokenPtr->value.value_int = 0;
    }

    tokenPtr->type = tokenType;

    return tokenPtr;
}

void TokenFree(Token *token){
    if (token->valueType == VALUE_STRING){
        DynamicStringFree(&token->value.value_str);
    }
    free(token);
}