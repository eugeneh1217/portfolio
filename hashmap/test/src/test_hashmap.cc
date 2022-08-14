#include <gtest/gtest.h>

extern "C"
{
    #include "hashmap.h"
}

class HashmapTests : public ::testing::Test
{
    public:
        size_t TEST_KEY_SIZE = sizeof(char);
        size_t TEST_VALUE_SIZE = sizeof(int);
        hashmap_t *hashmap;

        void SetUp()
        {
            hashmap = init_hashmap(TEST_KEY_SIZE, TEST_VALUE_SIZE);
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST_F(HashmapTests, TestInsert)
{
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap, (void *) &key, (void *) &value);

    EXPECT_EQ(hashmap->count, 1);
}

TEST_F(HashmapTests, TestInsertMultiple)
{
    int ret;
    int ELEMENT_COUNT = 6;
    char keys[ELEMENT_COUNT] = {'f', 'o', 'i', 'b', 'a', 'z'};
    int values[ELEMENT_COUNT] = {3, 1, 4, 1, 5, 9};

    for (int i = 0; i < ELEMENT_COUNT; ++ i)
    {
        hashmap_insert(hashmap, (void *) &keys[i], (void *) &values[i]);
        EXPECT_EQ(hashmap->count, i + 1);
    }

    for (int i = 0; i < ELEMENT_COUNT; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap, (void *) &keys[i], (void *) &ret), 0);
        EXPECT_EQ(ret, values[i]);
    }

}

TEST_F(HashmapTests, TestGet)
{
    int ret = 0;
    char invalid_key = 'o';
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap, (void *) &key, (void *) &value);

    EXPECT_EQ(hashmap_get(hashmap, (void *) &key, (void *) &ret), 0);
    EXPECT_EQ(ret, 42);

    EXPECT_EQ(hashmap_get(hashmap, (void *) &invalid_key, (void *) &ret), 1);
    EXPECT_EQ(ret, 42);
}

TEST_F(HashmapTests, TestDelete)
{
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap, (void *) &key, (void *) &value);

    hashmap_delete(hashmap, (void *) &key);
}

TEST_F(HashmapTests, TestLoadBalancing)
{
    char key = 'a';
    for (int i = 0; i < 7; ++ i)
    {
        hashmap_insert(hashmap, (void *) &key, (void *) &i);
        ++key;
    }
}
