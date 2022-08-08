#include <gtest/gtest.h>
#include <stdbool.h>

extern "C"
{
    #include "hashmap.h"
}

class HashmapTest : public ::testing::Test
{
    public:
        hashmap_t *hashmap;

        void SetUp()
        {
            hashmap = init_hashmap();
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST_F(HashmapTest, TestInit)
{
    EXPECT_EQ(hashmap->size_, 1);
    EXPECT_EQ(hashmap->count, 0);
}

class PairTest : public ::testing::Test
{
    public:
        pair_t *pair;

        void SetUp()
        {
            pair = init_pair(sizeof(char), sizeof(int));
        }

        void TearDown()
        {
            free_pair(pair);
        }
};

TEST_F(PairTest, TestInit)
{
    pair->first;
    pair->second;
    EXPECT_EQ(pair->first_size, sizeof(char));
    EXPECT_EQ(pair->second_size, sizeof(int));
}

TEST_F(PairTest, TestValueConstructor)
{
    char first = 'h';
    int second = 42;
    pair_t *value_pair = init_pair_values(
        sizeof(first),
        sizeof(second),
        (void *) &first,
        (void *) &second
    );
    free_pair(value_pair);
}

TEST_F(PairTest, TestInsertFirst)
{
    char first_value = 'h';
    pair_insert_first(pair, (void *) &first_value);
    EXPECT_EQ(*(char *) pair->first, 'h');
}

TEST_F(PairTest, TestInsertSecond)
{
    int second_value = 42;
    pair_insert_second(pair, (void *) &second_value);
    EXPECT_EQ(*(int *) pair->second, 42);
}

TEST_F(PairTest, TestInsert)
{
    char first_value = 'h';
    int second_value = 42;
    pair_insert(pair, (void *) &first_value, (void *) &second_value);
    EXPECT_EQ(*(char *) pair->first, 'h');
    EXPECT_EQ(* (int *) pair->second, 42);
}

class BucketTest : public ::testing::Test
{
    public:
        bucket_t *bucket;

        void SetUp()
        {
            bucket = init_bucket();
        }

        void TearDown()
        {
            free_bucket(bucket);
        }
};

TEST_F(BucketTest, TestInit)
{
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 0);
}

TEST_F(BucketTest, TestInsert)
{
    char key0 = 'a';
    int value0 = 2;
    pair_t *pair0 = init_pair_values(
        sizeof(char),
        sizeof(int),
        (void *) &key0,
        (void *) &value0
    );
    bucket_insert(bucket, pair0);
    free_pair(pair0);
}
