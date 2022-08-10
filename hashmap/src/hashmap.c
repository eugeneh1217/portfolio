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

pair_t *init_pair_array(size_t count, size_t first_size, size_t second_size)
{
    pair_t *pairs = (pair_t *) malloc(count * sizeof(pair_t));
    for (int i = 0; i < count; i ++)
    {
        pairs[i].first_size = first_size;
        pairs[i].second_size = second_size;
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

void copy_pair(pair_t *dest, const pair_t *original)
{
    dest->first_size = original->first_size;
    dest->second_size = original->second_size;
    memcpy(dest->first, original->first, original->first_size);
    memcpy(dest->second, original->second, original->second_size);
}

// bucket_t implementations

bucket_t *init_bucket(size_t key_size, size_t value_size)
{
    bucket_t *bucket = (bucket_t *) malloc(sizeof(bucket_t));
    bucket->size = 1;
    bucket->count = 0;
    bucket->data_ = init_pair(key_size, value_size);
    return bucket;
}

void free_bucket(bucket_t *bucket)
{
    free_pair_array(bucket->data_, bucket->size);
    free(bucket);
}

void bucket_insert(bucket_t *bucket, const pair_t *pair)
{
    for (int i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->data_[i].first, pair->first, pair->first_size))
        {
            pair_insert_second(&bucket->data_[i], pair->second);
            return;
        }
    }
    pair_t *temp = bucket->data_;
    if (bucket->count == bucket->size)
    {
        bucket->data_ = init_pair_array(bucket->size * 2, pair->first_size, pair->second_size);
        for (int i = 0; i < bucket->count; i ++)
        {
            copy_pair(&bucket->data_[i], &temp[i]);
        }
        free_pair_array(temp, bucket->size);
        bucket->size *= 2;
    }
    copy_pair(&bucket->data_[bucket->count ++], pair);
}

int bucket_get(bucket_t *bucket, const void *key, void *ret)
{
    for (int i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->data_[i].first, key, bucket->data_[i].first_size))
        {
            memcpy(ret, bucket->data_[i].second, bucket->data_[i].second_size);
            return SUCCESS;
        }
    }
    return KEY_NOT_FOUND_ERR;
}
