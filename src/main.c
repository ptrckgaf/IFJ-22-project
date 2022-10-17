#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    DynamicString *stringPtr = DynamicStringInit();
    DynamicString *stringPtr2 = DynamicStringInit();

    for (int i = 0; i < 5; ++i) {
        DynamicStringAddChar(stringPtr, 'v');
    }

    DynamicStringCopy(stringPtr, stringPtr2);
    DynamicStringAddChar(stringPtr2, 'a');

    Token *token = TokenInit(TOKEN_STRING, VALUE_STRING, stringPtr);
    Token *token2 = TokenInit(TOKEN_VAR_ID, VALUE_STRING, stringPtr2);

    Stack stack;
    Stack *stackPtr = &stack;
    StackInit(stackPtr);

    StackPush(stackPtr, token);
    StackPush(stackPtr, token2);
//    StackFree(stackPtr);

    printf("tokens:");
    StackPrint(stackPtr);
    Token *token3= StackPop(stackPtr);
    token3= StackPop(stackPtr);
    token3= StackPop(stackPtr);


    DynamicStringFree(stringPtr);
    DynamicStringFree(stringPtr2);
    TokenFree(token);
    TokenFree(token2);
    return 0;
}
