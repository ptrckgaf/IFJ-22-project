#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    DynamicString *stringPtr = DynamicStringInit();
    DynamicString *stringPtr2 = DynamicStringInit();
    for (int i = 0; i < 1020; ++i) {
        DynamicStringAddChar(stringPtr, 'q');
    }
    DynamicStringCopy(stringPtr, stringPtr2);
    Token *token = TokenInit(TOKEN_INT, VALUE_STRING, stringPtr);
    printf("%s\n", token->value.stringPtr->value);

    DynamicStringFree(stringPtr);
    DynamicStringFree(stringPtr2);
    TokenFree(token);
    return 0;
}
