#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){

    // warning unused params
    argv[0] = NULL;
    (void)argc;

    FILE *source = fopen("../debug.php", "r");
    Stack *stack = scanner(source); //to read from standard input change source to stdin
    if (stack == NULL){
        return LEX_ERR;
    }
    StackPrint(stack);


    parser(stack);
    //codegen();


    StackFree(stack);
    fclose(source);

    return 0;
}
