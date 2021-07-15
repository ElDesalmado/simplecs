
#include "simplecs/generic/selector.h"
#include "simplecs/impl/selector.h"

#include <gtest/gtest.h>
#include <random>

using EntityId = size_t;
using ComponentId = size_t;

using SelectorStd = eld::generic::selector<eld::impl::selector_std<EntityId, ComponentId>>;

using inputType = std::vector<EntityId>;
using selectedType = std::vector<ComponentId>;
using tableType = std::unordered_map<ComponentId, std::vector<EntityId>>;

namespace
{
    unsigned long generateRandom(unsigned long lower_bound, unsigned long upper_bound)
    {
        std::uniform_int_distribution<unsigned long> uniformIntDistribution(lower_bound,
                                                                            upper_bound);
        std::random_device r;
        std::default_random_engine re(r());
        return uniformIntDistribution(re);
    }
}   // namespace

class TestSelectorStd : public testing::Test
{
protected:
    void SetUp() override { Test::SetUp(); }
    void TearDown() override { Test::TearDown(); }
    SelectorStd selector{};
    tableType table{};
    inputType input{};
    selectedType selected{};
};
// TODO: test scenarios and implementations

TEST_F(TestSelectorStd, empty_table)
{
    input = { 4, 8, 15, 16, 23, 42 };

    selected = selector(table, input);
    EXPECT_TRUE(selected.empty());
}

TEST_F(TestSelectorStd, empty_select_input)
{
    table.insert({ 0, { 4, 8, 15 } });
    table.insert({ 1, { 16, 23, 42 } });

    selected = selector(table, input);
    EXPECT_TRUE(selected.empty());
}

TEST_F(TestSelectorStd, single_column)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert({ 0, Ids });
    input.push_back(0);
    selected = selector(table, input);
    EXPECT_EQ(selected, Ids);
}

TEST_F(TestSelectorStd, single_column_empty_result_positive)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert({ 0, Ids });
    input = std::vector<ComponentId>(1000);
    std::iota(input.begin(), input.end(), 1);
    std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });
    selected = selector(table, input);
    EXPECT_EQ(selected, selectedType{});
}

TEST_F(TestSelectorStd, single_column_empty_result_negative)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert({ 0, Ids });
    input = std::vector<ComponentId>(1000);
    std::iota(input.begin(), input.end(), -1000);
    std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });
    selected = selector(table, input);
    EXPECT_EQ(selected, selectedType{});
}

TEST_F(TestSelectorStd, thousand_columns_equal_entt_ids)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });
    for (size_t i = 0; i < 1000; ++i)
        table.insert({ i, Ids });

    input = inputType(1000);
    std::iota(input.begin(), input.end(), 0);
    std::shuffle(input.begin(), input.end(), std::mt19937{ std::random_device{}() });
    selected = selector(table, input);

    EXPECT_EQ(selected, Ids);
}

TEST_F(TestSelectorStd, three_columns_not_equal_entt_ids)
{
    table.insert({ 0, { 0, 1, 2, 3, 4, 5 } });
    table.insert({ 1, { 2, 3, 0, 1, 4, 5 } });
    table.insert({ 2, { 4, 5, 2, 3, 0, 1 } });

    input = { 0, 1, 2 };
    selected = selector(table, input);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, (selectedType{ 0, 1, 2, 3, 4, 5 }));
}

TEST_F(TestSelectorStd, three_columns_extra_numbers)
{
    table.insert({ 0, { 0, 1, 2, 3, 4, 5 } });
    table.insert({ 1, { 6, 7, 0, 1, 8, 9 } });
    table.insert({ 2, { 10, 11, 12, 13, 0, 1 } });

    input = { 0, 1, 2 };
    selected = selector(table, input);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, (selectedType{ 0, 1 }));
}

TEST_F(TestSelectorStd, three_columns_empty_ids)
{
    table.insert({ 3, {} });
    table.insert({ 1, {} });
    table.insert({ 2, {} });

    input = { 0, 1, 2 };
    selected = selector(table, input);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, selectedType{});
}

TEST_F(TestSelectorStd, three_columns_empty_result)
{
    table.insert({ 0, { 1, 2, 3, 4, 5 } });
    table.insert({ 1, { 2, 4, 6 } });
    table.insert({ 42, {} });
    input = { 0, 1, 42 };
    selected = selector(table, input);

    EXPECT_EQ(selected, selectedType{});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
