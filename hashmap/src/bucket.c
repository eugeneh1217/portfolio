#include "bucket.h"

bucket_t *init_bucket(size_t key_size, size_t value_size)
{
    bucket_t *bucket = malloc(sizeof(bucket_t));
    bucket->size = 1;
    bucket->count = 0;
    bucket->key_size = key_size;
    bucket->value_size = value_size;
    bucket->data_ = (pair_t *) malloc(sizeof(void *));
    return bucket;
}

void free_bucket(bucket_t *bucket)
{
    free(bucket->data_);
    free(bucket);
}

void insert_to_bucket(bucket_t *bucket, pair_t value)
{
    for (int i = 0; i < bucket->count; ++ i)
    {
        if (memcmp(bucket->data_[i].first, value.first, bucket->key_size) == 0)
        {
            bucket->data_[i].second = value.second;
            return;
        }
    }
    if (bucket->count == bucket->size)
    {
        bucket->size *= 2;
        bucket->data_ = reallocarray(bucket->data_, bucket->size, sizeof(pair_t));
    }
    bucket->data_[bucket->count++] = value;
}

int get_from_bucket(bucket_t *bucket, void *key, void *ret)
{
    for (int i = 0; i < bucket->size; ++ i)
    {
        if (memcmp(bucket->data_[i].first, key, bucket->key_size) == 0)
        {
            memcpy(ret, bucket->data_[i].second, bucket->value_size);
            return 0;
        }
    }
    memset(ret, 0, bucket->value_size);
    return 1;
}
