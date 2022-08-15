#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

// TODO: add github workflow for todo and test passing

typedef enum STATUS_T
{
    SUCCESS=0,
    KEY_NOT_FOUND=1
} STATUS_T;

typedef struct item_t item_t;

typedef struct item_t
{
    item_t *prev;
    item_t *next;
    void *k;
    void *v;
} item_t;

typedef struct hashmap_t
{
    size_t k_size;
    size_t v_size;
    size_t count;
    size_t size;
    item_t *items;
} hashmap_t;

hashmap_t *init_hashmap(size_t, size_t);
void free_hashmap(hashmap_t *);

void hashmap_insert(hashmap_t *, const void *, const void *);
STATUS_T hashmap_get(hashmap_t *, const void *, void *);
void hashmap_delete(hashmap_t *, const void *);


#endif
