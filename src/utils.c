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

void DynamicStringRemoveChar(DynamicString *string){
    if (string->length > 0){
        string->value[--string->length] = '\0';
    }
}

void DynamicStringFree(DynamicString *string){
    free(string->value);
    string->capacity = 0;
    string->length = 0;
    free(string);
}

bool DynamicStringCompare(DynamicString *string, const char *string2){
    return !strcmp(string->value, string2);
}

void DynamicStringClean(DynamicString *string){
    string->capacity = 8;
    DynamicStringResize(string, string->capacity);
    string->length = 0;
    string->value[0] = '\0';
}

TokenValueType getValueType(TokenType tokenType){
    switch (tokenType) {
        case TOKEN_TYPE_ID:
        case TOKEN_VAR_ID:
        case TOKEN_FUN_ID:
        case TOKEN_ID:
        case TOKEN_STRING:
            return VALUE_STRING;
        case TOKEN_INT:
            return VALUE_INT;
        case TOKEN_DOUBLE:
            return VALUE_DOUBLE;
        default:
            return VALUE_NULL;
    }
}

Token * TokenInit(TokenType tokenType, DynamicString *string){
    Token *tokenPtr = malloc(sizeof(Token));
    char **endPtr = NULL;
    if (!tokenPtr){
        ERROR_CODE = INT_ERR;
        return NULL;
    }

    switch (tokenPtr->valueType = getValueType(tokenType)) {
        case VALUE_INT:
            tokenPtr->value.integer = strtol(string->value, endPtr, 10);
            DynamicStringFree(string);
            break;

        case VALUE_DOUBLE:
            tokenPtr->value.decimal = strtod(string->value, endPtr);
            DynamicStringFree(string);
            break;

        case VALUE_STRING:
            tokenPtr->value.stringPtr = DynamicStringInit();
            DynamicStringCopy(string, tokenPtr->value.stringPtr);
            DynamicStringFree(string);
            break;

        case VALUE_NULL:
            tokenPtr->value.integer = 0;
            DynamicStringFree(string);
            break;
    }

    tokenPtr->type = tokenType;

    return tokenPtr;
}


//todo fix
Token *TokenCopy(Token *src){
    if (src->valueType == VALUE_STRING){
        Token *new = TokenInit(src->type, src->value.stringPtr);
        return new;
    }
}

void TokenPrint(Token *token){
    if (token){
        switch (token->valueType) {
            case VALUE_STRING:
                printf("{%s, %d}\n", token->value.stringPtr->value, token->type);
                break;

            case VALUE_INT:
                printf("{%d, %d}\n", token->value.integer, token->type);
                break;

            case VALUE_DOUBLE:
                printf("{%f, %d}\n", token->value.decimal, token->type);
                break;

            case VALUE_NULL:
                printf("{NULL, %d}\n", token->type);
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

Stack *StackInit(){
    Stack *stack = malloc(sizeof(stack));
    stack->size = 128;
    stack->value = calloc(stack->size, stack->size * sizeof(Token*));
    stack->top = 0;
    stack->size = 128;
    return stack;
}

bool StackIsEmpty(Stack *stack){
    return stack->top == 0;
}

Token *StackTop(Stack *stack){
    if (StackIsEmpty(stack)){
        return NULL;
    }
    return stack->value[stack->top - 1];
}

void StackPush(Stack *stack, Token *token){
    if (stack->top == stack->size){
        stack->size *= 2;
        stack->value = realloc(stack->value, stack->size * sizeof(Token*));
    }
    stack->value[stack->top++] = token;
}

Token* StackPop(Stack *stack)
{
   if (stack->top != 0)
   {
		stack->top--;
   }

}

void StackPrint(Stack *stack){
    for (int i = stack->top; i >= 0; --i) {
        TokenPrint(stack->value[i]);
    }
}

void StackFree(Stack *stack){
    for (int i = 0; i < stack->top; ++i) {
        TokenFree(stack->value[i]);
    }
    free(stack->value);
    free(stack);
}

void StackFlip(Stack *stack){
    Token *tmp;
    int k = stack->top - 1;
    for (int i = 0; i < (stack->top / 2); ++i) {
        tmp = stack->value[i];
        stack->value[i] = stack->value[k];
        stack->value[k--] = tmp;
    }
}

ASTstruct *createNode(int type, DynamicString *value, ASTstruct *leftNode, ASTstruct *rightNode)
{
    ASTstruct *tree = malloc(sizeof(struct ASTstruct));
    if (tree == NULL){
        ERROR_CODE = INT_ERR;
        return NULL;
    }
    
    tree->type = type;
    tree->value = value;
    tree->leftNode = leftNode;
    tree->rightNode = rightNode;

    return tree;
}


Token *loadToken(Stack *stack)
{
	if (StackIsEmpty(stack))
	{
		return NULL;
	}

	Token *temp = (Token *)StackTop(stack);
	StackPop(stack);

	return temp;
}

void unloadToken(Stack *stack)
{
    stack->top++;
}


void expectToken(int type, Stack *stack)
{
   if (StackIsEmpty(stack))
   {
       error_exit(SYN_ERR, "Syntax error!");
   }
   if (loadToken(stack)->type != type)
   {
       error_exit(SYN_ERR, "Syntax error!");
   }
}