#include "hashmap.h"

hashmap_t *init_hashmap()
{
    hashmap_t *hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
    hashmap->size_ = 1;
    hashmap->count = 0;
}

void free_hashmap(hashmap_t *hashmap)
{
    free(hashmap);
}

// pair_t implemetation

pair_t *init_pair(size_t first_size, size_t second_size)
{
    pair_t *pair = (pair_t *) malloc(sizeof(pair_t));
    pair->first_size = first_size;
    pair->second_size = second_size;
    pair->first = malloc(first_size);
    pair->second = malloc(second_size);
}

pair_t *init_pair_values(size_t first_size, size_t second_size, void *first_value, void *second_value)
{
    pair_t *pair = init_pair(first_size, second_size);
    pair_insert(pair, first_value, second_value);
    return pair;
}

void free_pair(pair_t *pair)
{
    free(pair->first);
    free(pair->second);
    free(pair);
}

void pair_insert_first(pair_t *pair, const void *value)
{
    memcpy(pair->first, value, pair->first_size);
}

void pair_insert_second(pair_t *pair, const void *value)
{
    memcpy(pair->second, value, pair->second_size);
}

void pair_insert(pair_t *pair, const void *first_value, const void *second_value)
{
    pair_insert_first(pair, first_value);
    pair_insert_second(pair, second_value);
}

// bucket_t implementations

bucket_t *init_bucket()
{
    bucket_t *bucket = (bucket_t *) malloc(sizeof(bucket_t));
    bucket->size = 1;
    bucket->count = 0;
    return bucket;
}

void free_bucket(bucket_t *bucket)
{
    free(bucket);
}

void bucket_insert(bucket_t *bucket, const pair_t *pair)
{
    
}

