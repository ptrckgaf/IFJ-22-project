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

ValueType getValueType(TokenType tokenType){
    switch (tokenType) {
        case TOKEN_TYPE_ID:
        case TOKEN_VAR_ID:
        case TOKEN_ID:
        case TOKEN_STRING:
            return VALUE_STRING;
        case TOKEN_INT:
            return VALUE_INT;
        case TOKEN_FLOAT:
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

Value *ValueInit(){
    Value *val = malloc(sizeof(Value));
    if (val == NULL){
        error_exit(INT_ERR, "Malloc error")
    }
    val->valueType = VALUE_NULL;
    val->data.stringPtr = NULL;
    return val;
}

Value *TokenToValue(Token *token){
    Value *value = ValueInit();
    switch (token->valueType) {
        case VALUE_STRING:
            value->data.stringPtr = DynamicStringInit();
            DynamicStringCopy(token->value.stringPtr, value->data.stringPtr);
            break;
        case VALUE_NULL:
        case VALUE_INT:
            value->data.integer = token->value.integer;
            break;
        case VALUE_DOUBLE:
            value->data.decimal = token->value.decimal;
            break;
    }
    value->valueType = token->valueType;
    return value;
}

char *displayTokenType[] = {
        "TOKEN_ID",
        "TOKEN_VAR_ID",
        "TOKEN_TYPE_ID",

        "TOKEN_KEYWORD_ELSE",
        "TOKEN_KEYWORD_FLOAT",
        "TOKEN_KEYWORD_FUNCTION",
        "TOKEN_KEYWORD_IF",
        "TOKEN_KEYWORD_INT",
        "TOKEN_KEYWORD_NULL",
        "TOKEN_KEYWORD_RETURN",
        "TOKEN_KEYWORD_STRING",
        "TOKEN_KEYWORD_VOID",
        "TOKEN_KEYWORD_WHILE",

        "TOKEN_DECLARE",
        "TOKEN_STRICT_TYPES",

        "TOKEN_L_PAR",
        "TOKEN_R_PAR",
        "TOKEN_L_BRACKET",
        "TOKEN_R_BRACKET",
        "TOKEN_COMMA",
        "TOKEN_SEMICOLON",
        "TOKEN_COLON",

        "TOKEN_MUL",
        "TOKEN_DIV",
        "TOKEN_PLUS",
        "TOKEN_MINUS",
        "TOKEN_GREATER",
        "TOKEN_GREATER_EQ",
        "TOKEN_LESS",
        "TOKEN_LESS_EQ",
        "TOKEN_ASSIGN",
        "TOKEN_COMPARE",
        "TOKEN_NEG_COMPARE",
        "TOKEN_CONCATENATE",

        "TOKEN_INT",
        "TOKEN_FLOAT",
        "TOKEN_STRING",

        "TOKEN_READS",
        "TOKEN_READI",
        "TOKEN_READF",
        "TOKEN_WRITE",
        "TOKEN_STRLEN",
        "TOKEN_SUBSTRING",
        "TOKEN_CHR",
        "TOKEN_ORD",

        "TOKEN_PROLOG",
        "TOKEN_END",
};


//todo fix
Token *TokenCopy(Token *src){
    if (src->valueType == VALUE_STRING){
        Token *new = TokenInit(src->type, src->value.stringPtr);
        return new;
    }

    return NULL;
}

void TokenPrint(Token *token){
    if (token){
        switch (token->valueType) {
            case VALUE_STRING:
                printf("{%s, %s}\n", token->value.stringPtr->value, displayTokenType[token->type]);
                break;

            case VALUE_INT:
                printf("{%d, %s}\n", token->value.integer, displayTokenType[token->type]);
                break;

            case VALUE_DOUBLE:
                printf("{%f, %s}\n", token->value.decimal, displayTokenType[token->type]);
                break;

            case VALUE_NULL:
                printf("{NULL, %s}\n", displayTokenType[token->type]);
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

void StackPop(Stack *stack)
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

ASTstruct *createNode(int type, Token *token, ASTstruct *leftNode, ASTstruct *rightNode)
{
    ASTstruct *tree = malloc(sizeof(struct ASTstruct));
    if (tree == NULL){
        ERROR_CODE = INT_ERR;
        return NULL;
    }
    
    tree->type = type;
    if (token != NULL){
        tree->value = TokenToValue(token);
    } else{
        tree->value = NULL;
    }
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
   if ((int)loadToken(stack)->type != type)
   {
       error_exit(SYN_ERR, "Syntax error!");
   }
}