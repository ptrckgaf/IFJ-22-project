#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    DynamicString *stringPtr = DynamicStringInit();
    DynamicString *stringPtr2 = DynamicStringInit();
    for (int i = 0; i < 100; ++i) {
        DynamicStringAddChar(stringPtr, 'a');
    }
    DynamicStringCopy(stringPtr, stringPtr2);
    Token *token = TokenInit(TOKEN_INT, VALUE_STRING, stringPtr);
    printf("%s\n", token->value.value_str->value);
//    printf("%s\n %d\n", stringPtr->value, stringPtr->length);
    DynamicStringFree(stringPtr);
    DynamicStringFree(stringPtr2);
    TokenFree(token);
    return 0;
}
