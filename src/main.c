#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    FILE *source = fopen("debug.php", "r");
    Stack *stack = scanner(source);
    if (stack == NULL){
        return LEX_ERR;
    }
    StackPrint(stack);
    StackFree(stack);
    fclose(source);

    return 0;
}
