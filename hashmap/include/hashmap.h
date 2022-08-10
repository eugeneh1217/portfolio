#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

struct pair_t;
struct bucket_t;
struct hashmap_t;

typedef struct pair_t
{
    size_t first_size;
    size_t second_size;
    void *first;
    void *second;
} pair_t;

typedef struct bucket_t
{
    size_t size;
    size_t count;
    pair_t *data_;
} bucket_t;

typedef struct hashmap_t
{
    size_t size_;
    size_t count;
} hashmap_t;

// pair memory management
pair_t *init_pair(size_t first_size, size_t second_size);
pair_t *init_pair_values(size_t first_size, size_t second_size, void *first_value, void *second_value);
pair_t *init_pair_array(size_t count, size_t first_size, size_t second_size);
void free_pair(pair_t *pair);
void free_pair_array(pair_t *pair_array, size_t count);

// pair data management
void pair_insert_first(pair_t *pair, const void *value);
void pair_insert_second(pair_t *pair, const void *value);
void pair_insert(pair_t *pair, const void *first_value, const void *second_value);
void copy_pair(pair_t *dest, const pair_t *original);

// bucket memory management
bucket_t *init_bucket(size_t key_size, size_t value_size);
void free_bucket(bucket_t *bucket);

// bucket data management
void bucket_insert(bucket_t *bucket, const pair_t *pair);

// hasmap memory management
hashmap_t *init_hashmap();
void free_hashmap(hashmap_t *hashmap);

#endif
