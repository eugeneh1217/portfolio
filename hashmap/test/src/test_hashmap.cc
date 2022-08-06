#include <gtest/gtest.h>
#include <stdbool.h>

extern "C"
{
    #include "hashmap.h"
}

class HashMapTest : public ::testing::Test
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

