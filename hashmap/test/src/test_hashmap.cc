#include <gtest/gtest.h>

extern "C"
{
    #include "hashmap.h"
}

size_t hash_char(const void *c, size_t size)
{
    return (int) *(char *) c % size;
}

TEST(HashTests, TestHashChar)
{
    char key = 'a';
    EXPECT_EQ(hash_char(&key, 5), 2);
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
            hashmap->hash = hash_char;
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST_F(HashmapTests, TestInit)
{
    EXPECT_EQ(hashmap->count, 0);
    EXPECT_EQ(hashmap->size, 8);
}

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
    int ret;
    char key = 'a';
    // insert elements before rebalancing
    for (int i = 0; i < 6; ++ i)
    {
        hashmap_insert(hashmap, (void *) &key, (void *) &i);
        ++key;
        EXPECT_EQ(hashmap->size, 8);
        EXPECT_EQ(hashmap->count, i + 1);
    }
    // expect rebalance
    int value = 6;
    hashmap_insert(hashmap, (void *) &key, (void *) &value);
    EXPECT_EQ(hashmap->size, 16);
    EXPECT_EQ(hashmap->count, 7);
    key = 'a';
    for (int i = 0; i < 7; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap, (void *) &key, &ret), 0);
        ++key;
        EXPECT_EQ(ret, i);
    }
}

TEST_F(HashmapTests, TestBuckets)
{
    /**
     * read buckets to ensure that they are allocated
     * by jumping on condition that requires bucket reads.
    */
    int a = 0;
    for (int i = 0; i < hashmap->size; ++ i)
    {
        if (*(char *) hashmap->items[i].k == ' ')
        {
            ++ a;
        }
    }
}

bool contains_key(item_t *items, char c)
{
    while (items != nullptr)
    {
        if (*(char *) items->k == c)
        {
            return true;
        }
        items = items->next;
    }
    return false;
}

int get_bucket_with(hashmap_t *map, char key)
{
    for (int b = 0; b < map->size; ++b)
    {
        if (contains_key(&map->items[b], key))
        {
            return b;
        }
    }
    return -1;
}

TEST_F(HashmapTests, TestHashing)
{
    char key = 'a';
    for (int i = 0; i < 6; ++ i)
    {
        hashmap_insert(hashmap, (void *) &key, (void *) &i);
        EXPECT_EQ(get_bucket_with(hashmap, key), (1 + i) % hashmap->size);
        ++key;
    }
}
