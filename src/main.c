#include "utils.h"
#include "scanner.h"

int main(int argc, char **argv){
    FILE *source = fopen("debug.php", "r");
    Stack *stack = scanner(source);
    StackPrint(stack);
    StackFree(stack);

    return 0;
}
