#include "hashmap.h"

// pair_t implemetation

pair_t *init_pair(size_t first_size, size_t second_size)
{
    pair_t *pair = (pair_t *) malloc(sizeof(pair_t));
    pair->first = malloc(first_size);
    pair->second = malloc(second_size);
}

pair_t *init_pair_values(size_t first_size, size_t second_size, 
                         const void *first_value, const void *second_value)
{
    pair_t *pair = init_pair(first_size, second_size);
    pair_insert(pair, first_value, second_value, first_size, second_size);
    return pair;
}

pair_t *init_pair_array(size_t count, size_t first_size, size_t second_size)
{
    pair_t *pairs = (pair_t *) malloc(count * sizeof(pair_t));
    for (int i = 0; i < count; i ++)
    {
        pairs[i].first = malloc(first_size);
        pairs[i].second = malloc(second_size);
    }
    return pairs;
}

void free_pair(pair_t *pair)
{
    free(pair->first);
    free(pair->second);
    free(pair);
}

void free_pair_array(pair_t *pair_array, size_t count)
{
    for (int i = 0; i < count; ++ i)
    {
        free(pair_array[i].first);
        free(pair_array[i].second);
    }
    free(pair_array);
}

void pair_insert_first(pair_t *pair, const void *value, size_t first_size)
{
    memcpy(pair->first, value, first_size);
}

void pair_insert_second(pair_t *pair, const void *value, size_t second_size)
{
    memcpy(pair->second, value, second_size);
}

void pair_insert(pair_t *pair,
                 const void *first_value, const void *second_value,
                 size_t first_size, size_t second_size)
{
    pair_insert_first(pair, first_value, first_size);
    pair_insert_second(pair, second_value, second_size);
}

void copy_pair(pair_t *dest, const pair_t *original,
               size_t first_size, size_t second_size)
{
    memcpy(dest->first, original->first, first_size);
    memcpy(dest->second, original->second, second_size);
}

// bucket_t implementations

bucket_t *init_bucket(size_t key_size, size_t value_size)
{
    bucket_t *bucket = (bucket_t *) malloc(sizeof(bucket_t));
    bucket->size = 1;
    bucket->count = 0;
    bucket->pairs_ = init_pair(key_size, value_size);
    return bucket;
}

bucket_t *init_bucket_array(size_t count, size_t key_size, size_t value_size)
{
    bucket_t *buckets = (bucket_t *) malloc(count * sizeof(bucket_t));
    for (int i = 0; i < count; ++ i)
    {
        buckets[i].size = 1;
        buckets[i].count = 0;
        buckets[i].pairs_ = init_pair(key_size, value_size);
    }
    return buckets;
}

void free_bucket(bucket_t *bucket)
{
    free_pair_array(bucket->pairs_, bucket->size);
    free(bucket);
}

void free_bucket_array(bucket_t *buckets, size_t count)
{
    for (int i = 0; i < count; ++ i)
    {
        free_pair_array(buckets[i].pairs_, buckets[i].size);
    }
    free(buckets);
}

void bucket_insert(bucket_t *bucket, const pair_t *pair,
                   size_t key_size, size_t value_size)
{
    for (int i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->pairs_[i].first, pair->first, key_size))
        {
            pair_insert_second(&bucket->pairs_[i], pair->second, value_size);
            return;
        }
    }
    pair_t *temp = bucket->pairs_;
    if (bucket->count == bucket->size)
    {
        bucket->pairs_ = init_pair_array(bucket->size * 2,
                                         key_size, value_size);
        for (int i = 0; i < bucket->count; i ++)
        {
            copy_pair(&bucket->pairs_[i], &temp[i], key_size, value_size);
        }
        free_pair_array(temp, bucket->size);
        bucket->size *= 2;
    }
    copy_pair(&bucket->pairs_[bucket->count ++], pair, key_size, value_size);
}

int bucket_get(bucket_t *bucket, const void *key,
               size_t key_size, size_t value_size, void *ret)
{
    for (int i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->pairs_[i].first, key, key_size))
        {
            memcpy(ret, bucket->pairs_[i].second, value_size);
            return SUCCESS;
        }
    }
    return KEY_NOT_FOUND_ERR;
}

// hashmap_t implementation

hashmap_t *init_hashmap(size_t key_size, size_t value_size, hashfunc_t hashfunc)
{
    hashmap_t *hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
    hashmap->size_ = 8;
    hashmap->count = 0;
    hashmap->key_size = key_size;
    hashmap->value_size = value_size;
    hashmap->hashfunc = hashfunc;
    hashmap->buckets_ = init_bucket_array(hashmap->size_, key_size, value_size);
    return hashmap;
}

void free_hashmap(hashmap_t *hashmap)
{
    free_bucket_array(hashmap->buckets_, hashmap->size_);
    free(hashmap);
}

void hashmap_insert(hashmap_t *hashmap, const void *key, const void *value)
{
    size_t index = hashmap->hashfunc(key, hashmap->size_);
    pair_t *pair = init_pair_values(hashmap->key_size, hashmap->value_size,
                                    key, value);
    bucket_insert(&hashmap->buckets_[index], pair,
                  hashmap->key_size, hashmap->value_size);
    free_pair(pair);
}

int hashmap_get(hashmap_t *hashmap, const void *key, void *ret)
{
    for (int i = 0; i < hashmap->size_; ++ i)
    {
        if (!bucket_get(&hashmap->buckets_[i], key, hashmap->key_size, hashmap->value_size, ret))
        {
            return SUCCESS;
        }
    }
    return KEY_NOT_FOUND_ERR;
}
