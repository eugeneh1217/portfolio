#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int *array = (int *) malloc(sizeof(int));
    array[0] = 5;
    if (argc == 2 && !strcmp(argv[1], "-r"))
    {
        array = (int *) realloc(array, sizeof(int) * 100000);
    }
    else if (argc == 1)
    {
        printf(
            "expect segfault due to collision "
            "with other allocated memory in heap.\n"
        );
    }
    else
    {
        printf(
            "usage: ./dynamic_array [-r]"
            "\n\t-r flag indicates run with reallocation. "
            "Defaults to running without reallocation.\n"
        );
        return 1;
    }
    array[100000] = 10;
    printf("array[0] = %d\narray[10000] = %d\n", array[0], array[100000]);
    
    return 0;
}
