#include <gtest/gtest.h>
#include <stdbool.h>

extern "C"
{
    #include "hashmap.h"
}

size_t hash_string(void *string, size_t size)
{
    size_t index = 0;
    char *head = (char *) string;
    while (*head != '\0')
    {
        index += (int) *(head ++);
    }
    return index % size;
}

bool string3_equal(void *first, void *second)
{
    return ! (bool) strcmp((char *) first, (char *) second);
}

class HashMapTest : public ::testing::Test
{
    public:
        hashmap_t *hashmap;

        void SetUp()
        {
            hashmap = init_hashmap();
            hashmap->hash_equal = string3_equal;
            hashmap->hash_function = hash_string;
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST(TestSampleHashFunctions, TestString3EqualTrue)
{
    char a[3] = "hi";
    char b[3] = "hi";
    EXPECT_TRUE(string3_equal((void *) &a, (void *) &b));
}

TEST(TestSampleHashFunctions, TestString3EqualFalse)
{
    char a[3] = "hi";
    char b[3] = "HI";
    EXPECT_FALSE(string3_equal((void *) &a, (void *) &b));
}

TEST(TestSampleHashFunctions, TestHashString)
{
    char a[3] = "hi";
    size_t expected = 104 + 105; // 'h' = 104, 'i' = 105
    EXPECT_EQ(hash_string(a, 3), 2); // hashmap of size 3, expected % 3 = 2
    EXPECT_EQ(hash_string(a, 4), 1); // hashmap of size 4, expected % 4 = 1
}

TEST_F(HashMapTest, TestInit)
{
    EXPECT_EQ(hashmap->size, 16);
    EXPECT_EQ(hashmap->count, 0);
}

TEST_F(HashMapTest, TestIO)
{
    char key0[3] = "aa";
    int val0 = 2;
    int ret;
    hashmap_insert(hashmap, (void *) key0, (void *) &val0);
    EXPECT_EQ(hashmap->count, 1);
    EXPECT_TRUE(hashmap_get(hashmap, (void *) key0, (void *) &ret));
    EXPECT_EQ(ret, 2);

    char key1[3] = "ab";
    int val1 = 4;
    hashmap_insert(hashmap, (void *) key1, (void *) &val1);
    EXPECT_EQ(hashmap->count, 2);
    EXPECT_TRUE(hashmap_get(hashmap, (void *) key1, (void *) &ret));
    EXPECT_EQ(ret, 4);


    int val2 = 6;
    hashmap_insert(hashmap, (void *) key0, (void *) &val2);
    EXPECT_EQ(hashmap->count, 3);
    EXPECT_TRUE(hashmap_get(hashmap, (void *) key0, (void *) &ret));
    EXPECT_EQ(ret, 6);
}

class DynamicArrayTest : public ::testing::Test
{
    public:
        bucket_t *array;

        void SetUp()
        {
            array = init_bucket();
        }

        void TearDown()
        {
            free_bucket(array);
        }
};

TEST_F(DynamicArrayTest, TestInsert)
{
    pair_t pair = {
        (void *) malloc(sizeof(int)),
        (void *) malloc(sizeof(int))
    };
    *(int *) pair.first = 5;
    *(int *) pair.second = 15;
    array->insert(pair);
}
