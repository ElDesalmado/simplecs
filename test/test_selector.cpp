
#include "simplecs/generic/selector.h"
#include "simplecs/impl/selector.h"

#include <gtest/gtest.h>

using EntityId = size_t;
using ComponentId = size_t;

using SelectorStd = eld::generic::selector<eld::impl::selector_std<EntityId, ComponentId>>;

// TODO: test scenarios and implementations

TEST(selector_std, empty_table)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> emptyTable{};
    SelectorStd selector{};

    std::vector<EntityId> selected =
        selector(emptyTable, std::vector<ComponentId>{ 4, 8, 15, 16, 23, 42 });
    ASSERT_TRUE(selected.empty());
}

TEST(selector_std, empty_select_input)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> table{
        std::make_pair(0, std::vector<EntityId>{ 4, 8, 15 }),
        std::make_pair(1, std::vector<EntityId>{ 16, 23, 42 })
    };
    SelectorStd selector{};

    std::vector<EntityId> selected = selector(table, std::vector<ComponentId>());
    ASSERT_TRUE(selected.empty());
}

TEST(selector_std, empty_input)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> table{};
    SelectorStd selector{};

    std::vector<EntityId> selected = selector(table, std::vector<ComponentId>());
    ASSERT_TRUE(selected.empty());
}

TEST(selector_std, single_column)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> table{
        std::make_pair(0, std::vector<EntityId>{ 4, 8, 15, 16, 23, 42 })
    };
    SelectorStd selector{};

    std::vector<EntityId> expected{ 4, 8, 15, 16, 23, 42 },
        selected = selector(table, std::vector<ComponentId>(0));

    ASSERT_EQ(selected, expected);
}

TEST(selector_std, single_column_empty_result)
{
    // TODO: implement
    FAIL();
}

TEST(selector_std, multiple_columns)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> table{
        std::make_pair(0, std::vector<EntityId>{ 1, 2, 3, 4, 5 }),
        std::make_pair(1, std::vector<EntityId>{ 2, 4, 6 })
    };
    SelectorStd selector{};

    std::vector<EntityId> expected{ 2, 4 },
        selected = selector(table, std::vector<ComponentId>{ 0, 1 });

    ASSERT_EQ(selected, expected);
}

TEST(selector_std, multiple_columns_2)
{
    std::unordered_map<ComponentId, std::vector<EntityId>> table{
        std::make_pair(0, std::vector<EntityId>{ 1, 2, 3, 4, 5 }),
        std::make_pair(1, std::vector<EntityId>{ 2, 4, 6 }),
        std::make_pair(42, std::vector<EntityId>())
    };
    SelectorStd selector{};

    std::vector<EntityId> expected{},
        selected = selector(table, std::vector<ComponentId>{ 0, 1, 42 });

    ASSERT_EQ(selected, expected);
}

TEST(selector_std, multiple_columns_empty_result)
{
    // TODO: implement
    FAIL();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
