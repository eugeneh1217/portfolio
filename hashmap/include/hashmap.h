#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

// TODO: add hashmap load balancing
// TODO: add github workflow for todo and test passing

typedef enum ERRORS
{
    SUCCESS=0,
    KEY_NOT_FOUND_ERR=1
} ERRORS;

typedef size_t (*hashfunc_t)(const void *, size_t);

struct pair_t;
struct bucket_t;
struct hashmap_t;

typedef struct pair_t
{
    void *first;
    void *second;
} pair_t;

typedef struct bucket_t
{
    size_t size;
    size_t count;
    pair_t *pairs_;
} bucket_t;

typedef struct hashmap_t
{
    size_t size_;
    size_t count;
    size_t key_size;
    size_t value_size;
    hashfunc_t hashfunc;
    bucket_t *buckets_;
} hashmap_t;

// pair memory

pair_t *init_pair(size_t first_size, size_t second_size);
pair_t *init_pair_values(size_t first_size, size_t second_size,
                         const void *first_value, const void *second_value);
pair_t *init_pair_array(size_t count, size_t first_size, size_t second_size);
void free_pair(pair_t *pair);
void free_pair_array(pair_t *pair_array, size_t count);

// pair data

void pair_insert_first(pair_t *pair, const void *value, size_t first_size);
void pair_insert_second(pair_t *pair, const void *value, size_t second_size);
void pair_insert(pair_t *pair,
                 const void *first_value, const void *second_value,
                 size_t first_size, size_t second_size);
void copy_pair(pair_t *dest, const pair_t *original,
               size_t first_size, size_t second_size);

// bucket memory

bucket_t *init_bucket(size_t key_size, size_t value_size);
bucket_t *init_bucket_array(size_t count, size_t key_size, size_t value_size);
void free_bucket(bucket_t *bucket);
void free_bucket_array(bucket_t *buckets, size_t count);

// bucket data

void bucket_insert(bucket_t *bucket, const pair_t *pair,
                   size_t key_size, size_t value_size);
int bucket_get(bucket_t *bucket, const void *key,
               size_t key_size, size_t value_size, void *ret);
void bucket_delete(bucket_t *bucket, const void *key,
                   size_t key_size, size_t value_size);

// hashmap memory

hashmap_t *init_hashmap(size_t key_size, size_t value_size,
                        hashfunc_t hashfunc);
void free_hashmap(hashmap_t *hashmap);

// hashmap data

void hashmap_insert(hashmap_t *hashmap, const void *key, const void *value);
int hashmap_get(hashmap_t *hashmap, const void *key, void *ret);
void hashmap_delete(hashmap_t *hashmap, const void *key);

#endif
