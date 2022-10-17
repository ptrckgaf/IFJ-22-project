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
            tokenPtr->value.integer = strtol(string->value, endPtr, 10);
            break;

        case VALUE_DOUBLE:
            tokenPtr->value.decimal = strtod(string->value, endPtr);
            break;

        case VALUE_STRING:
            tokenPtr->value.stringPtr = DynamicStringInit();
            DynamicStringCopy(string, tokenPtr->value.stringPtr);
            break;

        case VALUE_NULL:
            tokenPtr->value.integer = 0;
    }
    tokenPtr->valueType = valueType;

    tokenPtr->type = tokenType;

    return tokenPtr;
}

void TokenFree(Token *token){
    if (token->valueType == VALUE_STRING){
        DynamicStringFree(token->value.stringPtr);
    }
    free(token);
}