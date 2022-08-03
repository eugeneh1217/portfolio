#include <gtest/gtest.h>

extern "C"
{
    #include "hashmap.h"
}

class HashMapTest: public ::testing::Test
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

TEST_F(HashMapTest, TestInit)
{
    hashmap->size = 0;
}


