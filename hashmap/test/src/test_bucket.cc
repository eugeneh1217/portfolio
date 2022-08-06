#include <gtest/gtest.h>
extern "C"
{
    #include "bucket.h"
}

::testing::AssertionResult expect_pair_eq(pair_t a, pair_t b)
{
    if (memcmp(a.first, b.first, sizeof(int)) == 0 && memcmp(a.second, b.second, sizeof(int)) == 0)
    {
        return ::testing::AssertionSuccess();
    }
    return ::testing::AssertionFailure();
}

class BucketTest : public ::testing::Test
{
    public:
        bucket_t *bucket;

        void SetUp()
        {
            bucket = init_bucket(sizeof(int), sizeof(int));
        }

        void TearDown()
        {
            free_bucket(bucket);
        }
};

TEST_F(BucketTest, test_contructor)
{
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 0);
    EXPECT_EQ(bucket->key_size, sizeof(int));
    EXPECT_EQ(bucket->value_size, sizeof(int));
}

TEST_F(BucketTest, test_insert)
{
    int a = 5;
    int b = 15;
    pair_t pair = {
        .first = (void *) &a,
        .second = (void *) &b
    };
    insert_to_bucket(bucket, pair);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[0], pair));
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);
}

TEST_F(BucketTest, test_insert_multiple)
{
    int a = 5;
    int b = 15;
    int c = 2;
    int d = 12;
    int e = 1;
    int f = 11;
    pair_t pair0 = {
        .first = (void *) &a,
        .second = (void *) &b
    };
    pair_t pair1 = {
        .first = (void *) &c,
        .second = (void *) &d
    };
    pair_t pair2 = {
        .first = (void *) &e,
        .second = (void *) &f
    };
    insert_to_bucket(bucket, pair0);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[0], pair0));
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);

    insert_to_bucket(bucket, pair1);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[1], pair1));
    EXPECT_EQ(bucket->size, 2);
    EXPECT_EQ(bucket->count, 2);

    insert_to_bucket(bucket, pair2);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[2], pair2));
    EXPECT_EQ(bucket->size, 4);
    EXPECT_EQ(bucket->count, 3);
}

TEST_F(BucketTest, test_insert_same_key)
{
    int key0 = 1;
    int key1 = 1;
    int value0 = 2;
    int value1 = 3;
    pair_t pair0 = {
        .first = (void *) &key0,
        .second = (void *) &value0
    };
    pair_t pair1 = {
        .first = (void *) &key1,
        .second = (void *) &value1
    };

    insert_to_bucket(bucket, pair0);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[0], pair0));
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);

    insert_to_bucket(bucket, pair1);
    EXPECT_TRUE(expect_pair_eq(bucket->data_[0], pair1));
    EXPECT_EQ(bucket->size, 1);
    EXPECT_EQ(bucket->count, 1);
}

int compare_int(void *first, void *second)
{
    return *(int *) first == *(int *) second;
}

TEST_F(BucketTest, test_get)
{
    int a = 5;
    int b = 15;
    int ret = 0;
    pair_t pair = {
        .first = (void *) &a,
        .second = (void *) &b
    };
    insert_to_bucket(bucket, pair);
    EXPECT_EQ(get_from_bucket(bucket, (void *) &a, (void *) &ret), 0);
    EXPECT_EQ(ret, 15);
    EXPECT_EQ(get_from_bucket(bucket, (void *) &b, (void *) &ret), 1);
    EXPECT_EQ(ret, 0);
}
