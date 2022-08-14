#include <stdbool.h>

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
    pair_t *pairs = (pair_t *) calloc(count, sizeof(pair_t));
    for (size_t i = 0; i < count; i ++)
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
    for (size_t i = 0; i < count; ++ i)
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
    bucket_t *buckets = (bucket_t *) calloc(count, sizeof(bucket_t));
    for (size_t i = 0; i < count; ++ i)
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
    for (size_t i = 0; i < count; ++ i)
    {
        free_pair_array(buckets[i].pairs_, buckets[i].size);
    }
    free(buckets);
}

void bucket_insert(bucket_t *bucket, const pair_t *pair,
                   size_t key_size, size_t value_size)
{
    for (size_t i = 0; i < bucket->count; ++ i)
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
        for (size_t i = 0; i < bucket->count; i ++)
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
    for (size_t i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->pairs_[i].first, key, key_size))
        {
            memcpy(ret, bucket->pairs_[i].second, value_size);
            return SUCCESS;
        }
    }
    return KEY_NOT_FOUND_ERR;
}

void bucket_delete(bucket_t *bucket, const void *key,
                   size_t key_size, size_t value_size)
{
    size_t index = 0;
    int found = 0;
    for (size_t i = 0; i < bucket->count; ++ i)
    {
        if (!memcmp(bucket->pairs_[i].first, key, key_size))
        {
            index = i;
            found = 1;
            break;
        }
    }
    bucket_t *temp = bucket;
    if (found)
    {
        bucket = (bucket_t *) malloc(sizeof(bucket_t));
        if (temp->count == temp->size / 2 + 1 && temp->size != 1)
        {
            bucket->size = temp->size / 2;
        }
        else
        {
            bucket->size = temp->size;
        }
        bucket->count = temp->count - 1;
        bucket->pairs_ = init_pair_array(bucket->size, key_size, value_size);
        for (size_t i = 0; i < index; ++ i)
        {
            copy_pair(&bucket->pairs_[i], &temp->pairs_[i],
                      key_size, value_size);
        }
        for (size_t i = index + 1; i < temp->count; ++ i)
        {
            copy_pair(&bucket->pairs_[i-1], &temp->pairs_[i],
                      key_size, value_size);
        }
        free_pair_array(temp->pairs_, temp->size);
        memcpy(temp, bucket, sizeof(bucket_t));
        free(bucket);
    }
}

// hashmap_t implementation

// typedef struct key_t
// {
//     bool valid;
//     void *data;
// } key_t;

hashmap_t *init_hashmap(size_t key_size, size_t value_size, hashfunc_t hashfunc)
{
    hashmap_t *hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
    hashmap->size_ = 8;
    hashmap->count = 0;
    hashmap->key_size = key_size;
    hashmap->value_size = value_size;
    hashmap->hashfunc = hashfunc;
    // hashmap->buckets_ = init_bucket_array(hashmap->size_, sizeof(key_t), value_size);
    hashmap->buckets_ = init_bucket_array(hashmap->size_, key_size, value_size);
    return hashmap;
}

void free_hashmap(hashmap_t *hashmap)
{
    free_bucket_array(hashmap->buckets_, hashmap->size_);
    free(hashmap);
}

// void copy_bucket(bucket_t *dest, const bucket_t *original,
//                  size_t key_size, size_t value_size)
// {
//     dest->count = original->count;
//     dest->size = original->size;
//     for (size_t i = 0; i < original->count; ++ i)
//     {
//         copy_pair(&dest->pairs_[i], &original->pairs_[i], key_size, value_size);
//     }
// }

// need to distinguish between empty pairs and 0 pairs
// currently inserting all empty pairs as 0 pairs.
void grow_hashmap(hashmap_t *hashmap)
{
    // bucket_t *temp = hashmap->buckets_;
    // hashmap->size_ *= 2;
    // hashmap->buckets_ = init_bucket_array(hashmap->size_, hashmap->key_size, hashmap->value_size);
    // for (size_t b = 0; b < hashmap->size_ / 2; ++ b)
    // {
    //     for (size_t p = 0; p < temp[b].count; ++ p)
    //     {
    //         hashmap_insert(hashmap, temp[b].pairs_[p].first, temp[b].pairs_[p].second);
    //     }
    // }
    // free_bucket_array(temp, hashmap->size_ / 2);

    // temp point to bucketarray
    // hashmap->buckets_ points to new bucket array 2x size
    // copy buckets from temp to hashmap->buckets_
    // free temp
    // update hashmap->size_
}

void hashmap_insert(hashmap_t *hashmap, const void *key, const void *value)
{
    size_t index = hashmap->hashfunc(key, hashmap->size_);
    // key_t insert_key = {.valid = 1, .data = key};
    // pair_t *pair = init_pair_values(hashmap->key_size, hashmap->value_size,
    //                                 (void *) &insert_key, value);
    pair_t *pair = init_pair_values(hashmap->key_size, hashmap->value_size,
                                    key, value);
    hashmap->count -= hashmap->buckets_[index].count;
    bucket_insert(&hashmap->buckets_[index], pair,
                  hashmap->key_size, hashmap->value_size);
    hashmap->count += hashmap->buckets_[index].count;
    if (hashmap->count > hashmap->size_ * HASHMAP_MAX_LOAD)
    {
        grow_hashmap(hashmap);
    }
    free_pair(pair);
}

int hashmap_get(hashmap_t *hashmap, const void *key, void *ret)
{
    size_t index = hashmap->hashfunc(key, hashmap->key_size);
    // for (size_t i = 0; i < hashmap->size_; ++ i)
    // {
    if (!bucket_get(&hashmap->buckets_[index], key, hashmap->key_size, hashmap->value_size, ret))
    {
        return SUCCESS;
    }
    // }
    return KEY_NOT_FOUND_ERR;
}

void hashmap_delete(hashmap_t *hashmap, const void *key)
{
    size_t index = hashmap->hashfunc(key, hashmap->size_);
    hashmap->count -= hashmap->buckets_[index].count;
    bucket_delete(&hashmap->buckets_[index], key,
                  hashmap->key_size, hashmap->value_size);
    hashmap->count += hashmap->buckets_[index].count;
}
