
#include "simplecs/c_api/c_core.hpp"

#include <gtest/gtest.h>

#include <vector>

// TODO: make tests

TEST(c_api_registry, reg_components)
{
    // TODO: implement

    FAIL();
}

TEST(c_api_registry, unreg_components)
{
    // TODO: implement

    FAIL();
}

/**
 * 1. Register components for several entities
 * 2. Select all entities with a set of components
 */
TEST(c_api_registry, selec_entities)
{
    using namespace eld::c_api;

    std::vector<component_descriptor> componentsEntity0{ component_descriptor{ 0 },   //
                                              component_descriptor{ 1 },   //
                                              component_descriptor{ 2 },   //
                                              component_descriptor{ 3 },   //
                                              component_descriptor{ 4 },   //
                                              component_descriptor{ 5 } },
        componentsEntity1{ component_descriptor{ 2 },   //
                           component_descriptor{ 3 },
                           component_descriptor{ 5 },
                           component_descriptor{ 6 } };

    entity_descriptor entity0{ 0 },   //
        entity1{ 1 };

    std::vector<reg_error> regResultExpected(componentsEntity0.size(), reg_error::success),
        regResult = regResultExpected;

    register_components(entity0,
                        componentsEntity0.data(),
                        componentsEntity0.size(),
                        regResult.data());

    ASSERT_EQ(regResult, regResultExpected);

    regResultExpected = std::vector(componentsEntity1.size(), reg_error::success),
        regResult = regResultExpected;

    register_components(entity1,
                        componentsEntity1.data(),
                        componentsEntity1.size(),
                        regResult.data());

    ASSERT_EQ(regResult, regResultExpected);

    entity_selection entitySelection{};

    std::vector<component_descriptor> selectComponents{
        component_descriptor{2},
        component_descriptor{3},
        component_descriptor{5}
    };

    select_entities_by_components(selectComponents.data(),
                                  selectComponents.size(),
                                  entitySelection);

    ASSERT_NE(entitySelection.array, nullptr);
    EXPECT_EQ(entitySelection.length, 2);

    free_entity_selection(entitySelection);
    EXPECT_EQ(entitySelection.handle, -1);
    EXPECT_EQ(entitySelection.array, nullptr);
    EXPECT_EQ(entitySelection.length, 0);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
