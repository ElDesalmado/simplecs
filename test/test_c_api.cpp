
#include "simplesc/c_registry_api.h"

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

TEST(c_api_registry, selec_entities)
{
    // TODO: implement

    using namespace eld::c_api;

    std::vector<component_id> componentsEntity0{ component_id{ 0 },   //
                                              component_id{ 1 },   //
                                              component_id{ 2 },   //
                                              component_id{ 3 },   //
                                              component_id{ 4 },   //
                                              component_id{ 5 } },
        componentsEntity1{ component_id{ 2 },   //
                           component_id{ 3 },
                           component_id{ 5 },
                           component_id{ 6 } };

    entity entity0{ 0 },   //
        entity1{ 1 };

    std::vector<reg_result> regResultExpected(componentsEntity0.size(), reg_result::success),
        regResult = regResultExpected;

    register_components(entity0,
                        componentsEntity0.data(),
                        componentsEntity0.size(),
                        regResult.data());

    ASSERT_EQ(regResult, regResultExpected);

    regResultExpected = std::vector(componentsEntity1.size(), reg_result::success),
        regResult = regResultExpected;

    register_components(entity1,
                        componentsEntity1.data(),
                        componentsEntity1.size(),
                        regResult.data());

    ASSERT_EQ(regResult, regResultExpected);

    entity_selection entitySelection{};

    std::vector<component_id> selectComponents{
        component_id{2},
        component_id{3},
        component_id{5}
    };

    select_entities_by_components(selectComponents.data(),
                                  selectComponents.size(),
                                  entitySelection);

    ASSERT_NE(entitySelection.array, nullptr);
    EXPECT_EQ(entitySelection.length, 2);

    free_entity_selection(entitySelection);
    EXPECT_EQ(entitySelection.handle, invalid_id);
    EXPECT_EQ(entitySelection.array, nullptr);
    EXPECT_EQ(entitySelection.length, 0);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
