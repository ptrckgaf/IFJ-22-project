#include "utils.h"
#include "scanner.h"
#include "semantics.h"

int main(int argc, char **argv){

    // warning unused params
    argv[0] = NULL;
    (void)argc;
    ASTstruct *ast;
    FILE *source = fopen("../debug.php", "r");
    Stack *stack = scanner(source); //to read from standard input change source to stdin
    if (stack == NULL){
        return LEX_ERR;
    }
    //StackPrint(stack);


    parser(stack);
    printf("%d",ast->rightNode->value->valueType);
    semantics();

    //codegen();


    StackFree(stack);
    fclose(source);

    return 0;
}
