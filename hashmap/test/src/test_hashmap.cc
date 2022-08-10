#include <gtest/gtest.h>
#include <stdbool.h>

extern "C"
{
    #include "hashmap.h"
}

pair_t *init_test_pair(char first, int second)
{
    pair_t *pair = init_pair_values(
        sizeof(char), sizeof(int), (void *) &first, (void *) &second
    );
    return pair;
}

::testing::AssertionResult expect_pair_equal(const pair_t a, const pair_t b)
{
    EXPECT_EQ(a.first_size, b.first_size);
    EXPECT_EQ(a.second_size, b.second_size);
    if (
        !memcmp(a.first, b.first, a.first_size)
        and !memcmp(a.second, b.second, a.second_size)
    ) {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure() << "pairs unequal: " << *(char *) a.first << " "
           << *(int *) a.second << " " << *(char *) b.first << " " << *(int *) b.second;
}

::testing::AssertionResult expect_pair_is_copy(const pair_t *a, const pair_t *b)
{
    EXPECT_TRUE(expect_pair_equal(*a, *b));
    EXPECT_NE(a, b);
    EXPECT_NE(a->first, b->first);
    EXPECT_NE(a->second, b->second);
    return ::testing::AssertionSuccess();
}

size_t hash_char(const void *key, size_t hashmap_size)
{
    return (size_t) *(char *) key % hashmap_size;
}

TEST(HashCharTest, TestHash)
{
    char key = 'h'; // 'h' -> 104
    EXPECT_EQ(hash_char((void *) &key, 4), 0); // 104 % 4 = 0
    EXPECT_EQ(hash_char((void *) &key, 16), 8); // 104 % 16 = 8
}

class PairTestSuite : public ::testing::Test
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

TEST_F(PairTestSuite, TestInit)
{
    pair->first;
    pair->second;
    EXPECT_EQ(pair->first_size, sizeof(char));
    EXPECT_EQ(pair->second_size, sizeof(int));
}

TEST_F(PairTestSuite, TestValueConstructor)
{
    char first = 'h';
    int second = 42;

    pair_t *value_pair = init_pair_values(
        sizeof(first),
        sizeof(second),
        (void *) &first,
        (void *) &second
    );

    EXPECT_EQ(value_pair->first_size, sizeof(char));
    EXPECT_EQ(value_pair->second_size, sizeof(int));
    EXPECT_EQ(*(char *) value_pair->first, 'h');
    EXPECT_EQ(*(int *) value_pair->second, 42);

    free_pair(value_pair);
}

TEST_F(PairTestSuite, TestInsertFirst)
{
    char first_value = 'h';

    pair_insert_first(pair, (void *) &first_value);

    EXPECT_EQ(*(char *) pair->first, 'h');
}

TEST_F(PairTestSuite, TestInsertSecond)
{
    int second_value = 42;

    pair_insert_second(pair, (void *) &second_value);

    EXPECT_EQ(*(int *) pair->second, 42);
}

TEST_F(PairTestSuite, TestInsert)
{
    char first_value = 'h';
    int second_value = 42;

    pair_insert(pair, (void *) &first_value, (void *) &second_value);

    EXPECT_EQ(*(char *) pair->first, 'h');
    EXPECT_EQ(* (int *) pair->second, 42);
}

TEST_F(PairTestSuite, TestCopyPair)
{
    pair_t *copy = init_pair(sizeof(char), sizeof(int));
    char key = 'h';
    int value = 5;
    pair_insert(pair, (void *) &key, (void *) &value);

    copy_pair(copy, pair);

    EXPECT_TRUE(expect_pair_is_copy(copy, pair));

    free_pair(copy);
}

TEST_F(PairTestSuite, TestInitFreeArray)
{
    pair_t *pairs = init_pair_array(2, sizeof(char), sizeof(int));
    char key0 = 'h';
    int value0 = 1;
    char key1 = 'e';
    int value1 = 2;
    pair_insert(pairs, (void *) &key0, (void *) &value0);
    pair_insert(pairs + 1, (void *) &key1, (void *) &value1);

    EXPECT_EQ(*(char *) pairs[0].first, 'h');
    EXPECT_EQ(*(int *) pairs[0].second, 1);
    EXPECT_EQ(*(char *) pairs[1].first, 'e');
    EXPECT_EQ(*(int *) pairs[1].second, 2);
    free_pair_array(pairs, 2);
}

class BucketTestSuite : public ::testing::Test
{
    public:
        bucket_t *bucket;

        void SetUp()
        {
            bucket = init_bucket(sizeof(char), sizeof(int));
        }

        void TearDown()
        {
            free_bucket(bucket);
        }
};

TEST_F(BucketTestSuite, TestInit)
{
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 0);
}

TEST_F(BucketTestSuite, TestInsert)
{
    pair_t *pair0 = init_test_pair('a', 2);

    bucket_insert(bucket, pair0, sizeof(char), sizeof(int));

    EXPECT_TRUE(expect_pair_is_copy(bucket->pairs_, pair0));
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);

    free_pair(pair0);
}

