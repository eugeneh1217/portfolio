#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>

struct hashmap_t;

typedef struct hashmap_t {
    uint8_t size;
} hashmap_t;

hashmap_t *init_hashmap();

void free_hashmap(hashmap_t *hashmap);

#endif
