#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    DynamicString string;
    DynamicStringInit(&string);
    DynamicStringAddChar(&string, '1');
    Token *token = TokenInit(TOKEN_INT, VALUE_INT, &string);
    printf("%d\n", token->value.value_int);
    DynamicStringFree(&string);
    TokenFree(token);
    return 0;
}
