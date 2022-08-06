#include "hashmap.h"
#include <stdio.h>

// void insert_at_bucket_(hashmap_t *hashmap, size_t bucket_index, void *key, void *value)
// {
//     // bucket_t *bucket = &hashmap->data_[bucket_index];
//     // // for (int i = 0; i < bucket.size; i ++)
//     // // {
//     // //     if (hashmap->hash_equal(bucket.data_[i].first, key))
//     // //     {
//     // //         bucket.data_[i].second = value;
//     // //         return;
//     // //     }
//     // // }
//     // printf('BUCKET_SIZE: %ld', bucket->size);
//     // // reallocarray(bucket.data_, ++bucket.size, sizeof(pair_t));
//     // // bucket.data_[bucket.size - 1] = (pair_t) {
//     // //     key,
//     // //     value
//     // // };
// }

// bool get_at_bucket_(hashmap_t *hashmap, size_t bucket_index, void *key, void *ret)
// {
//     // bucket_t bucket = hashmap->data_[bucket_index];
//     // for (int i = 0; i < bucket.size; i ++)
//     // {
//     //     if (hashmap->hash_equal(bucket.data_[i].first, key))
//     //     {
//     //         ret = bucket.data_[i].second;
//     //         return true;
//     //     }
//     // }
//     // ret = NULL;
//     // return false;
// }

// void init_bucket(bucket_t *bucket)
// {
//     bucket = (bucket_t *) malloc(sizeof(bucket_t));
//     bucket->size = 0;
//     bucket->data_ = NULL;
// }

// void free_bucket(bucket_t *bucket)
// {
//     // free(bucket->data_);
// }

bucket_t *init_bucket_t()
{
    bucket_t *array = (bucket_t *) malloc(sizeof(bucket_t));
    array->size = 0;
    array->data_ = NULL;
    return array;
}

void free_bucket_t(bucket_t *array)
{
    free(array->data_);
    free(array);
}

hashmap_t *init_hashmap()
{
    hashmap_t *hashmap = (hashmap_t *) malloc(sizeof(hashmap_t));
    hashmap->size = 16;
    hashmap->count = 0;
    hashmap->data_ = (bucket_t *) calloc(hashmap->size, sizeof(bucket_t));
    bucket_t *head = hashmap->data_;
    for (int i = 0; i < hashmap->size; i ++)
    {
        init_bucket(head++);
    }
    return hashmap;
}

void free_hashmap(hashmap_t *hashmap)
{
    for (int i = 0; i < hashmap->size; i ++)
    {
        free_bucket(&hashmap->data_[i]);
    }
    free(hashmap->data_);
    free(hashmap);
}

void hashmap_insert(hashmap_t *hashmap, void *key, void *value)
{
    size_t index = hashmap->hash_function(key, hashmap->size);
    // printf("INSERT %ld\n", index);
    insert_at_bucket_(hashmap, index, key, value);
    ++ hashmap->count;
    // printf(
    //     "AFTER INSERT: %d, %d",
    //     *(int *) hashmap->data_[2].data_[0].first,
    //     *(int *) hashmap->data_[2].data_[0].second);
}

bool hashmap_get(hashmap_t *hashmap, void *key, void *ret)
{
    size_t index = hashmap->hash_function(key, hashmap->size);
    // printf("GET %ld\n", index);
    return get_at_bucket_(hashmap, index, key, ret);
}
