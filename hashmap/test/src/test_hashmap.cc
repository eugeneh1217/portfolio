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
            hashmap = init_hashmap(TEST_KEY_SIZE, TEST_VALUE_SIZE, hash_char);
        }

        void TearDown()
        {
            free_hashmap(hashmap);
        }
};

TEST_F(HashmapTests, TestInit)
{
    EXPECT_EQ(hashmap->count, 0);
    EXPECT_EQ(hashmap->size, HASHMAP_INIT_SIZE);
    EXPECT_EQ(hashmap->hash, hash_char);
}

TEST_F(HashmapTests, TestInsert)
{
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap,  &key,  &value);

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
        hashmap_insert(hashmap,  &keys[i],  &values[i]);
        EXPECT_EQ(hashmap->count, i + 1);
    }

    for (int i = 0; i < ELEMENT_COUNT; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap,  &keys[i],  &ret), SUCCESS);
        EXPECT_EQ(ret, values[i]);
    }

}

TEST_F(HashmapTests, TestGet)
{
    int ret = 0;
    char invalid_key = 'o';
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap,  &key,  &value);

    EXPECT_EQ(hashmap_get(hashmap,  &key,  &ret), SUCCESS);
    EXPECT_EQ(ret, 42);

    EXPECT_EQ(hashmap_get(hashmap,  &invalid_key,  &ret), KEY_NOT_FOUND_ERR);
    EXPECT_EQ(ret, 42);
}

TEST_F(HashmapTests, TestDelete)
{
    int ret;
    char key = 'f';
    int value = 42;
    hashmap_insert(hashmap,  &key,  &value);

    hashmap_delete(hashmap,  &key);

    EXPECT_EQ(hashmap_get(hashmap, &key, &ret), KEY_NOT_FOUND_ERR);
    EXPECT_EQ(hashmap->count, 0);
}

TEST_F(HashmapTests, TestDeleteSameHash)
{
    int ret;
    char keys[3] = {'P', 'X', '`'}; // ascii 80, 88, 96
    int values[3] = {42, 24, 1};

    for (int i = 0; i < 3; ++ i)
    {
        hashmap_insert(hashmap, &keys[i], &values[i]);
    }

    hashmap_delete(hashmap, &keys[1]);

    EXPECT_EQ(hashmap_get(hashmap, &keys[0], &ret), SUCCESS);
    EXPECT_EQ(hashmap_get(hashmap, &keys[2], &ret), SUCCESS);
}

TEST_F(HashmapTests, TestLoadBalancingGrow)
{
    int ret;
    char key = 'a';

    // insert elements before rebalancing
    for (int i = 0; i < 6; ++ i)
    {
        hashmap_insert(hashmap,  &key,  &i);
        ++key;

        EXPECT_EQ(hashmap->size, HASHMAP_INIT_SIZE);
        EXPECT_EQ(hashmap->count, i + 1);
    }

    // expect rebalance
    int value = 6;

    hashmap_insert(hashmap,  &key,  &value);

    EXPECT_EQ(hashmap->size, 16);
    EXPECT_EQ(hashmap->count, 7);

    key = 'a';

    for (int i = 0; i < 7; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap,  &key, &ret), SUCCESS);
        ++key;
        EXPECT_EQ(ret, i);
    }
}

TEST_F(HashmapTests, TestLoadBalancingShrink)
{
    int ret;
    char key = 'a';

    for (int i = 0; i < 7; ++ i)
    {
        hashmap_insert(hashmap,  &key,  &i);
        ++ key;
        EXPECT_EQ(hashmap->count, i + 1);
    }

    // delete before shrink
    key = 'a';

    for (int i = 0; i < 3; ++ i)
    {
        hashmap_delete(hashmap, &key);
        ++ key;

        EXPECT_EQ(hashmap->size, 16);
        EXPECT_EQ(hashmap->count, 7 - i - 1);
    }

    // delete after shrink
    hashmap_delete(hashmap, &key);
    EXPECT_EQ(hashmap->size, HASHMAP_INIT_SIZE);

    key = 'a';

    for (int i = 0; i < 4; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap, &key, &ret), KEY_NOT_FOUND_ERR);
        ++ key;
    }

    for (int i = 4; i < 7; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap, &key, &ret), SUCCESS);
        ++ key;
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
        hashmap_insert(hashmap,  &key,  &i);
        EXPECT_EQ(get_bucket_with(hashmap, key), (1 + i) % hashmap->size);
        ++key;
    }
}

TEST_F(HashmapTests, TestClear)
{
    int ret;
    char keys[11] = {'h', 'e', 'l',  'l', '0', ' ', 'w', 'o', 'r', 'l', 'd'};
    int values[11] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    for (int i = 0; i < 11; ++ i)
    {
        hashmap_insert(hashmap, &keys[i], &values[i]);
    }

    hashmap_clear(hashmap);

    EXPECT_EQ(hashmap->count, 0);
    for (int i = 0; i < 11; ++ i)
    {
        EXPECT_EQ(hashmap_get(hashmap, &keys[i], &ret), KEY_NOT_FOUND_ERR);
    }
}
