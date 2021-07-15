
#include "simplecs/generic/selector.h"
#include "simplecs/impl/selector.h"

#include <gtest/gtest.h>
#include <random>

using EntityId = size_t;
using ComponentId = size_t;

using SelectorStd = eld::generic::selector<eld::impl::selector_std<EntityId, ComponentId>>;

using SelectInputType = std::vector<ComponentId>;
using SelectedType = std::vector<ComponentId>;
using TableType = std::unordered_map<ComponentId, std::vector<EntityId>>;

class TestSelectorStd : public testing::Test
{
protected:
    void SetUp() override { Test::SetUp(); }
    void TearDown() override { Test::TearDown(); }
    SelectorStd selector{};
    TableType table{};
    SelectInputType inputComponents{};
    SelectedType selected{};
};
// TODO: test scenarios and implementations

TEST_F(TestSelectorStd, empty_table)
{
    inputComponents = { 4, 8, 15, 16, 23, 42 };

    selected = selector(table, inputComponents);
    EXPECT_TRUE(selected.empty());
}

TEST_F(TestSelectorStd, empty_select_input)
{
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 4, 8, 15 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 16, 23, 42 }));

    selected = selector(table, inputComponents);
    EXPECT_TRUE(selected.empty());
}

TEST_F(TestSelectorStd, single_column)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert(std::make_pair(ComponentId(0), Ids));
    inputComponents.push_back(0);
    selected = selector(table, inputComponents);
    std::sort(Ids.begin(), Ids.end());
    EXPECT_EQ(selected, Ids);
}

TEST_F(TestSelectorStd, single_column_empty_result_positive_values)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert(std::make_pair(ComponentId(0), Ids));
    inputComponents = std::vector<ComponentId>(1000);
    std::iota(inputComponents.begin(), inputComponents.end(), 1);
    std::shuffle(inputComponents.begin(),
                 inputComponents.end(),
                 std::mt19937{ std::random_device{}() });
    selected = selector(table, inputComponents);
    EXPECT_EQ(selected, SelectedType{});
}

TEST_F(TestSelectorStd, single_column_empty_result_negative_values)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });

    table.insert(std::make_pair(ComponentId(0), Ids));
    inputComponents = std::vector<ComponentId>(1000);
    std::iota(inputComponents.begin(), inputComponents.end(), -1000);
    std::shuffle(inputComponents.begin(),
                 inputComponents.end(),
                 std::mt19937{ std::random_device{}() });
    selected = selector(table, inputComponents);
    EXPECT_EQ(selected, SelectedType{});
}

TEST_F(TestSelectorStd, thousand_columns_equal_entt_ids)
{
    std::vector<EntityId> Ids(100);
    std::iota(Ids.begin(), Ids.end(), 0);
    std::shuffle(Ids.begin(), Ids.end(), std::mt19937{ std::random_device{}() });
    for (size_t i = 0; i < 1000; ++i)
        table.insert(std::make_pair(ComponentId(i), Ids));

    inputComponents = SelectInputType(1000);
    std::iota(inputComponents.begin(), inputComponents.end(), 0);
    std::shuffle(inputComponents.begin(),
                 inputComponents.end(),
                 std::mt19937{ std::random_device{}() });
    selected = selector(table, inputComponents);
    std::sort(Ids.begin(), Ids.end());
    EXPECT_EQ(selected, Ids);
}

TEST_F(TestSelectorStd, three_columns_not_equal_entt_ids)
{
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 0, 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 2, 3, 0, 1, 4, 5 }));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{ 4, 5, 2, 3, 0, 1 }));

    inputComponents = { 0, 1, 2 };
    selected = selector(table, inputComponents);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, (SelectedType{ 0, 1, 2, 3, 4, 5 }));
}

TEST_F(TestSelectorStd, three_columns_extra_numbers)
{
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 0, 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 6, 7, 0, 1, 8, 9 }));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{ 10, 11, 12, 13, 0, 1 }));

    inputComponents = { 0, 1, 2 };
    selected = selector(table, inputComponents);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, (SelectedType{ 0, 1 }));
}

TEST_F(TestSelectorStd, three_columns_empty_ids)
{
    table.insert(std::make_pair(ComponentId(3), std::vector<EntityId>{}));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{}));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{}));

    inputComponents = { 0, 1, 2 };
    selected = selector(table, inputComponents);
    std::sort(selected.begin(), selected.end());
    EXPECT_EQ(selected, SelectedType{});
}

TEST_F(TestSelectorStd, three_columns_empty_result)
{
    inputComponents = { 0, 1, 2 };

    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 2, 4, 6 }));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{}));

    selected = selector(table, inputComponents);
    EXPECT_EQ(selected, SelectedType{});

    table.clear();
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{}));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{ 2, 4, 6 }));

    selected = selector(table, inputComponents);
    EXPECT_EQ(selector(table, inputComponents), SelectedType{});

    table.clear();
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{}));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{ 2, 4, 6 }));

    selected = selector(table, inputComponents);
    EXPECT_EQ(selector(table, inputComponents), SelectedType{});

    table.clear();
    table.insert(std::make_pair(ComponentId(0), std::vector<EntityId>{ 45, 46, 101 }));
    table.insert(std::make_pair(ComponentId(1), std::vector<EntityId>{ 1, 2, 3, 4, 5 }));
    table.insert(std::make_pair(ComponentId(2), std::vector<EntityId>{ 2, 4, 6 }));

    selected = selector(table, inputComponents);
    EXPECT_EQ(selector(table, inputComponents), SelectedType{});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
