#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct hashmap_t;
struct pair_t;
struct bucket_t;

typedef size_t (*hashfunction_t)(void *, size_t);
typedef bool (*hash_equality_t)(void *, void *);

typedef struct pair_t
{
    void *first;
    void *second;
} pair_t;

typedef struct bucket_t
{
    size_t size;
    pair_t *data_;
} bucket_t;

typedef struct hashmap_t
{
    size_t size;
    size_t count;
    hashfunction_t hash_function;
    hash_equality_t hash_equal;
    bucket_t *data_;
} hashmap_t;

hashmap_t *init_hashmap();
void free_hashmap(hashmap_t *hashmap);

void hashmap_insert(hashmap_t *hashmap, void *key, void *value);
bool hashmap_get(hashmap_t *hashmap, void *key, void *ret);

#endif
