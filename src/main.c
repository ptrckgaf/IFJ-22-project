#include "utils.h"
#include "scanner.h"
#include <wctype.h>
int main(int argc, char **argv){
//    DynamicString *stringPtr = DynamicStringInit();
//    DynamicString *stringPtr2 = DynamicStringInit();
//
//    for (int i = 0; i < 5; ++i) {
//        DynamicStringAddChar(stringPtr, 'v');
//    }
//
//    DynamicStringCopy(stringPtr, stringPtr2);
//    DynamicStringAddChar(stringPtr2, 'a');
//
//    Token *token = TokenInit(TOKEN_STRING, VALUE_STRING, stringPtr);
//    Token *token2 = TokenInit(TOKEN_VAR_ID, VALUE_STRING, stringPtr2);
//
//    Stack stack;
//    Stack *stackPtr = &stack;
//    StackInit(stackPtr);
//
//    StackPush(stackPtr, token);
//    StackPush(stackPtr, token2);
//    StackFree(stackPtr);
//
//    printf("tokens:");
//    StackPrint(stackPtr);
//    Token *token3= StackPop(stackPtr);
//    token3= StackPop(stackPtr);
//    token3= StackPop(stackPtr);
//
//
//    DynamicStringFree(stringPtr);
//    DynamicStringFree(stringPtr2);
//    TokenFree(token);
//    TokenFree(token2);

    FILE *source = fopen("/Users/antonmiklis/FIT/IFJ/IFJ-22-projekt/debug.php", "r");

    Stack stack;
    Stack *stackPtr = &stack;
    StackInit(stackPtr);

    DynamicString *bufferPtr = DynamicStringInit();
    DynamicString *tokenValuePtr;

    AutomatonState current = START;
    AutomatonState next = START;

    TokenType tokenType;
    char input = ' ';


    while (true){
        input = fgetc(source);
        if (input == EOF){
            break;
        }

        next = AutomatonNext(current, input);
        if (isStateFinal(next) || isStateFinal(current)){
            DynamicStringAddChar(bufferPtr, input);
        }
        if (next == ERROR){
            if (isStateFinal(current)){
                ungetc(input, source);
                DynamicStringRemoveChar(bufferPtr);
                tokenType = getToken(current);

                //Initializes dynamic string in new memory location and copies there bufferPtr content
                tokenValuePtr = DynamicStringInit();
                DynamicStringCopy(bufferPtr, tokenValuePtr);

                StackPush(stackPtr, TokenInit(tokenType, tokenValuePtr));
                DynamicStringClean(bufferPtr);
                current = START;
            } else{
                return LEX_ERR;
            }
        } else{
            current = next;
        }
    }
    StackPrint(stackPtr);

    return 0;
}
