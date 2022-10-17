#include "utils.h"

int ERROR_CODE = 0;

DynamicString* DynamicStringInit(){
    DynamicString *string = malloc(sizeof(DynamicString));
    string->length = 0;
    string->capacity = 8;
    string->value = malloc(string->capacity);
    if (!string->value){
        ERROR_CODE = INT_ERR;
    }
    return string;
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
    free(string);
}

bool DynamicStringCompare(DynamicString *string, const char *string2){
    return strcmp(string->value, string2);
}

void DynamicStringClean(DynamicString *string){
    string->length = 0;
    string->value[0] = '\0';
}

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
            break;
    }
    tokenPtr->valueType = valueType;

    tokenPtr->type = tokenType;

    return tokenPtr;
}

void TokenPrint(Token *token){
    if (token){
        switch (token->valueType) {
            case VALUE_STRING:
                printf("{%s, %d}", token->value.stringPtr->value, token->type);
                break;

            case VALUE_INT:
                printf("{%d, %d}", token->value.integer, token->type);
                break;

            case VALUE_DOUBLE:
                printf("{%f, %d}", token->value.decimal, token->type);
                break;

            case VALUE_NULL:
                printf("{NULL, %d}", token->type);
                break;
        }
    }
}

void TokenFree(Token *token){
    if (token->valueType == VALUE_STRING){
        DynamicStringFree(token->value.stringPtr);
    }
    free(token);
}

void StackInit(Stack *stack){
    stack->top = NULL;
}

Token* StackPop(Stack *stack){
    if (stack->top){
        Token *token = stack->top->data;
        NodePtr tmp = stack->top;
        stack->top = stack->top->previous;
        free(tmp);
        return token;
    } else{
        return NULL;
    }
}

Token* StackTop(Stack *stack){
    return stack->top->data;
}

unsigned int StackSize(Stack *stack){
    NodePtr current = stack->top;
    unsigned int size = 0;

    while (current){
        size++;
        current = current->previous;
    }
    return size;
}

void StackPush(Stack *stack, Token *token){
    NodePtr node = malloc(sizeof(struct Node));
    if (!node){
        ERROR_CODE = INT_ERR;
    }

    node->data = token;
    node->previous = stack->top;
    stack->top = node;
}

void StackPrint(Stack *stack){
    if (stack){
        NodePtr current = stack->top;

        while (current) {
            TokenPrint(current->data);
            current = current->previous;
        }
    }
}

void StackFree(Stack *stack){
    NodePtr current = stack->top;
    NodePtr previous = NULL;
    
    while (current){
        previous = current->previous;
        free(current);
        current = previous;
    }
    stack->top = NULL;
}
