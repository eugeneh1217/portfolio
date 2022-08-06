#ifndef BUCKET_H
#define BUCKET_H

#include <stdlib.h>
#include <string.h>

struct bucket_t;
struct pair_t;

typedef int (equality_func_t(void *, void *));

typedef struct pair_t 
{
    void *first;
    void *second;
} pair_t;

typedef struct bucket_t
{
    size_t size;
    size_t count;
    size_t key_size;
    size_t value_size;
    pair_t *data_;
} bucket_t;

bucket_t *init_bucket(size_t, size_t);
void free_bucket(bucket_t *bucket);

void insert_to_bucket(bucket_t *bucket, pair_t value);
int get_from_bucket(bucket_t *bucket, void *key, void *ret);

#endif
