#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct boolint_t
{
    bool b;
    int i;
};

bool *get_bool(void *boolint)
{
    return (bool *) boolint;
}

int *get_int(void *boolint)
{
    return (int *) ( ((bool *) boolint ) + 1 );
}

int main()
{
    printf("Size of Struct:\n");
    printf("\tsizeof(bool) = %ld\n", sizeof(bool));
    printf("\tsizeof(int) = %ld\n", sizeof(int));
    printf("\tsizeof(struct boolint) = %ld\n", sizeof(struct boolint_t));

    void *boolint = malloc(sizeof(bool) + sizeof(int));
    *get_bool(boolint) = true;
    *get_int(boolint) = 5;
    printf("Custom Memory Management with Void Pointer Casts\n");
    printf("\t5 bytes of memory allocated (sizeof(bool) + sizeof(int))\n");
    printf("\tint of void pointer should be 5: %d\n", *get_int(boolint));
    printf("\tbool of void pointer should be 1: %d\n", *get_bool(boolint));
    free(boolint);
    return 0;
}
