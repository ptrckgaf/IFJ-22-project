/*
    - ifj22 compiler main
    - Author: Anton Miklis(xmikli05)
*/

#include "utils.h"
#include "scanner.h"
#include "semantics.h"

int main(){
    FILE *source = fopen("../debug.php", "r");//set source to stdin
    if (source == NULL){
        error_exit(INT_ERR, "Can't open file");
    }
    Stack *stack = scanner(source);
    if (stack == NULL){
        return LEX_ERR;
    }
    //StackPrint(stack);

    parser(stack);
    semantics();
    codegen();


    StackFree(stack);
    fclose(source);

    return 0;
}
