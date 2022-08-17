#include <stdio.h>
#include <time.h>

#include "hashmap.h"

#define KEY_SIZE sizeof(int)
#define VALUE_SIZE sizeof(float)

size_t hash_int(const void *key, size_t size)
{
    return *(int *) key % size;
}

int main()
{
    system("clear");
    printf("HASHMAP DEMO\n\n");

    hashmap_t *map = init_hashmap(KEY_SIZE, VALUE_SIZE, hash_int);

    clock_t time;
    double elapsed;
    int ret;

    int element_count = 10000;
    printf("Element Insertion:\n");
    for (int i = 0; i < 10; ++ i)
    {
        time = clock();
        for (int i = 0; i < element_count; ++ i)
        {
            hashmap_insert(map, &i, &i);
        }

        time = clock() - time;
        elapsed = (double) time / CLOCKS_PER_SEC;

        printf("\tTime to insert %d elements: %f, time / element: %lf\n",
               element_count, elapsed, elapsed / element_count);
        element_count *= 2;
        hashmap_clear(map);
    }

    element_count = 10000;
    printf("Element Access:\n");
    for (int i = 0; i < 10; ++ i)
    {
        time = clock();
        for (int i = 0; i < element_count; ++ i)
        {
            hashmap_insert(map, &i, &i);
        }
        for (int i = 0; i < element_count; ++ i)
        {
            hashmap_get(map, &i, &ret);
        }

        time = clock() - time;
        elapsed = (double) time / CLOCKS_PER_SEC;

        printf("\tTime to access %d elements: %f, time / element: %lf\n",
               element_count, elapsed, elapsed / element_count);
        element_count *= 2;
        hashmap_clear(map);
    }

    element_count = 10000;
    printf("Element Deletion:\n");
    for (int i = 0; i < 10; ++ i)
    {
        time = clock();
        for (int i = 0; i < element_count; ++ i)
        {
            hashmap_insert(map, &i, &i);
        }
        for (int i = 0; i < element_count; ++ i)
        {
            hashmap_delete(map, &i);
        }

        time = clock() - time;
        elapsed = (double) time / CLOCKS_PER_SEC;

        printf("\tTime to delete %d elements: %f, time / element: %lf\n",
               element_count, elapsed, elapsed / element_count);
        element_count *= 2;
        hashmap_clear(map);
    }

    printf("DONE\n");

    free_hashmap(map);
}
