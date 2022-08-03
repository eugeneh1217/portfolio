#include "hashmap.h"

hashmap_t *init_hashmap()
{
    hashmap_t *hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
    hashmap->size = 0;
    return hashmap;
}

void free_hashmap(hashmap_t *hashmap)
{
    free(hashmap);
}