TEST_F(BucketTestSuite, TestInsertMultiple)
{
    pair_t *pair0 = init_test_pair('a', 2);
    pair_t *pair1 = init_test_pair('b', 3);
    pair_t *pair2 = init_test_pair('b', 4);

    bucket_insert(bucket, pair0, sizeof(char), sizeof(int));

    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);

    bucket_insert(bucket, pair1, sizeof(char), sizeof(int));
    EXPECT_EQ(bucket->size, 2);
    EXPECT_EQ(bucket->count, 2);

    EXPECT_TRUE(expect_pair_is_copy(bucket->pairs_, pair0));
    EXPECT_TRUE(expect_pair_is_copy(&bucket->pairs_[1], pair1));
    
    bucket_insert(bucket, pair2, sizeof(char), sizeof(int));

    EXPECT_EQ(bucket->size, 2);
    EXPECT_EQ(bucket->count, 2);

    EXPECT_TRUE(expect_pair_is_copy(&bucket->pairs_[1], pair2));

    free_pair(pair0);
    free_pair(pair1);
    free_pair(pair2);
}

TEST_F(BucketTestSuite, TestGet)
{
    int ret;
    char invalid_key = 'c';
    char key0 = 'a';
    char key1 = 'b';
    pair_t *pair0 = init_test_pair(key0, 2);
    pair_t *pair1 = init_test_pair(key1, 3);

    bucket_insert(bucket, pair0, sizeof(char), sizeof(int));
    bucket_insert(bucket, pair1, sizeof(char), sizeof(int));
    
    EXPECT_EQ(bucket_get(bucket, (void *) &key0, sizeof(char), sizeof(int), (void *) &ret), 0);
    EXPECT_EQ(ret, 2);

    EXPECT_EQ(bucket_get(bucket, (void *) &key1, sizeof(char), sizeof(int), (void *) &ret), 0);
    EXPECT_EQ(ret, 3);

    EXPECT_EQ(bucket_get(bucket, (void *) &invalid_key, sizeof(char), sizeof(int), (void *) &ret), 1);
    EXPECT_EQ(ret, 3);

    free_pair(pair0);
    free_pair(pair1);
}

TEST_F(BucketTestSuite, TestInitFreeBucketArray)
{
    pair_t *pair0 = init_test_pair('h', 1);
    pair_t *pair1 = init_test_pair('e', 2);
    pair_t *pair2 = init_test_pair('l', 3);

    bucket_t *buckets = init_bucket_array(2, sizeof(char), sizeof(int));
    bucket_insert(&buckets[0], pair0, sizeof(char), sizeof(int));
    bucket_insert(&buckets[0], pair1, sizeof(char), sizeof(int));
    bucket_insert(&buckets[1], pair2, sizeof(char), sizeof(int));

    EXPECT_TRUE(expect_pair_is_copy(&buckets[0].pairs_[0], pair0));
    EXPECT_TRUE(expect_pair_is_copy(&buckets[0].pairs_[1], pair1));
    EXPECT_TRUE(expect_pair_is_copy(&buckets[1].pairs_[0], pair2));

    free_bucket_array(buckets, 2);

    free_pair(pair0);
    free_pair(pair1);
    free_pair(pair2);
}

class HashmapTestSuite : public ::testing::Test
{
    public:
        hashmap_t *hashmap;

        void SetUp()
        {
            hashmap = init_hashmap(sizeof(char), sizeof(int), hash_char);
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST_F(HashmapTestSuite, TestInit)
{
    EXPECT_EQ(hashmap->size_, 8);
    EXPECT_EQ(hashmap->count, 0);
    EXPECT_EQ(hashmap->key_size, sizeof(char));
    EXPECT_EQ(hashmap->value_size, sizeof(int));
    EXPECT_EQ(hashmap->hashfunc, hash_char);
}

TEST_F(HashmapTestSuite, TestInsert)
{
    char key0 = 'h';
    int value0 = 1;
    char key1 = 'e';
    int value1 = 2;

    hashmap_insert(hashmap, (void *) &key0, (void *) &value0);
    hashmap_insert(hashmap, (void *) &key1, (void *) &value1);
    
    EXPECT_EQ(*(char *) hashmap->buckets_[0].pairs_[0].first, key0);
    EXPECT_EQ(*(int *) hashmap->buckets_[0].pairs_[0].second, value0);
    EXPECT_EQ(*(char *) hashmap->buckets_[5].pairs_[0].first, key1);
    EXPECT_EQ(*(int *) hashmap->buckets_[5].pairs_[0].second, value1);
}

TEST_F(HashmapTestSuite, TestGet)
{
    int ret;
    char invalid_key = 'l';
    char key0 = 'h';
    int value0 = 1;
    char key1 = 'e';
    int value1 = 2;
    hashmap_insert(hashmap, (void *) &key0, (void *) &value0);
    hashmap_insert(hashmap, (void *) &key1, (void *) &value1);

    EXPECT_EQ(hashmap_get(hashmap, (void *) &key0, (void *) &ret), 0);
    EXPECT_EQ(ret, 1);
    EXPECT_EQ(hashmap_get(hashmap, (void *) &key1, (void *) &ret), 0);
    EXPECT_EQ(ret, 2);
    EXPECT_EQ(hashmap_get(hashmap, (void *) &invalid_key, &ret), 1);
    EXPECT_EQ(ret, 2);
}
