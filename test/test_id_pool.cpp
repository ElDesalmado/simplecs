#include "simplecs/impl/id_pool.h"

#include <gtest/gtest.h>

TEST(IdPoolTests, NextAvailable)
{
    eld::detail::id_pool<uint32_t> pool{};

    for(uint32_t i = 0; i < 100; ++i)
        EXPECT_EQ(pool.next_available(), i);
}

TEST(IdPoolTests, IsFree)
{
    eld::detail::id_pool<uint32_t> pool{};

    for(uint32_t i = 0; i < 100; ++i)
        EXPECT_TRUE(pool.is_free(i));
}

TEST(IdPoolTests, NextAvailable_IsFree)
{
    eld::detail::id_pool<uint32_t> pool{};

    std::vector<uint32_t> ids;

    for(uint32_t i = 0; i < 10; ++i)
        pool.next_available();

    for(uint32_t i = 0; i < 10; ++i)
        EXPECT_FALSE(pool.is_free(i));
}

TEST(IdPoolTests, NextAvailable_Free_IsFree)
{
    eld::detail::id_pool<uint32_t> pool{};

    std::vector<uint32_t> ids;

    for(uint32_t i = 0; i < 10; ++i)
        ids.push_back(pool.next_available());

    for(uint32_t i = 0; i < 10; ++i)
        pool.free(i);

    for(uint32_t i = 0; i < 10; ++i)
        EXPECT_TRUE(pool.is_free(i));
}

TEST(IdPoolTests, Reserve_IsFree)
{
    eld::detail::id_pool<uint32_t> pool{};

    for(uint32_t i = 0; i < 10; ++i)
        EXPECT_TRUE(pool.reserve(i));

    for(uint32_t i = 0; i < 10; ++i)
        EXPECT_FALSE(pool.is_free(i));
}

TEST(IdPoolTests, Reserve_IsFree_1)
{
    eld::detail::id_pool<uint32_t> pool{};

    EXPECT_TRUE(pool.reserve(100));

    EXPECT_FALSE(pool.is_free(100));

    for(uint32_t i = 0; i < 100; ++i)
        EXPECT_TRUE(pool.is_free(i));
}

TEST(IdPoolTests, Reserve_NextAvailable)
{
    eld::detail::id_pool<uint32_t> pool{};

    EXPECT_TRUE(pool.reserve(100));

    for(uint32_t i = 0; i < 100; ++i)
        EXPECT_EQ(pool.next_available(), i);
}

TEST(IdPoolTests, Reserve_Free_NextAvailable)
{
    eld::detail::id_pool<uint32_t> pool{};

    EXPECT_TRUE(pool.reserve(3));
    EXPECT_TRUE(pool.reserve(5));
    EXPECT_TRUE(pool.reserve(8));

    pool.free(3);
    pool.free(5);
    pool.free(8);

    for(uint32_t i = 0; i < 10; ++i)
        EXPECT_EQ(pool.next_available(), i);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
