#ifndef HASHMAP_H
#define HASHMAP_H

#include <string.h>

struct hashmap_t;

typedef struct hashmap_t
{

} hashmap_t;

hashmap_t *init_hashmap();

void free_hashmap(hashmap_t *hashmap);

#endif
